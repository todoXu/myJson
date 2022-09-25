#pragma once
#include <map>
#include <string>
#include <vector>
namespace myJson
{
class Json
{
public:
    enum Type
    {
        Json_null = 0,
        Json_bool,
        Json_int,
        Json_double,
        Json_string,
        Json_array,
        Json_object
    };
    Json();
    Json(Type type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char *value);
    Json(const std::string &value);
    Json(const Json &other);
    ~Json();

    Type type() const;

    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    int size() const;
    bool empty() const;

    void clear();

    bool has(int index);
    bool has(const char *key);
    bool has(const std::string &key);
    void remove(int index);
    void remove(const char *key);
    void remove(const std::string &key);
    void append(const Json &value);

    void operator=(const Json &other);

    bool operator==(const Json &other) const;
    bool operator!=(const Json &other) const;
    bool operator<(const Json &other) const;
    bool operator<=(const Json &other) const;
    bool operator>=(const Json &other) const;
    bool operator>(const Json &other) const;

    Json &operator[](int index);
    Json &operator[](const char *key);
    Json &operator[](const std::string &key);

    operator bool();
    operator int();
    operator double();
    operator std::string();

    //这个其实就是解析器，专门负责把字符串解析成json
    void parse(const std::string &str);

    std::string str() const;

    typedef std::vector<Json>::iterator Json_iterator;
    Json_iterator begin()
    {
        if (m_type == Json_array && m_value.m_array)
        {
            return (m_value.m_array)->begin();
        }
        else
        {
            return {};
        }
    }
    Json_iterator end()
    {
        if (m_type == Json_array && m_value.m_array)
        {
            return (m_value.m_array)->end();
        }
        else
        {
            return {};
        }
    }

private:
    void copy(const Json &other);

private:
    // union占用内存小 而且value在同一时刻只能是一种类型，不可能又是int 又是 double，所以可以用union
    union Value {
        bool m_bool;
        int m_int;
        double m_double;
        // union里面是不允许直接用string vector map类型的，所以用指针替代
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<std::string, Json> *m_object;
    };
    Type m_type;
    Value m_value;
};

} // namespace myJson
