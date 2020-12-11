#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "parser.h"
#include "getip.h"

int getIP(char* address, ParsedURL* parsedURL) {
  struct hostent *h;
  char* ip;
  int ip_size;

  if ((h = gethostbyname(address)) == NULL) {
    herror("gethostbyname");
    return -1;
  }

  ip_size = strlen(inet_ntoa(*((struct in_addr *) h->h_addr)));
  ip = (char*) malloc(ip_size * sizeof(char));
  strcpy(ip, inet_ntoa(*((struct in_addr *) h->h_addr)));

  parsedURL->ip = ip;

  return 0;
}
