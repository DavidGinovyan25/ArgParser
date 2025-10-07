//#include <ArgParser.hpp>

/*REMAKE SRUCTURE OF COMMANDS, SPLIT_STRING, CHECHCOMMAND*/
namespace ArgumentParser {
    template <typename T>
    T value;

    template <typename T>
    std::vector<T> values;

    // std::vector<Command> commands; 
    // std::vector<std::string> split_string;

    //using ::ArgParser;
    class ArgParser;

    // bool CheckCommand(const char param1, const std::string& param2) {
    //     for (size_t i = 0; i < commands.size(); ++i) {
    //         if (param1 == commands[i].param1 && param2 == commands[i].param2)
    //             return true;
    //     }
    //     return false;
    // }
    // bool CheckCommand(const std::string& param2) {
    //     for (size_t i = 0; i < commands.size(); ++i) {
    //         if (param2 == commands[i].param2)
    //             return true;
    //     }
    //     return false;
    // }
}