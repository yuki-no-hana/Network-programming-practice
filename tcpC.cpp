//
// Created by 赵星宇 on 2023/4/12.
//

#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>

#include<sys/socket.h>
using namespace std;

class tcpC {
public:
    int my_socket;
    sockaddr_in addr;
    sockaddr_in addr_port;
    int addr_port_len;

    int create() {
        memset(&addr, 0, sizeof(addr));
        my_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(my_socket == -1)     // 创建socket失败
        {
            perror("socket");
            exit(0);
        }
        return my_socket;
    };

    void client_connect(int server_port) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(server_port);   // 大端端口
        addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //默认走127.0.0.1的IP

        int ret = connect(my_socket, (struct sockaddr*)&addr, sizeof(addr));
        addr_port_len = sizeof(addr_port);
        int temp = getsockname(my_socket, (struct sockaddr*)&addr_port, (socklen_t*)&addr_port_len);                    
        if(ret == -1)           //if connection fails, close the socket we create and print error
        {
            close(my_socket);
            perror("connect");
            exit(0);
        }
    };

    int client_send(int sent_socket, string& buffer) {
        return send(sent_socket, buffer.c_str(), buffer.length(), 0);
    };

    int client_receive(int rec_socket, char* buffer) {
        // zxy
        memset(buffer, 0, 8192);
        return recv(rec_socket, buffer, 8192, 0);
    };

    ~tcpC() {
        close(my_socket);
    };
};