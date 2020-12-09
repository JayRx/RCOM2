#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "download.h"
#include "parser.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (checkArgs(argc) != 0)
    exit(-1);

  ParsedURL parsedURL;

  if (parseURL(argv[1], &parsedURL) == -1) {
    printUsage();
    exit(-1);
  }

  printParsedURL(&parsedURL);

  return 0;
}

int checkArgs(int argc) {
  if (argc != 2) {
    printUsage();
    return -1;
  }

  return 0;
}
