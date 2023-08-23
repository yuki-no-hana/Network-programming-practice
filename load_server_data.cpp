//
// Created by 赵星宇 on 2023/4/13.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iosfwd>
#include <vector>
#include "data_structure.cpp"


using namespace std;

string vec_time_to_string(vector<pair<int,int> > time_int) {
    string result = "";

    for (int i = 0; i < time_int.size(); i++) {
        result += "[" + to_string(time_int[i].first) + "," + to_string(time_int[i].second) + "],";
    }
    result = result.substr(0,result.size()-1);
    result = "[" + result + "]";
    return result;
}

string& remove_all_space(string &one_line_string) {
    int index = 0;
    while( (index = one_line_string.find(' ',index)) != string::npos)
    {
        one_line_string.erase(index,1);
    }
    return one_line_string;
};

vector<pair<int,int> > process_2d_string(string &one_line_string) {
    vector<pair<int,int> > real_input;
    one_line_string = one_line_string.substr(1,one_line_string.size()-2);

    int left[50], right[50];
    int left_point = 0;
    int right_point = 0;

    for (int i=0; i<one_line_string.size(); i+=1) {
        if (one_line_string[i] == '[') {
            left[left_point] = i;
            left_point += 1;
        }
        if (one_line_string[i] == ']') {
            right[right_point] = i;
            right_point += 1;
        }
    }

    for (int i=0; i<left_point; i+=1) {
        string temp_sub = one_line_string.substr(left[i]+1, right[i] - left[i] - 1);
        int divide_location = temp_sub.find(",");
        real_input.emplace_back(make_pair(stoi(temp_sub.substr(0,divide_location)),stoi(temp_sub.substr(divide_location+1,temp_sub.size()))));
    }

    return real_input;
}

vector<pair<int,int> > process_2d_string_nochange(string one_line_string) {
    // if input string is none
    vector<pair<int,int> > real_input;
    one_line_string = one_line_string.substr(1,one_line_string.size()-2);

    int left[50], right[50];
    int left_point = 0;
    int right_point = 0;

    for (int i=0; i<one_line_string.size(); i+=1) {
        if (one_line_string[i] == '[') {
            left[left_point] = i;
            left_point += 1;
        }
        if (one_line_string[i] == ']') {
            right[right_point] = i;
            right_point += 1;
        }
    }


    for (int i=0; i<left_point; i+=1) {
        string temp_sub = one_line_string.substr(left[i]+1, right[i] - left[i] - 1);
        int divide_location = temp_sub.find(",");
        real_input.emplace_back(make_pair(stoi(temp_sub.substr(0,divide_location)),stoi(temp_sub.substr(divide_location+1,temp_sub.size()))));
    }

    return real_input;
}



server_input process_server_input(string &one_line_string)
{
    // 去除整行数据的前后空格
    one_line_string.erase(0, one_line_string.find_first_not_of(" "));
    one_line_string.erase(one_line_string.find_last_not_of(" ") + 1);

    // 将整行数据分成两部分
    int divide_location = one_line_string.find(";");
    string username = one_line_string.substr(0,divide_location);
    string time_intervals = one_line_string.substr(divide_location+1,one_line_string.size());

    // 处理两部分的空格
    username.erase(username.find_last_not_of(" ")+1);
    time_intervals = remove_all_space(time_intervals);

    server_input real_input;

    real_input.worker_name = username;
    real_input.available_times = process_2d_string(time_intervals);

    return real_input;
};

server_inputs load_txt(string filename) {
    ifstream openfile;
    openfile.open(filename);

    string one_line_input;
    server_inputs real_input;

    int count = 0;
    while (getline(openfile,one_line_input))
    {
        server_input temp_input = process_server_input(one_line_input);
        real_input.single_input[count] = temp_input;
        count += 1;
    };
    real_input.length = count;
    return real_input;
}







/*int main() {
    server_inputs real_input = load_txt("b.txt");
    cout << real_input.single_input[10].worker_name <<endl;

};*/
