#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

void printUsage() {
  printf("Usage: ./download <FTP-url>\n");
}

void printParsedURL(ParsedURL* parsedURL) {
  printf("\nProtocol: \t%s\n", parsedURL->protocol);
  printf("User: \t\t%s\n", parsedURL->user);
  printf("Password: \t");
  printPassword(parsedURL->password);
  printf("Host: \t\t%s\n", parsedURL->host);
  printf("IP: \t\t%s\n", parsedURL->ip);
  printf("Path: \t\t%s\n", parsedURL->path);
  printf("Filename: \t%s\n\n", parsedURL->filename);
}

void printProgressBar(unsigned long long int current, unsigned long long int total) {
  float percentage = 100.0 * (float) current / (float) total;

  printf("\r[");

  int i, len = 30;
  int pos = percentage * len / 100.0;

  for (i = 0; i < len; i++)
    i <= pos ? printf("■") : printf(" ");

  printf("] %6.2f%%", percentage);

  fflush(stdout);
}

void printPassword(char *password) {
  if (password == NULL) return;
  for (int i = 0; i < (int) strlen(password); i++) {
    printf("*");
  }
  printf("\n");
}
