#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "parser.h"

// ftp://[<user>:<password>@]<host>/<url-path>
int parseURL(char* arg, ParsedURL* parsedURL) {
  if (getProtocol(&arg, parsedURL) == -1) return -1;
  if (getUser(&arg, parsedURL) == -1) return -1;
  if (getHost(&arg, parsedURL) == -1) return -1;
  if (getPath(&arg, parsedURL) == -1) return -1;

  return 0;
}

int getProtocol(char** arg, ParsedURL* parsedURL) {
  char* rest;
  char* protocol;
  int rest_index;

  // Get the position where the protocol ends
  rest = strstr(*arg, "://");
  if (rest == NULL) {
    return -1;
  }
  rest_index = (int) ((rest - *arg) / sizeof(char));

  // Parse Protocol and store it in struct
  protocol = (char*) malloc(rest_index * sizeof(char));
  memcpy(protocol, *arg, rest_index);
  parsedURL->protocol = protocol;

  // Ignore ://
  rest += 3 * sizeof(char);
  *arg = rest;

  return 0;
}

int getUser(char** arg, ParsedURL* parsedURL) {
  char* rest;
  char* rest2;
  char* user;
  char* password;
  int rest_index;

  // There is no user
  if (*arg[0] != '[') {
    parsedURL->user = NULL;
    parsedURL->password = NULL;
    return 1;
  }

  //There is a user

  // Ignore [
  *arg += 1 * sizeof(char);

  rest = strstr(*arg, ":");

  // There is no password
  if (rest == NULL) {
    // Get the position where the user ends
    rest2 = strstr(*arg, "@");
    if (rest2 == NULL)
      return -1;
    rest_index = (int) ((rest2 - *arg) / sizeof(char));

    // Parse User
    user = (char*) malloc(rest_index * sizeof(char));
    memcpy(user, *arg, rest_index);
    parsedURL->user = user;

    // Ignore user@]
    rest2 += 2 * sizeof(char);
    *arg = rest2;

    parsedURL->password = NULL;

    return 2;
  }

  // There is a password

  // Get the position where the user ends
  rest_index = (int) ((rest - *arg) / sizeof(char));

  // Parse User
  user = (char*) malloc(rest_index * sizeof(char));
  memcpy(user, *arg, rest_index);
  parsedURL->user = user;

  // Ignore user:
  *arg += (rest_index + 1) * sizeof(char);

  // Get the position where the password ends
  rest2 = strstr(*arg, "@");
  if (rest2 == NULL)
    return -1;
  rest_index = (int) ((rest2 - 1 - rest) / sizeof(char));

  // Parse password
  password = (char*) malloc(rest_index * sizeof(char));
  memcpy(password, *arg, rest_index);
  parsedURL->password = password;

  // Ignore user@]
  rest2 += 2 * sizeof(char);
  *arg = rest2;

  return 0;
}

int getHost(char** arg, ParsedURL* parsedURL) {
  char* rest;
  char* host;
  int rest_index;

  // Get the position where the protocol ends
  rest = strstr(*arg, "/");
  if (rest == NULL) {
    return -1;
  }
  rest_index = (int) ((rest - *arg) / sizeof(char));

  // Parse Protocol and store it in struct
  host = (char*) malloc(rest_index * sizeof(char));
  memcpy(host, *arg, rest_index);
  parsedURL->host = host;

  // Ignore /
  rest += 1 * sizeof(char);
  *arg = rest;

  return 0;
}

int getPath(char** arg, ParsedURL* parsedURL) {
  char* path;

  // Parse Path and store it in struct
  path = (char*) malloc(sizeof(*arg) * sizeof(char));
  strcpy(path, *arg);
  parsedURL->path = path;

  return 0;
}
