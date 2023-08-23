//
// Created by 赵星宇 on 2023/4/14.
//

#include <vector>
#include <string>
#include <cstring>
#include <iosfwd>

using namespace std;

struct server_input {
    string worker_name;
    vector<pair<int,int> > available_times;
};

struct server_inputs {
    int length;
    server_input single_input[200];
};