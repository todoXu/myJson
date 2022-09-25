#include "Parser.h"
#include "Json.h"
#include <stdexcept>
#include <string>
namespace myJson
{
Parser::Parser() : m_idx(0)
{
}

Parser::~Parser()
{
}

void Parser::load(const std::string &str)
{
    m_str = str;
    m_idx = 0;
}

Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':
            m_idx--;
            return parse_null();
        case 't':
        case 'f':
            m_idx--;
            return parse_bool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_idx--;
            return parse_number();
        case '"':
            return Json(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }
    throw std::logic_error("unexpected character in parse json");
}

void Parser::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
    {
        m_idx++;
    }
}

char Parser::get_next_token()
{
    skip_white_space();
    if (m_idx == m_str.size())
    {
        throw std::logic_error("unexpected end of input");
    }
    char res = m_str[m_idx];
    m_idx++;
    return res;
}

Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    size_t pos = m_idx;

    //负数的情况
    if (m_str[m_idx] == '-')
    {
        m_idx++;
    }

    //数字有2个部分，整数和小数 比如123.321  0.123  先把整数部分拿出来 比如123  再考虑小数
    if (m_str[m_idx] == '0')
    {
        m_idx++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }

    //说明没有小数，直接转数字
    if (m_str[m_idx] != '.')
    {
        printf("数字 %d\n", std::atoi(m_str.c_str() + pos));
        return Json(std::atoi(m_str.c_str() + pos));
    }

    //现在是小数部分 0.321
    m_idx++;
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("at least one digit required in fractional part");
    }
    while (in_range(m_str[m_idx], '0', '9'))
    {
        m_idx++;
    }
    return Json(std::atof(m_str.c_str() + pos));
}

std::string Parser::parse_string()
{
    std::string out;
    while (true)
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx];
        m_idx++;
        if (ch == '"')
        {
            break;
        }

        //有转义符 例如\r \n \t \"
        if (ch == '\\')
        {
            ch = m_str[m_idx];
            m_idx++;
            switch (ch)
            {
                case 'b':
                    out += '\b';
                    break;
                case 't':
                    out += '\t';
                    break;
                case 'n':
                    out += '\n';
                    break;
                case 'f':
                    out += '\f';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case '"':
                    out += "\\\"";
                case '\\':
                    out += "\\\\";
                    break;
                case 'u':
                    out += "\\u";
                    for (int i = 0; i < 4; i++)
                    {
                        out += m_str[m_idx];
                        m_idx++;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            out += ch;
        }
    }
    return out;
}

Json Parser::parse_array()
{
    Json arr(Json::Json_array);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx--;
    while (true)
    {
        arr.append(parse());
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in array");
        }
        m_idx++;
    }

    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::Json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
    m_idx--;
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
        }
        std::string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}

} // namespace myJson
