#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <expected>
#include <iostream>
#include <string>
#include <variant>

namespace cli_parser {
    typedef struct {
        std::string name;
        std::variant<bool, int, float, std::string> value;
    } ArgInfo;

    class Parser {
    public: // public interface
        Parser(int argc, char** argv, bool enable_logging);
        ~Parser() = default;
        std::expected<ArgInfo, std::string> get_positional_arg(const std::string &arg_name, int position, bool required);
        std::expected<ArgInfo, std::string> get_flag_arg(const std::string &flag_name, bool has_value, const std::string &value_hint);
        [[nodiscard]] bool has_failed_args() const { return m_failed_to_parse; }
    private: // private functions
        static bool is_int(const std::string& s);
        static bool is_float(const std::string& s);
    private: // member vars
        int m_argc;
        char** m_argv;
        bool m_is_logging;
        bool m_failed_to_parse = false;
    };

    inline Parser::Parser(const int argc, char **argv, bool enable_logging = true) : m_argc(argc), m_argv(argv), m_is_logging(enable_logging) {}

    // returns a positional argument after the executable arg in argv
    inline std::expected<ArgInfo, std::string> Parser::get_positional_arg(const std::string &arg_name, int position, bool required = false) {
        if (position  <= 0 || m_argc < position) {
            m_failed_to_parse = true;
            return std::unexpected(std::format("position must be greater than 0 (0 is the program arg) and less than the count of args passed in - position = {}", position));
        }

        const std::string arg = m_argv[position];
        if (arg.starts_with("-") && required) {
            m_failed_to_parse = true;
            return std::unexpected(std::format("positional argument \"{}\" is required at position {}", arg_name, position));
        }

        // check its type
        std::cout << std::format("positional argument found \"{}\" -> {}\n",arg_name, arg);
        if (is_int(arg)) {
            return ArgInfo{
                .name = arg_name,
                .value = std::stoi(arg)
            };
        }
        if (is_float(arg)) {
            return ArgInfo{
                .name = arg_name,
                .value = std::stof(arg)
            };
        }
        return ArgInfo{
            .name = arg_name,
            .value = arg
        };
    }

    inline std::expected<ArgInfo, std::string> Parser::get_flag_arg(const std::string &flag_name, bool has_value = false, const std::string& value_hint = "") {
        for (int i = 0; i < m_argc; i++) {
            std::string arg = m_argv[i];
            if (arg != flag_name)
                continue;

            if (!has_value) {
                return ArgInfo{
                    .name = flag_name,
                    .value = true
                };
            }

            // only if it has a value following the flag
            if (i < m_argc) {
                std::string value = m_argv[i + 1];
                if (value.starts_with("-")) {
                    m_failed_to_parse = true;
                    return std::unexpected(std::format("flag argument \"{}\" -> hint: {}", flag_name, value_hint));
                }

                std::cout << std::format("flag argument found \"{}\" -> {}\n",arg, value);
                if (is_int(arg)) {
                    return ArgInfo{
                        .name = flag_name,
                        .value = std::stoi(value)
                    };
                }
                if (is_float(arg)) {
                    return ArgInfo{
                        .name = flag_name,
                        .value = std::stof(value)
                    };
                }
                return ArgInfo{
                    .name = flag_name,
                    .value = value
                };
            }
        }

        m_failed_to_parse = true;
        return std::unexpected(std::format("flag argument \"{}\" was not found", flag_name));
    }

    inline bool Parser::is_int(const std::string &s) {
        if (s.empty()) return false;
        size_t idx;
        try {
            (void)std::stoi(s, &idx);
        } catch (...) {
            return false; // Not a valid integer format
        }
        return idx == s.length(); // Entire string must be consumed
    }

    inline bool Parser::is_float(const std::string &s) {
        if (s.empty()) return false;
        size_t idx;
        try {
            (void)std::stof(s, &idx);
        } catch (...) {
            return false; // Not a valid floating-point format
        }
        return idx == s.length(); // Entire string must be consumed
    }
}

#endif //CLI_PARSER_H
