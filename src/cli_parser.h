#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <string>

namespace cli_parser {
    class Parser {
    public: // public interface
        Parser(int argc, char** argv);
        ~Parser();
        void try_parse_value(std::string value_name);
        void try_get_value(std::string value_name);
    private: // private functions
    private: // member vars

    };

    inline Parser::Parser(int argc, char **argv) {

    }

    inline Parser::~Parser() {
    }

    inline void Parser::try_parse_value(std::string value_name) {
    }

    inline void Parser::try_get_value(std::string value_name) {
    }
}

#endif //CLI_PARSER_H
