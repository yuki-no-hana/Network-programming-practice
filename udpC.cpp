//
// Created by 赵星宇 on 2023/4/15.
//
#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include <errno.h>
#include<cstring>

#include<sys/socket.h>

using namespace std;

class udpC {
    public:
        //int client_socket;
        sockaddr_in server_addr;

        int create_connect(int server_port) {
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            server_addr.sin_port = htons(server_port);

            int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
            return client_socket;
        };
        int udp_client_send(int client_socket, string& buffer) {
            int buffer_size = sizeof(buffer);
            socklen_t socket_size = sizeof(sockaddr_in);
            int result = sendto(client_socket, buffer.c_str(), buffer.size(), 0, (struct sockaddr *) &server_addr, socket_size);
            return result;
        };

        int udp_client_receive(int client_socket, char* buffer, int buffer_size){
            // int buffer_size = sizeof(buffer);
            memset(buffer, 0, buffer_size);
            socklen_t socket_size = sizeof(sockaddr_in);

            int result = recvfrom(client_socket, buffer, buffer_size, 0, (struct sockaddr *) &server_addr, &socket_size);
            return result;
        };
};
