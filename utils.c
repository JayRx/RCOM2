#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"

void printUsage() {
  printf("Usage: ./download <FTP-url>\n");
}

void printParsedURL(ParsedURL* parsedURL) {
  printf("\nProtocol: \t%s\n", parsedURL->protocol);
  printf("User: \t\t%s\n", parsedURL->user);
  printf("Password: \t%s\n", parsedURL->password);
  printf("Host: \t\t%s\n", parsedURL->host);
  printf("Path: \t\t%s\n\n", parsedURL->path);
}
