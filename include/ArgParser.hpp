#include <cstring>
#include <string>
#include <vector>

#include <ArgumentParser.hpp>

struct Command {
    const char param1;
    const std::string& param2;
    const std::string& description;
};

class GetMethodsHandler {
protected:
    std::string Get(const std::string& param, int32_t index) {
        
        return "2";
    }
};

class AddMethodsHandler { //ADD in some container where will be --commands (may be of their types)
protected:
    Command Add(const char param1, const std::string& param2, const std::string& description = "") {
        return Command{param1, param2, description};
    }
    Command Add(const std::string& param2, const std::string& description = "") {
        return Command{.param2 = param2, .description = description};
    }
};

class ArgumentParser::ArgParser : private AddMethodsHandler, private GetMethodsHandler {
public:
    std::string name;
    ArgParser(std::string s) : name(s) {}
 
    bool Parse(std::vector<std::string> v); 
    bool Parse(int argc, char *argv[]);

    template <typename T>
    ArgParser& Default(T param);    /*default argument*/
     
    // template <typename T>
    // ArgParser& StoreValue(T value);

    // template <typename T>
    // ArgParser& StoreValues(std::vector<T> values);

    ArgParser& MultiValue(int32_t min_args_count = 0);  /*some values*/ 

    const std::string& GetStringValue(const std::string& param, int32_t index = 0);

    int32_t GetIntValue(const std::string& param, int32_t index = 0);

    bool GetFlag(const std::string& param);

    ArgParser& AddIntArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddIntArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddStringArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddStringArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddFlag(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddFlag(const std::string& param2, const std::string& description = "");

    ArgParser& AddHelp(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddHelp(const std::string& param2, const std::string& description = "");

    bool Help();
    // void HelpDescription();
    ArgParser& Positional();
};





