#include <stdio.h>
void do_http_request(int client_sock);
int get_line(int sock, char* buf, int size);
void do_http_response(int client_sock, const char* path);
void file_not_found(int client_sock);
void send_http_header(int client_sock, FILE* fp);
void send_file_body(int client_sock, FILE* fp);
void iner_error(int client_sock);