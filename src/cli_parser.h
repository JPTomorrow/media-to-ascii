#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace cli_parser {
    typedef struct {
        std::string name;
        std::variant<bool, int, float> value;
    } ArgInfo;

    class Parser {
    public: // public interface
        Parser(int argc, char** argv);
        ~Parser() = default;
        void register_positional_arg(const std::string& value_name);
        void register_flag_arg(const std::string& value_name);
        void get(const std::string& value_name);
    private: // private functions
    private: // member vars
        std::vector<ArgInfo> m_args;
        int m_argc;
        char** m_argv;
    };

    inline Parser::Parser(int argc, char **argv) : m_argc(argc), m_argv(argv) {}

    inline void Parser::register_flag_arg(const std::string& value_name) {
        for (int i = 0; i < m_argc; i++) {
            std::string arg = m_argv[i];
            if (arg == value_name) {

            }
            // std::cout << m_argv[i] << "\n";
        }
    }

    inline void Parser::get(const std::string& value_name) {
    }
}

#endif //CLI_PARSER_H
