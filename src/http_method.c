#include "http_method.h"
#include <stdio.h>
#include <string.h>
#include "define.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
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
    char path[256];
    struct stat file_stat;

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
        printf("buf: %s\n",buf);
        printf("request method: %s\n", method);
#endif

        if (strncasecmp(method , "GET", sizeof("GET")) == 0){  // 处理GET请求
#ifdef DEBUG
            printf("method = GET\n");
#endif
            // 获取url
            while (isspace(buf[j]))
            {
                j++;
            };
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
            // 2.读取http头部
            buf_len = 0;
            memset(buf, 0, sizeof(buf));
            do {
                buf_len = get_line(client_sock, buf, sizeof(buf));
#ifdef DEBUG
                printf("read: %s\n", buf);
#endif
            }while(buf_len > 0); 


            // 3.定位服务器的html文件
            {
                char* pos = strchr(url, '?');
                if(pos) {
                    *pos = '\0';
#ifdef DEBUG
                    printf("real url: %s\n", url);
#endif
                }
                sprintf(path, "./docs%s", url);
#ifdef DEBUG
                printf("path: %s\n",path);
#endif
                // 4.执行http响应
                // 判断文件是否存在，如果存在就响应200 , ok ，同时发送相应的html。如果不存在就响应404
                if(stat(path, &file_stat) == -1){
                    file_not_found(client_sock);
                }
                else {
                    do_http_response(client_sock);
                }
            }
        }
        else  // 处理非GET请求,读取http 头部，并响应客户端501  Method Not Implemented
        {
            fprintf(stderr, "warning! other request [%s]\n", method);
            buf_len = 0;
            memset(buf, 0, sizeof(buf));
            do {
                buf_len = get_line(client_sock, buf, sizeof(buf));
#ifdef DEBUG
                printf("read: %s\n", buf);
#endif
            }while(buf_len > 0); 

        }
    }


}

void do_http_response(int client_sock)
{

}

void file_not_found(int client_sock)
{
    const char* reply = "HTTP/1.0 404 NOT FOUND\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML>\r\n\
<HEAD>\r\n\
<TITLE>NOT FOUND</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
    <h1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</h1>\r\n\
</BODY>\r\n\
</HTML>";
    int len = write(client_sock, reply, strlen(reply));
#ifdef DEBUG
    fprintf(stdout, reply);
#endif

    if (len < 0) {
#ifdef DEBUG
    fprintf(stderr, "send reply failed : info : %s", strerror(errno));
#endif
    }

}