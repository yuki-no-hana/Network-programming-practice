//
// Created by 赵星宇 on 2023/4/15.
//

// 这里因为serverb有了就没倒进去
//# include "data_structure.cpp"
# include<list>
# include <string>
# include <vector>
# include<algorithm>

using namespace std;

void bianli_vector(vector<string> b_infor) {
    for (int i = 0; i < b_infor.size(); ++i)
    {
        cout << b_infor[i] << " " <<  b_infor[i].size() << endl;
    }
}

void bianli_time_interval(vector<pair<int,int> > b_infor) {
    for (int i = 0; i < b_infor.size(); ++i)
    {
        cout << b_infor[i].first << " " <<  b_infor[i].second << endl;
    }
}

string server_input_names_to_string(server_inputs si) {
    int length = si.length;
    string output = "";
    for (int i = 0; i < length; i += 1) {
        output += si.single_input[i].worker_name + ',';
    }
    return output.substr(0, output.length()-1);
}

vector<string> sting_names_to_string_list(string names) {
    vector<string > result;
    int loc[200];
    loc[0] = -1;
    int point = 1;

    if (names.empty()) {
        return result;
    }

    for (int i=0; i<names.size(); i+=1) {
        if (names[i] == ',') {
            loc[point] = i;
            point += 1;
        }
    }
    loc[point] = names.size();
    point += 1;

    for (int i=1; i<point; i += 1) {
        result.push_back(names.substr(loc[i-1]+1,loc[i]-loc[i-1]-1));
    }
    return result;
}

vector<string> process_client_input(string names) {
    vector<string > result;
    int loc[200];
    loc[0] = -1;
    int point = 1;

    for (int i=0; i<names.size(); i+=1) {
        if (names[i] == ' ') {
            loc[point] = i;
            point += 1;
        }
    }
    loc[point] = names.size();
    point += 1;

    for (int i=1; i<point; i += 1) {
        if (!names.substr(loc[i-1]+1,loc[i]-loc[i-1]-1).empty()) {
            result.push_back(names.substr(loc[i-1]+1,loc[i]-loc[i-1]-1));
        }
    }
    return result;
}

string selected_vector_name_to_string(vector<string> vec_name) {
    int length = vec_name.size();
    string output = "";
    for (int i = 0; i < length; i += 1) {
        output += vec_name[i] + ',';
    }
    return output.substr(0, output.length()-1);
}

bool if_string_in_vector(vector<string> vec, string ele){
    vector<string>::iterator ite;
    ite=find(vec.begin(),vec.end(),ele);
    if (ite != vec.end()){
        return true;
    }
    else{
        return false;
    }
}

int find_index_in_server_inputs(server_inputs total_data, string name) {
    for (int i=0; i < total_data.length; i++) {
        if (total_data.single_input[i].worker_name == name) {
            return i;
        }
    }
    return -1;
}


vector<string> obtain_intersection(vector<string> vec1,vector<string> vec2){
    vector<string> result;
    sort(vec1.begin(),vec1.end());
    sort(vec2.begin(),vec2.end());
    set_intersection(vec1.begin(),vec1.end(),vec2.begin(),vec2.end(),back_inserter(result));//求交集
    return result;
}

string obtain_difference(vector<string> vec1, vector<string> vec2, vector<string> vec3) {
    // find strings in vec1 but not in vec2
    string result = "";

    for (int i = 0; i < vec1.size(); i+=1) {
        if (!if_string_in_vector(vec2, vec1[i]) and !if_string_in_vector(vec3, vec1[i])) {
            result += vec1[i] + ",";
        }
    }
    /*set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(),
                   back_inserter(result));*/

    return result.substr(0, result.size()-1);
}

vector<pair<int,int> >  obtain_time_from_name(server_inputs total_data, string target) {
    int result = 0;
    for (int i = 0; i < total_data.length; i+=1) {
        if (total_data.single_input[i].worker_name == target) {
            return total_data.single_input[i].available_times;
        }
    }
    return total_data.single_input[0].available_times;
}

vector<pair<int,int> > intersection_time(vector<pair<int,int> > time1, vector<pair<int,int> > time2) {
    vector<pair<int,int> > result;
    int point1 = 0;
    int point2 = 0;
    while (point1 < time1.size() and point2 < time2.size()) {
        if (time2[point2].second <= time1[point1].first) {point2++;}
        else {
            if (time2[point2].second <= time1[point1].second and time2[point2].second > time1[point1].first) {
                result.emplace_back(make_pair(max(time1[point1].first, time2[point2].first), time2[point2].second));
                point2++;
            }
            else {
                if (time2[point2].first >= time1[point1].second) {point1++;}
                else{
                    result.emplace_back(make_pair(max(time1[point1].first, time2[point2].first), time1[point1].second));
                    point1++;
                }
            }
        }
    }
    return result;

};

vector<pair<int,int> > obtain_inter_from_names(server_inputs total_data, vector<string> name_list) {
    if (name_list.size() == 1) {
        return obtain_time_from_name(total_data, name_list[0]);
    }

    vector<pair<int,int> > prior = obtain_time_from_name(total_data, name_list[0]);
    for (int i = 1; i < name_list.size(); i += 1) {
        prior = intersection_time(prior, obtain_time_from_name(total_data, name_list[i]));
    }
    return prior;
};

string main_server_intersaction(vector<pair<int,int> > a_time, vector<pair<int,int> > b_time) {
    // if no intersection return "[]"
    string no_meaning = "[]";
    if (a_time.empty() and b_time.empty()) {
        string empty = "[]";
        return empty;
    }
    if (a_time.empty() and !b_time.empty()) {
        return vec_time_to_string(b_time);
    }
    if (!a_time.empty() and b_time.empty()) {
        return vec_time_to_string(a_time);
    }
    if (!a_time.empty() and b_time.empty()) {
        vector<pair<int,int> > result;
        result = intersection_time(a_time, b_time);
        return vec_time_to_string(a_time);
    }
    return no_meaning;
}

vector<string > valid_client_input(vector<string > vector_user_input, string invalid_users) {
    vector<string > invalid_vec = sting_names_to_string_list(invalid_users);
    vector<string > result;
    for (int i = 0; i < vector_user_input.size(); i++) {
        if (!if_string_in_vector(invalid_vec, vector_user_input[i])) {
            result.push_back(vector_user_input[i]);
        }
    }
    return result;
}

pair<int, int> process_user_time_decision(string ipt) {
    pair<int, int> result;
    result.first = -1;
    if (ipt.size() <= 2 or ipt[0] != '[' or ipt[ipt.size()-1] != ']') {
        return result;
    }
    ipt = ipt.substr(1, ipt.size()-2);

    int loc_douhao = -1;
    int num_douhao = 0;
    for (int i=0; i<ipt.size(); i+=1) {
        if (ipt[i] == ',') {
            loc_douhao = i;
            num_douhao += 1;
        }
    }

    if (num_douhao != 1) {
        return result;
    }
    result.first = stoi(ipt.substr(0, loc_douhao));
    result.second = stoi(ipt.substr(loc_douhao+1, ipt.size()-loc_douhao-1));
    return result;
}

int judge_user_time_decision(string time_interval, pair<int, int> user_decision) {
    vector<pair<int,int> > vec_time_interval = process_2d_string(time_interval);
    for (int i=0; i <vec_time_interval.size(); i++) {
        if (user_decision.first >= vec_time_interval[i].first and user_decision.second <= vec_time_interval[i].second) {
            return 1;
        }
    }
    return 0;
}









