//
// Created by 赵星宇 on 2023/4/14.
//

# include "tcpS.cpp"
# include "udpC.cpp"
# include "udpS.cpp"
# include <iostream>
# include "load_server_data.cpp"
# include "net_infor_process.cpp"

using namespace std;

int main() {
    // create TCP server
    tcpS server_tcp = tcpS();
    int server_tcp_socket = server_tcp.create(24829, 128);
    int opt = 1;
    setsockopt(server_tcp_socket,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));
    server_tcp.tcp_bind();
    server_tcp.tcp_listen();

    // create and bind two UDP client and server socket, send message to tell servera and serverb to send names
    udpC udp_client_a = udpC();
    udpS udp_server = udpS();
    udpC udp_client_b = udpC();
    int udp_server_socket = udp_server.create_bind(23829);
    int udp_client_a_socket = udp_client_a.create_connect(21829);
    int udp_client_b_socket = udp_client_b.create_connect(22829);
    cout << "Main Server is up and running" << endl;

    // obtain names from serverA and serverB
    string ready = "serverM is boot up";
    udp_client_a.udp_client_send(udp_client_a_socket, ready);
    char buf_temp_total_names[8192];
    udp_server.receive(udp_server_socket, buf_temp_total_names, 8192);
    vector<string > a_total_names = sting_names_to_string_list(string(buf_temp_total_names));
    cout << "Main Server received the username list from server A using UDP over port 23829." << endl;

    udp_client_b.udp_client_send(udp_client_b_socket, ready);
    udp_server.receive(udp_server_socket, buf_temp_total_names, 8192);
    vector<string > b_total_names = sting_names_to_string_list(string(buf_temp_total_names));
    cout << "Main Server received the username list from server B using UDP over port 23829." << endl;

    char user_input_buf[8192];
    char a_intervals[8192];
    char b_intervals[8192];
    char buf[8192];
    char determine_time[8192];

    int client_tcp_socket = server_tcp.tcp_accept();

    while (1) {
        // Obtain user input names
        server_tcp.tcp_server_receive(client_tcp_socket, user_input_buf);
        vector<string > vector_user_input = process_client_input(string(user_input_buf));
        cout << "Main Server received the request from client using TCP over port 24829" <<endl;

        // determine which part is A and B, which is not exist
        vector<string > vector_user_input_a = obtain_intersection(vector_user_input, a_total_names);
        vector<string > vector_user_input_b = obtain_intersection(vector_user_input, b_total_names);
        string difference_vector = obtain_difference(vector_user_input, a_total_names, b_total_names);
        if (!difference_vector.empty()) {
            cout << difference_vector << " do not exist. Send a reply to the client." << endl;
        }
        if (!vector_user_input_a.empty()) {
            cout << "Found "<< selected_vector_name_to_string(vector_user_input_a) << " located at Server A Send to Server A." << endl;
        }
        if (!vector_user_input_b.empty()) {
            cout << "Found "<< selected_vector_name_to_string(vector_user_input_b) << " located at Server B Send to Server B." << endl;
        }

        // send incorrect usernames to client. if all is correct, send "nobody"
        if (!difference_vector.empty()) {
            server_tcp.tcp_server_send(client_tcp_socket, difference_vector);
            server_tcp.tcp_server_receive(client_tcp_socket, buf);
        }
        else {
            string all_in = "Nobody";
            // cout << "when nobody not in a b send " << all_in << endl;
            server_tcp.tcp_server_send(client_tcp_socket, all_in);
            server_tcp.tcp_server_receive(client_tcp_socket, buf);
        }

        // send user input names to A and get time intervals
        string string_user_input_a = selected_vector_name_to_string(vector_user_input_a);  //input kong output ""
        if (string_user_input_a.empty()) {string_user_input_a = "noone";}
        udp_client_a.udp_client_send(udp_client_a_socket, string_user_input_a);
        udp_server.receive(udp_server_socket, a_intervals, 8192);   //input none or no intersaction return "[]"
        vector<pair<int,int> > a_interval_vector;
        if (string(a_intervals) != "[]") {
            a_interval_vector = process_2d_string_nochange(string(a_intervals));
            cout << "Main Server received from server A the intersection result using UDP over port 23829:" << endl;
            cout << string(a_intervals) << endl;

        }

        // send user input names to B and get time intervals
        string string_user_input_b = selected_vector_name_to_string(vector_user_input_b);
        if (string_user_input_b.empty()) {string_user_input_b = "noone";}
        udp_client_b.udp_client_send(udp_client_b_socket, string_user_input_b);
        udp_server.receive(udp_server_socket, b_intervals, 8192);
        vector<pair<int,int> > b_interval_vector;
        if (string(b_intervals) != "[]") {
            b_interval_vector = process_2d_string_nochange(string(b_intervals));
            cout << "Main Server received from server B the intersection result using UDP over port 23829:" << endl;
            cout << string(b_intervals) << endl;
        }

        // send final time interval to client, if no intersaction, send "[]"
        string final_interval;
        final_interval = main_server_intersaction(a_interval_vector, b_interval_vector);
        cout << "Found the intersection between the results from server A and B:" << endl;
        cout << final_interval << endl;
        server_tcp.tcp_server_send(client_tcp_socket, final_interval);
        cout << "Main Server sent the result to the client." << endl;




        // Extra credit
        server_tcp.tcp_server_receive(client_tcp_socket, determine_time);
        if (string(determine_time) != "no") {
            if (!vector_user_input_a.empty()) {
                string string_determine_time = string(determine_time);
                udp_client_a.udp_client_send(udp_client_a_socket, string_determine_time);
            }
            if (!vector_user_input_b.empty()) {
                string string_determine_time = string(determine_time);
                udp_client_b.udp_client_send(udp_client_b_socket, string_determine_time);
            }
        }

        // cout << "-----------------start a new---------------" << endl;
    }

    //string temp = "完成连接";
    //server_tcp.tcp_server_send(client_tcp_socket, temp);




    //bianli_vector(a_total_names);
    //cout << "bbbbbbbbbb" << endl;
    //bianli_vector(b_total_names);


   

    // tcp信息交互测试
    /*int client_tcp_socket = server_tcp.tcp_accept();
    char buf[8192];
    string a = "here is serverM";
    memset(buf, 0, sizeof(buf));
    int receive_result = server_tcp.tcp_server_receive(client_tcp_socket, buf);
    cout << "main server receive "<< buf <<endl;
    server_tcp.tcp_server_send(client_tcp_socket, a);
    cout << "main server send "<< a <<endl;*/

    return 1;

}