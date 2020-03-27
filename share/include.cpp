#include "include.hpp"

void assert_with_out(const bool true_condition, const std::string& comment) {
    if (!true_condition) {
        std::cout << comment << std::endl;
        assert(false);
    }
}

bool str_starts_with(const std::string& str, const std::string& pre) {
    return str.size() >= pre.size() && std::equal(std::begin(pre), std::end(pre), std::begin(str));
}

std::vector<std::string> str_split(const std::string& str, const char del) {
    int first = 0;
    int last = str.find_first_of(del);
    std::vector<std::string> result;
    while (first < str.size()) {
        std::string subStr(str, first, last - first); 
        result.push_back(subStr);
        first = last + 1;
        last = str.find_first_of(del, first);
        if (last == std::string::npos) {
            last = str.size();
        }
    }
    return result;
}

bool check_openable_file(const std::string& file_name) {
    std::ifstream ifs(file_name);
    return ifs.is_open();
}

json11::Json load_json_from_file(const std::string& file_name) {
    std::ifstream ifs(file_name);
    if (ifs.fail()) {
        std::cerr << "error:load_json_from_file " + file_name << std::endl;
        assert(false);
    }
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    std::string err;
    json11::Json ret_json = json11::Json::parse(str, err);
    // err に関するassert
    ifs.close();
    return ret_json;
}

std::vector<json11::Json> load_json_vec_from_file(const std::string& file_name) {
    std::vector<json11::Json> json_vec;
    std::ifstream ifs(file_name);
    std::string str;
    if (ifs.fail()) {
        std::cerr << "error:load_json_vec_from_file " + file_name << std::endl;
        assert(false);
    }
    while (getline(ifs, str)) {
        std::string err;
        json11::Json json_elem = json11::Json::parse(str, err);
        json_vec.push_back(json_elem);
    }
    ifs.close();
    return json_vec;
}

void dump_json_to_file(const json11::Json& json, const std::string& file_name) {
    std::ofstream outputfile(file_name);
    outputfile << json.dump() << std::endl;
    outputfile.close();
}

void dump_json_vec_to_file(const std::vector<json11::Json>& json_vec, const std::string& file_name) {
	std::ofstream outputfile(file_name);
	for (int i = 0; i < json_vec.size(); i++) {
    	outputfile << json_vec[i].dump() << std::endl;
	}
    outputfile.close();
}

std::vector<std::string> get_files_path(const std::string& dir_name) {
    std::string ls_command = "ls -1 ./" + dir_name + "/* > tmp.txt"; // 指定したディレクトリ内のmjsonファイルを一旦tmp.txtに書き出す。
    int system_result = system(ls_command.c_str());
    assert_with_out(system_result != -1, "get_files_path error: system_result == -1, command:" + ls_command);
    std::ifstream ifs_tmptxt("tmp.txt");
    assert_with_out(!ifs_tmptxt.fail(), "tmp.txt not generated");
    std::string file_path;
    std::vector<std::string> ret;
    while (getline(ifs_tmptxt, file_path)) {
        ret.push_back(file_path);
    }
    ifs_tmptxt.close();
    ls_command = "rm ./tmp.txt";
    system_result = system(ls_command.c_str());
    assert_with_out(system_result != -1, "get_files_path error: system_result == -1, command:" + ls_command);
    return ret;
}

#ifdef WINSTD
void make_dir(const std::string& dir_name) {
    mkdir(dir_name.c_str());
}
#else
void make_dir(const std::string& dir_name) {
    mkdir(dir_name.c_str(), 0755);
}
#endif