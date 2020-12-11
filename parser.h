#define FTP_PORT 21

typedef struct {
  char *protocol;
  char *user;
  char *password;
  char *host;
  char *path;
  char *filename;
  char *ip;
  unsigned long long int filesize;
} ParsedURL;

int parseURL(char* arg, ParsedURL* parsedURL);

int getProtocol(char** arg, ParsedURL* parsedURL);

int getUser(char** arg, ParsedURL* parsedURL);

int getHost(char** arg, ParsedURL* parsedURL);

int getPath(char** arg, ParsedURL* parsedURL);

int getFilename(char** arg, ParsedURL* parsedURL);

void destructParsedURL(ParsedURL* parsedURL);
