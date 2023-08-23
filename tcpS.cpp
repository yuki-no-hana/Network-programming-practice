//
// Created by 赵星宇 on 2023/4/14.
//

#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>


#include<sys/socket.h>
using namespace std;

class tcpS {
public:
    int tcp_server_socket;
    int backlog;
    sockaddr_in server_addr, client_addr;

    int create(int port, int bl) {
        memset(&tcp_server_socket, 0, sizeof(tcp_server_socket));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);
        backlog = bl;       //指定的是等待被accept的全连接socket队列长度, 代替了未全连接请求的数量

        tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);
        return tcp_server_socket;
    };

    void tcp_bind() {
        bind(tcp_server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    };

    void tcp_listen() {
        listen(tcp_server_socket, backlog);
    };

    int tcp_accept() {
        struct sockaddr_in temp_child_addr;
        socklen_t temp_child_addr_size = sizeof(temp_child_addr);
        int cfd = accept(tcp_server_socket, (struct sockaddr*)&client_addr, &temp_child_addr_size);
        return cfd;
    };

    int tcp_server_send(int sent_socket, string& buffer){
        return send(sent_socket, buffer.c_str(), buffer.length(), 0);
    }

    int tcp_server_receive(int rec_socket, char* buffer){
        // TODO: 目前的buffersize是固定的
        memset(buffer, 0, 8192);
        return recv(rec_socket, buffer, 8192, 0);
    }

    ~tcpS() {
        close(tcp_server_socket);
    };
};
