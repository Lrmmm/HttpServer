#include "http_method.h"
#include <stdio.h>
#include <string.h>
#include "define.h"
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
    char method[64];
    char url[256];

    // 读取客户端发送的http请求

    // 1.读取请求行
    buf_len = get_line(client_sock, buf, sizeof(buf));

    if (buf_len > 0)  // 读取第一行成功
    {
        int i = 0;
        int j = 0;
        while (!isspace(buf[j]) && (i < sizeof(method) - 1))
        {
            method[i] = buf[j];
            i++;
            j++;
        }
        method[i] = '\0';
#ifdef DEBUG
        printf("request method: %s\n", method);
#endif

        if (strncasecmp(method , "GET", sizeof("GET")) == 0){
#ifdef DEBUG
            printf("method = GET\n");
#endif
            // 获取url
            while (isspace(buf[j++]));
            i = 0;
            
            while (!isspace(buf[j]) && (i < sizeof(url) - 1))
            {
                url[i] = buf[j];
                i++;
                j++;
            }

            url[i] = '\0';

#ifdef DEBUG
            printf("url: %s\n", url);
#endif          
        }
    }

    // 2.读取http头部
    buf_len = 0;
    memset(buf, 0, sizeof(buf));

    do {
        buf_len = get_line(client_sock, buf, sizeof(buf));
#ifdef DEBUG
        printf("read: %s\n", buf);
#endif
    }while(buf_len > 0);
}