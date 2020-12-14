#define MAX_SIZE 256
#define TRANSFER_BUFFER_SIZE 256

typedef enum {ASCII, BINARY} FTP_TRANSFER_MODES;

int read_welcome(int sockfd);

int ftp_login_user(int sockfd, ParsedURL* parsedURL);

int ftp_enter_passive_mode(int sockfd, int* newsockfd);

int ftp_get_file_size(int sockfd, ParsedURL* parseURL);

int ftp_set_transfer_mode(int sockfd, FTP_TRANSFER_MODES transfer_mode);

int ftp_retrieve_file(int sockfd, ParsedURL* parsedURL);

int ftp_download_file(int sockfd, ParsedURL* parsedURL, int progressbar_status);

int ftp_close_connection(int sockfd);

int ftp_send_command(int socket_id, char* command);

int open_socket(char* ip, int port);

int read_socket(int sockfd, char* answer);
