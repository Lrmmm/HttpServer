void do_http_request(int client_sock);
int get_line(int sock, char* buf, int size);
void do_http_response(int client_sock);
void file_not_found(int client_sock);