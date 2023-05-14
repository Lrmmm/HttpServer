#include "http_method.h"
#include <stdio.h>
int get_line(int sock, char* buf, int size)
{
    int count = 0;
    char ch ='\0';
    int len = 0;

    while ((count < size - 1) && ch != '\n')
    {
        len = read(sock, &ch, 1);
        if (len == 1) {
            if (ch == '\r') {
                continue;
            }
            else if(ch == '\n') {
                break;
            }

            buf[count] = ch;
            count++;
        }
        else if (len == -1) {
            perror("read failed");
            count = -1;
            break;
        }
        else {
            fprintf(stderr,"client close. \n");
            count = -1;
            break;
        }
    }

    if (count >= 0)
        buf[count] = '\0';
    
    return count;
    

}
void do_http_request(int client_sock)
{
    int buf_len = 0;
    char buf[256];

    // 读取客户端发送的http请求

    // 1.读取请求行
    do {
        buf_len = get_line(client_sock, buf, sizeof(buf));
        printf("read line : %s\n", buf);
    }while(buf_len > 0);
    // buf_len = get_line(client_sock, buf, sizeof(buf));
    printf("ok");
}