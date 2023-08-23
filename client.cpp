//
// Created by 赵星宇 on 2023/4/14.
//


# include "tcpC.cpp"
# include <iostream>
# include "load_server_data.cpp"
# include "net_infor_process.cpp"


using namespace std;

int main() {
    // create and connect tcp
    tcpC client_tcp = tcpC();
    int client_tcp_socket = client_tcp.create();
    client_tcp.client_connect(24829);
    cout << "Client is up and running." <<endl;

    char buf[8192];
    char time_intervals[8192];
    while (1) {
        // receive user input and preprocess the input
        cout << "Please enter the usernames to check schedule availability:" <<endl;
        string string_user_input;
        getline( cin,string_user_input);
        string_user_input.erase(0, string_user_input.find_first_not_of(" "));
        string_user_input.erase(string_user_input.find_last_not_of(" ") + 1);

        // send request name to main server
        client_tcp.client_send(client_tcp_socket, string_user_input);
        vector<string > vector_user_input = process_client_input(string_user_input);
        cout << "Client finished sending the usernames to Main Server." <<endl;

        // receive none-exist user and print
        string valid_vector_user_input_string = string_user_input;
        vector<string > valid_vector_user_input_vec = vector_user_input;

        client_tcp.client_receive(client_tcp_socket, buf);  // buf is the invalid username
        string send_i_receive_invalid = "hh";
        client_tcp.client_send(client_tcp_socket, send_i_receive_invalid);
        if (string(buf) != "Nobody") {
            valid_vector_user_input_vec = valid_client_input(vector_user_input, string(buf));
            valid_vector_user_input_string = selected_vector_name_to_string(valid_vector_user_input_vec);
            cout << "Client received the reply from Main Server using TCP over port "<< ntohs(client_tcp.addr_port.sin_port)<< endl;
            cout << string(buf) << " do not exist" <<endl;
        }

        client_tcp.client_receive(client_tcp_socket, time_intervals);
        cout << "Client received the reply from Main Server using TCP over port "<< ntohs(client_tcp.addr_port.sin_port)<<": Time intervals " << time_intervals << " works for " << valid_vector_user_input_string << endl;



        //extra credit
        if (string(time_intervals) != "[]") {
            cout << "Please enter the final meeting time to register an meeting:" << endl;
            string user_time_decision;
            int correct_time_decision = 0;
            while (correct_time_decision != 1) {
                cin >> user_time_decision;
                cin.ignore();
                if (string(user_time_decision).size() >= 5){
                    user_time_decision = remove_all_space(user_time_decision);
                    pair<int, int> vec_user_time_decision = process_user_time_decision(user_time_decision);
                    correct_time_decision = judge_user_time_decision(string(time_intervals), vec_user_time_decision);
                    if (vec_user_time_decision.first == vec_user_time_decision.second) {
                        correct_time_decision = 0;
                    }
                }
                if (correct_time_decision == 0) {cout << "Time interval "<< user_time_decision << " is not valid. Please enter again:" << endl;}
            }
            client_tcp.client_send(client_tcp_socket, user_time_decision);
        }
        else {
            string no_credit = "no";
            client_tcp.client_send(client_tcp_socket, no_credit);
            cout << "since there is no intersaction time, you can not choose a time for meeting" << endl;
        }





        cout << "-----Start a new request-----" << endl;
        // receive time interval

        //
        //cout << "Client received the reply from Main Server using TCP over port 24819" << endl;
        //cout << "Time intervals " <<  << " works for " <<  << endl;




    }

    return 1;

}