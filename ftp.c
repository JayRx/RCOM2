#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parser.h"
#include "ftp.h"
#include "utils.h"


int read_welcome(int sockfd) {
  char answer[MAX_SIZE];
  read_socket(sockfd, answer);

  printf("\n");

  return 0;
}

int ftp_login_user(int sockfd, ParsedURL* parsedURL) {
  char command[MAX_SIZE];
  char answer[MAX_SIZE];

  sprintf(command, "user %s\r\n", parsedURL->user);
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  read_socket(sockfd, answer);

  printf("\n");

  sprintf(command, "pass %s\r\n", parsedURL->password);
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  read_socket(sockfd, answer);

  printf("\n");

  return 0;
}

int ftp_enter_passive_mode(int sockfd, int* newsockfd) {
  char new_ip[MAX_SIZE];
  char command[MAX_SIZE];
  char answer[MAX_SIZE];
  int ip1, ip2, ip3, ip4;
  int port1, port2, new_port;

  sprintf(command, "pasv\r\n");
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  read_socket(sockfd, answer);

  sscanf(answer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &port1, &port2);

  sprintf(new_ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);

  printf("Port1: %d\n", port1);
  printf("Port2: %d\n", port2);

  new_port = port1 * 256 + port2;

  printf("IP: %s\n", new_ip);
	printf("PORT: %d\n", new_port);

  if ((*newsockfd = open_socket(new_ip, new_port)) < 0) {
    perror("socket()");
    return -1;
  }

  printf("\n");

  return 0;
}

int ftp_get_file_size(int sockfd, ParsedURL* parsedURL) {
  char command[MAX_SIZE];
  char answer[MAX_SIZE];
  char fullFilePath[MAX_SIZE];
  unsigned long long int filesize;

  sprintf(fullFilePath, "%s%s", parsedURL->path, parsedURL->filename);
  sprintf(command, "size %s\r\n", fullFilePath);
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  read_socket(sockfd, answer);
  sscanf(answer, "213 %lld", &filesize);

  parsedURL->filesize = filesize;

  printf("\n");

  return 0;
}

int ftp_retrieve_file(int sockfd, ParsedURL* parsedURL) {
  char command[MAX_SIZE];
  char answer[MAX_SIZE];
  char fullFilePath[MAX_SIZE];

  sprintf(fullFilePath, "%s%s", parsedURL->path, parsedURL->filename);
  sprintf(command, "retr %s\r\n", fullFilePath);
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  return 0;
}

int ftp_download_file(int sockfd, ParsedURL* parsedURL, int progressbar_status) {
  char buffer[TRANSFER_BUFFER_SIZE];
  char c;
  int filefd;
  int res;

  unsigned long long int bytes_transfered = 0;

  if ((filefd = open(parsedURL->filename, O_WRONLY | O_CREAT, 0666)) < 0) {
    perror("open()");
    return -1;
  }

  do {
    res = read(sockfd, buffer, sizeof(buffer));
    printf("res: %d\n", res);

    if (res < 0) {
      perror("read()");
      return -1;
    }

    if (write(filefd, buffer, res) < 0) {
      perror("write()");
      return -1;
    }

    bytes_transfered += res;

    if (progressbar_status == PROGRESSBAR_SHOW)
      printProgressBar(bytes_transfered, parsedURL->filesize);
  } while (res != 0);

  close(filefd);

  printf("\n\n");

  return 0;
}

int ftp_close_connection(int sockfd) {
  char command[MAX_SIZE];
  char answer[MAX_SIZE];

  sprintf(command, "quit\r\n");
  if (ftp_send_command(sockfd, command) != 0)
    return -1;

  read_socket(sockfd, answer);

  return 0;
}

int ftp_send_command(int sockfd, char* command) {
  int command_size = strlen(command);
  int res = write(sockfd, command, command_size);

  if (res <= 0) {
    perror("ftp_send_command()\n");
    return -1;
  }

  printf ("\e[38;5;050m");
  printf("%s\n", command);
  printf ("\e[38;5;255m");

  return 0;
}

int open_socket(char* ip, int port) {
  int sockfd;
  struct sockaddr_in server_addr;
  char buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
  int bytes;

  // Server address handling
  bzero((char*) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip); // 32 bit Internet address network byte ordered
  server_addr.sin_port = htons(port); // Server TCP port must be network byte ordered

  // Open a TCP socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    return -1;
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr*) & server_addr, sizeof(server_addr)) < 0) {
    perror("connect()");
    exit(-1);
  }

  return sockfd;
}

int read_socket(int sockfd, char* answer) {
  int res;
  do {
    memset(answer, 0, MAX_SIZE);
    res = read(sockfd, answer, MAX_SIZE);
    printf("%s", answer);
  } while(!('1' <= answer[0] && answer[0] <= '5') || answer[3] != ' ');

  return 0;
}
