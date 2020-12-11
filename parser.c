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
  if (getFilename(&arg, parsedURL) == -1) return -1;

  if (parsedURL->user == NULL) {
    char *user, *password;
    user = (char*) malloc(32 * sizeof(char));
    password = (char*) malloc(32 * sizeof(char));

    strcpy(user, "anonymous");
    strcpy(password, "12345");

    parsedURL->user = user;
    parsedURL->password = password;
  }

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

  rest2 = strstr(*arg, "@");

  // There is no user

  if (rest2 == NULL) {
    parsedURL->user = NULL;
    parsedURL->password = NULL;
    return 1;
  }

  //There is a user

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

    // Ignore @
    rest2 += 1 * sizeof(char);
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

  // Ignore @
  rest2 += 1 * sizeof(char);
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
  char* arg_aux = *arg;
  char* rest;
  char* rest2;
  char* path;
  int rest_index;

  // Get the position where the path ends
  do {
    rest = strstr(arg_aux, "/");
    if (rest != NULL)
      rest2 = rest;
    arg_aux = rest + 1 * sizeof(char);
  } while(rest != NULL);

  rest2 += 1 * sizeof(char); // Add "/"

  rest_index = (int) ((rest2  - *arg) / sizeof(char));

  // Parse Path and store it in struct
  path = (char*) malloc(rest_index * sizeof(char));
  memcpy(path, *arg, rest_index);
  parsedURL->path = path;

  *arg = rest2;

  return 0;
}

int getFilename(char** arg, ParsedURL* parsedURL) {
  char* filename;

  // Parse Filename and store it in struct
  filename = (char*) malloc(sizeof(*arg) * sizeof(char));
  strcpy(filename, *arg);
  parsedURL->filename = filename;

  return 0;
}

void destructParsedURL(ParsedURL* parsedURL) {
  free(parsedURL->protocol);
  free(parsedURL->user);
  free(parsedURL->password);
  free(parsedURL->host);
  free(parsedURL->path);
  free(parsedURL->filename);
  free(parsedURL->ip);
}
