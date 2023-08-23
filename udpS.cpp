//
// Created by 赵星宇 on 2023/4/15.
//

#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include<cstring>

#include<sys/socket.h>

using namespace std;

class udpS{
    public:
        //int server_socket;   //md不知道为什么保存不了值，离大谱
        sockaddr_in my_addr;
        sockaddr_in client_addr;

        int create_bind(int server_port) {
            memset(&my_addr, 0, sizeof(my_addr));
            my_addr.sin_family = AF_INET;
            my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            my_addr.sin_port = htons(server_port);

            int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

            bind(server_socket, (struct sockaddr *)&my_addr, sizeof(my_addr));
            return server_socket;
        };
        int send(int server_socket, string& buffer) {
            int buffer_size = 8192;
            sendto(server_socket, buffer.c_str(), buffer.size(), 0, (struct sockaddr *) &client_addr, buffer_size);
            return 1;
        };

        int receive(int server_socket, char* buffer, int buffer_size){
            memset(buffer, 0, buffer_size);

            socklen_t socket_size = sizeof(client_addr);

            int result = recvfrom(server_socket, buffer, buffer_size, 0, (struct sockaddr *) &client_addr, &socket_size);
            return 1;
        };
};
