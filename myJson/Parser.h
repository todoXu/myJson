#pragma once

#include "Json.h"
#include <string>
namespace myJson
{
class Parser
{
public:
    Parser();
    ~Parser();

    void load(const std::string &str);
    Json parse();

private:
    void skip_white_space();
    char get_next_token();
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    std::string parse_string();
    Json parse_array();
    Json parse_object();

    bool in_range(int x, int lower, int upper)
    {
        return (x >= lower) && (x <= upper);
    }

private:
    std::string m_str;
    size_t m_idx;
};
} // namespace myJson