typedef struct {
  char *protocol;
  char *user;
  char *password;
  char *host;
  char *path;
  char *filename;
} ParsedURL;

int parseURL(char* arg, ParsedURL* parsedURL);

int getProtocol(char** arg, ParsedURL* parsedURL);

int getUser(char** arg, ParsedURL* parsedURL);

int getHost(char** arg, ParsedURL* parsedURL);

int getPath(char** arg, ParsedURL* parsedURL);
