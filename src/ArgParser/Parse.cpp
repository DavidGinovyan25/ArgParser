#include <cstring>
#include <ArgParser.hpp>

namespace ArgumentParser {

auto print = [](const std::string& text) {std::cout << text << std::endl;};

bool ArgParser::Parse(std::vector<std::string> v) {  
    auto error_handling = [&](bool error_condition, const char* error_type) {
        if (error_condition) {
            std::cout << error_type << std::endl;
            return true;
        }    
        return false;};

    auto is_not_command = [&](int i) {
        return !IsCommand(split_string[i].c_str(), "--") && !IsCommand(split_string[i].c_str(), "-");};

    auto tokenize = [&](int start, int finish, int i, std::string sym = "") {
        split_string.insert(split_string.begin() + i + 1, sym + split_string[i].substr(start));
        split_string[i] = split_string[i].substr(0, finish);};

    if (error_handling(commands_dict.empty() || commands_list.empty(), "Empty commands_list"))
        return true;    

    split_string.clear();
    if (split_string.empty())
        split_string = v;        
    program_name = split_string[0];
    split_string.erase(split_string.begin());
    

    for (int i = 0; i < split_string.size(); ++i) {
        if (is_not_command(i)) 
            continue;
        size_t index_equal_sign = split_string[i].find('=');
        if (index_equal_sign != std::string::npos) {
            tokenize(index_equal_sign + 1, index_equal_sign, i);
            continue;
        }
        if (IsCommand(split_string[i].c_str(), "--"))
            continue;
        while (split_string[i].size() > 2) {
            tokenize(split_string[i].size() - 1, split_string[i].size() - 1, i, "-");  
        }
    }
    for (auto cmd = commands_list.begin(); cmd != commands_list.end(); ++cmd) {
        bool finded_command = false;
        for (auto s = split_string.begin(); s != split_string.end(); ++s) {
            if ((s->size() > 2 && s->substr(2) == cmd->param2) || (s->size() > 1 && (*s)[1] == cmd->param1)) {
                finded_command = true;
                break;
            }
        }
        if (error_handling(split_string.size() == 0 && !finded_command && cmd->args.empty(), "ERROR"))
            return false;
    }
    for (std::string s : split_string) {
        print(s);
    }
    auto is_arg = [&](int& i){return i < split_string.size() - 1 && is_not_command(i + 1);};
    auto add_arg = [&](int& i, auto& it){if (is_arg(i)) it->second.args.push_back(split_string[++i]);};
    auto add_args = [&](int& i, auto& it){while(is_arg(i)) {add_arg(i, it);}};
    auto check_default = [&](int i, auto it){if(!it->second.args.empty()) it->second.args.erase(it->second.args.begin());};

    auto vec_pos_com = commands_list.begin();
    for (int i = 0; i < split_string.size(); ++i) {
        if (is_not_command(i)) {
            vec_pos_com = std::find_if(vec_pos_com, commands_list.end(), 
                [&](const Command& cmd){return cmd.positional;});

            if (error_handling(vec_pos_com == commands_list.end(), "extra arguments")) {
                return false;
            }

            auto map_pos_com = commands_dict.find(vec_pos_com->param2);
            ++vec_pos_com;
            add_args(i, map_pos_com);
            map_pos_com->second.args.insert(map_pos_com->second.args.begin(), split_string[0]);
            continue;
        }
        auto it_long_command = commands_dict.find(split_string[i].substr(2));
        auto it_short_command = std::find_if(commands_dict.begin(), commands_dict.end(), 
            [&](const auto& pair){return pair.second.param1 == split_string[i][1];});

        auto it_map = (it_short_command != commands_dict.end())
            ? it_short_command 
            : (it_long_command != commands_dict.end()) ? it_long_command : commands_dict.end();

        if (error_handling(it_map == commands_dict.end(), "unreserved command")) 
            return false;
                
        if (it_map->second.multivalue) {
            add_args(i, it_map);
            if (error_handling(it_map->second.args.size() < it_map->second.min_args_count, "min args doesnt have"))
                return false;
            continue;
        }
        check_default(i, it_map);
        add_arg(i, it_map);
        if (is_arg(i)) {
            vec_pos_com = std::find_if(vec_pos_com, commands_list.end(), 
                [&](const Command& cmd){return cmd.positional;});

            if (error_handling(vec_pos_com == commands_list.end(), "extra arguments"))
                return false; 

            auto map_pos_com = commands_dict.find(vec_pos_com->param2);
            ++vec_pos_com;
            add_args(i, map_pos_com);
            if (error_handling(
                    map_pos_com->second.args.size() < map_pos_com->second.min_args_count, "min args doesnt have"))
                return false;

            if (error_handling(!map_pos_com->second.multivalue || it_map->second.args.size() > 1, "extra argunemts"))
                return false;
        }
    }
    for (auto& cmd : commands_list) {
        cmd.args = commands_dict[cmd.param2].args;
    }    
    for (auto& cmd : commands_list) {
        if (cmd.store_ref.has_value()) {
            std::visit([&](auto&& stored_ref) {storage_handler(stored_ref, cmd.args);}, cmd.store_ref.value());
        }
    }
// for (auto& [key, cmd] : commands_dict) {
//     if (cmd.args.empty()) {
//         std::cout << key << " " << cmd.param1 << std::endl;
//     } else {
//         for (size_t i = 0; i < cmd.args.size(); ++i) {
//             std::cout << key << " " << cmd.param1 << " " << cmd.args[i] << std::endl;
//         }
//     }
// }
    return true;
}
bool ArgParser::Parse(int argc, char *argv[]) {
    split_string.clear();
    for (int i = 0; i < argc; ++i) {
        split_string.push_back(argv[i]);
    }
    return Parse(split_string);
}
}