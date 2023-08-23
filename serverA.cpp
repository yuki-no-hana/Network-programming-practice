//
// Created by 赵星宇 on 2023/4/14.
//

# include "udpS.cpp"
# include "udpC.cpp"
# include <iostream>
# include "load_server_data.cpp"
# include "net_infor_process.cpp"
//using namespace std;

int main() {
    // load txt
    string input_txt_path = "a.txt";
    server_inputs data = load_txt(input_txt_path);
    string total_a_names = server_input_names_to_string(data);

    // create udp server and client socket
    udpS udp_server = udpS();
    udpC udp_client = udpC();
    int udp_a_client_socket = udp_client.create_connect(23829);
    int udp_a_server_socket = udp_server.create_bind(21829);
    cout << "Server A is up and running using UDP on port " << "21829" <<endl;

    // wait and send names to Main server
    char buf[8192];
    char determine_time[8192];
    udp_server.receive(udp_a_server_socket, buf, 8192); //等serverM启动，发人员名单
    udp_client.udp_client_send(udp_a_client_socket, total_a_names);
    cout << "Server A finished sending a list of usernames to Main Server." <<endl;


    while (1) {
        // obtain user input from main server
        udp_server.receive(udp_a_server_socket, buf, 8192); //等serverM启动，发人员名单
        if (string(buf) != "noone") {
            vector<string> user_input_name = sting_names_to_string_list(string(buf));
            if (!user_input_name.empty()) {
                cout << "Server A received the usernames from Main Server using UDP over port 21829." << endl;
                vector<pair<int, int> > shared_time_interval = obtain_inter_from_names(data, user_input_name);
                string shared_time_interval_string = vec_time_to_string(shared_time_interval);      //input none output "[]"
                cout << "Found the intersection result: " << shared_time_interval_string << " for "<< string(buf) << endl;
                udp_client.udp_client_send(udp_a_client_socket, shared_time_interval_string);
                cout << "Server A finished sending the response to Main Server." << endl;


                // Extra credits
                udp_server.receive(udp_a_server_socket, determine_time, 8192);
                pair<int, int> vec_time_decision = process_user_time_decision(string(determine_time));
                cout <<"Register a meeting at "<< determine_time <<" and update the availability for the following users:" << endl;

                for (int i = 0; i < user_input_name.size(); i++) {
                    int idx = find_index_in_server_inputs(data, user_input_name[i]);
                    // change
                    for (int j = 0; j < data.single_input[idx].available_times.size(); j++) {
                        if (vec_time_decision.first >= data.single_input[idx].available_times[j].first and vec_time_decision.second <= data.single_input[idx].available_times[j].second) {
                            string origin_data = vec_time_to_string(data.single_input[idx].available_times);
                            pair<int, int> temp_pair = data.single_input[idx].available_times[j];
                            pair<int, int> temp_small;
                            pair<int, int> temp_big;

                            data.single_input[idx].available_times.erase(data.single_input[idx].available_times.begin()+j);

                            temp_small.first = temp_pair.first;
                            temp_small.second = vec_time_decision.first;
                            temp_big.first = vec_time_decision.second;
                            temp_big.second = temp_pair.second;

                            if (temp_big.first != temp_big.second) {data.single_input[idx].available_times.insert(data.single_input[idx].available_times.begin()+j, temp_big);}
                            if (temp_small.first != temp_small.second) {data.single_input[idx].available_times.insert(data.single_input[idx].available_times.begin()+j, temp_small);}

                            string new_data = vec_time_to_string(data.single_input[idx].available_times);
                            cout << data.single_input[idx].worker_name << ": updated from " << origin_data << " to " << new_data << endl;
                        }
                    }
                }
                cout << "Notified Main server that registration has finished" << endl;
            }
            else {
                string return_empty_string = "[]";
                udp_client.udp_client_send(udp_a_client_socket, return_empty_string);
            }
        }
        else {
            string return_empty_string = "[]";
            udp_client.udp_client_send(udp_a_client_socket, return_empty_string);
        }
        cout << endl;

    }



    cout << "serverA work done" <<endl;
}