#include <algorithm>
#include <cstring>
#include <variant>

namespace ArgumentParser {
enum class CommandType {
    INT_T,
    STRING_T,
    FLAG_T,
    HELP_T
};

using ref_type = std::optional<std::variant<
        std::reference_wrapper<int>,
        std::reference_wrapper<std::string>,
        std::reference_wrapper<bool>,
        std::reference_wrapper<std::vector<int64_t>>,
        std::reference_wrapper<std::vector<std::string>>,
        std::reference_wrapper<std::vector<bool>>>>;

 struct Command {
    CommandType type;
    char param1;
    std::string param2;
    std::string description;
    std::vector<std::string> args;
    int min_args_count = 0;
    bool multivalue = false;
    bool positional = false; 
    ref_type store_ref;
};

using CommandsDictionary = std::unordered_map<std::string, Command>;
using CommandsOrder = std::vector<Command>;

class CommandManager {
public:
    CommandsOrder commands_list;
    CommandsDictionary commands_dict;
    Command* current_cmd = nullptr;

    static constexpr int kIntMissingValue = -1;
    static constexpr const char* kStringMissingValue = "[MISSING VALUE]";

    bool IsCommand(const char* flag, const char* string);
    bool IsReservedCommand(const std::string& s);
    std::optional<std::string> GetFlagValue(const std::string& flag, int index = 0);
    void Add(CommandType type, const char param1, const std::string& param2, const std::string& description = "");
    void Add(CommandType type, const std::string& param2, const std::string& description = "");    
};
}

namespace ArgumentParser {
inline bool CommandManager::IsCommand(const char* flag, const char* string) {
    return (std::strncmp(flag, string, strlen(string)) == 0) ? true : false;
}
inline bool CommandManager::IsReservedCommand(const std::string& s) {
    if (commands_dict.find(s) != commands_dict.end())
        return true;
        
    if (s.size() == 2 && s[0] == '-') {
        for (const auto& [key, cmd] : commands_dict) {
            if (s[1] == cmd.param1)
                return true;
        }
    }
    return false;
}

inline std::optional<std::string> CommandManager::GetFlagValue(const std::string& flag, int index) {
    if (index < 0)
        return std::nullopt;
    for (const auto& [key, cmd] : commands_dict) {
        if ((flag == key || flag[0] == cmd.param1) && index < cmd.args.size())
            return cmd.args[index];
    }
    return std::nullopt;
}

inline void CommandManager::Add(
        CommandType type, 
        const char param1, 
        const std::string& param2, 
        const std::string& description) {
    if (!IsReservedCommand(std::string(1, param1)) && !IsReservedCommand(param2)) {
        commands_list.push_back({.type = type, .param1 = param1, .param2 = param2, .description = description});
        commands_dict[param2] = commands_list.back();
        current_cmd = &commands_dict[param2];
    }
}
inline void CommandManager::Add(
        CommandType type, 
        const std::string& param2, 
        const std::string& description) {
    if (!IsReservedCommand(param2)) {
        commands_list.push_back(Command{.type = type, .param2 = param2, .description = description});
        commands_dict[param2] = commands_list.back();
        current_cmd = &commands_dict[param2];
    }
}
}