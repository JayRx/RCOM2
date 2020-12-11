#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

#include "download.h"
#include "parser.h"
#include "utils.h"
#include "getip.h"
#include "ftp.h"

int main(int argc, char **argv) {
  if (checkArgs(argc) != 0)
    exit(-1);

  ParsedURL parsedURL;

  if (parseURL(argv[1], &parsedURL) == -1) {
    printUsage();
    exit(-1);
  }

  if (getIP(parsedURL.host, &parsedURL) == -1) {
    printUsage();
    exit(-1);
  }

  printParsedURL(&parsedURL);

  int sockfd, newsockfd;
  
  if ((sockfd = open_socket(parsedURL.ip, FTP_PORT)) == -1) {
    printf("Error when opening socket!\n");
    exit(-1);
  }

  read_welcome(sockfd);

  if (ftp_login_user(sockfd, &parsedURL) != 0) {
    printf("Couldn't login user!\n");
    exit(-1);
  }

  if (ftp_enter_passive_mode(sockfd, &newsockfd) != 0) {
    printf("Couldn't enter passive mode!\n");
    exit(-1);
  }

  if (ftp_get_file_size(sockfd, &parsedURL) != 0) {
    printf("Couldn't get file size!\n");
    exit(-1);
  }

  if (ftp_retrieve_file(sockfd, &parsedURL) != 0) {
    printf("Couldn't retrieve file!\n");
    exit(-1);
  }

  if (ftp_download_file(newsockfd, &parsedURL) != 0) {
    printf("Couldn't download file!\n");
    exit(-1);
  }

  if (ftp_close_connection(sockfd) != 0) {
    printf("Couldn't close connection!\n");
    exit(-1);
  }

  close(sockfd);
  close(newsockfd);

  destructParsedURL(&parsedURL);

  return 0;
}

int checkArgs(int argc) {
  if (argc != 2) {
    printUsage();
    return -1;
  }

  return 0;
}
