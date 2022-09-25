#include "Json.h"
#include "cstring"
#include <sstream>
#include <stdexcept>
#include "Parser.h"
namespace myJson
{
Json::Json() : m_type(Json_null)
{
}

Json::Json(Type type) : m_type(type)
{
    switch (type)
    {
        case Json_null:
            break;
        case Json_bool:
            m_value.m_bool = false;
            break;
        case Json_int:
            m_value.m_int = 0;
            break;
        case Json_double:
            m_value.m_double = 0;
            break;
        case Json_string:
            m_value.m_string = new std::string("");
            break;
        case Json_array:
            m_value.m_array = new std::vector<Json>();
            break;
        case Json_object:
            m_value.m_object = new std::map<std::string, Json>();
            break;
        default:
            break;
    }
}

Json::Json(bool value) : m_type(Json_bool)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(Json_int)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(Json_double)
{
    m_value.m_double = value;
}

Json::Json(const char *value) : m_type(Json_string)
{
    m_value.m_string = new std::string(value);
}

Json::Json(const std::string &value) : m_type(Json_string)
{
    m_value.m_string = new std::string(value);
}

Json::Json(const Json &other) : m_type(Json_null)
{
    copy(other);
}

Json::Type Json::type() const
{
    return m_type;
}

bool Json::isNull() const
{
    return m_type == Json_null;
}

bool Json::isBool() const
{
    return m_type == Json_bool;
}

bool Json::isInt() const
{
    return m_type == Json_int;
}

bool Json::isDouble() const
{
    return m_type == Json_double;
}

bool Json::isString() const
{
    return m_type == Json_string;
}

bool Json::isArray() const
{
    return m_type == Json_array;
}

bool Json::isObject() const
{
    return m_type == Json_object;
}

bool Json::asBool() const
{
    if (m_type == Json_bool)
    {
        return m_value.m_bool;
    }
    throw std::logic_error("function Json::asBool value type error");
}

int Json::asInt() const
{
    if (m_type == Json_int)
    {
        return m_value.m_int;
    }
    throw std::logic_error("function Json::asInt value type error");
}

double Json::asDouble() const
{
    if (m_type == Json_double)
    {
        return m_value.m_double;
    }
    throw std::logic_error("function Json::asDouble value type error");
}

std::string Json::asString() const
{
    if (m_type == Json_string)
    {
        return *(m_value.m_string);
    }
    throw std::logic_error("function Json::asString value type error");
}

Json::~Json()
{
    clear();
}

void Json::copy(const Json &other)
{
    clear();
    m_type = other.m_type;
    switch (m_type)
    {
        case Json_null:
            break;
        case Json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case Json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case Json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case Json_string:
            m_value.m_string = new std::string(*(other.m_value.m_string));
            break;
        case Json_array:
            m_value.m_array = new std::vector<Json>(*(other.m_value.m_array));
            break;
        case Json_object:
            m_value.m_object = new std::map<std::string, Json>(*(other.m_value.m_object));
            break;
        default:
            break;
    }
}

int Json::size() const
{
    switch (m_type)
    {
        case Json_array:
            return m_value.m_array->size();
        case Json_object:
            return m_value.m_object->size();
        default:
            break;
    }
    throw std::logic_error("function Json::size value type error");
}

bool Json::empty() const
{
    switch (m_type)
    {
        case Json_null:
            return true;
        case Json_array:
            return m_value.m_array->empty();
        case Json_object:
            return m_value.m_object->empty();
        default:
            break;
    }
    return false;
}

void Json::clear()
{
    switch (m_type)
    {
        case Json_null:
        case Json_bool:
        case Json_int:
        case Json_double:
            break;
        case Json_string:
            if (m_value.m_string)
            {
                delete m_value.m_string;
                m_value.m_string = nullptr;
            }
            break;
        case Json_array:
            if (m_value.m_array)
            {
                for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); it++)
                {
                    it->clear();
                }
                delete m_value.m_array;
                m_value.m_array = nullptr;
            }
            break;
        case Json_object:
            if (m_value.m_object)
            {
                for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); it++)
                {
                    it->second.clear();
                }
                delete m_value.m_object;
                m_value.m_object = nullptr;
            }
            break;
        default:
            break;
    }
    m_type = Json_null;
}

bool Json::has(int index)
{
    if (m_type != Json_array)
    {
        return false;
    }

    return (index >= 0) && (index < m_value.m_array->size());
}

bool Json::has(const char *key)
{
    std::string str(key);
    return has(str);
}

bool Json::has(const std::string &key)
{
    if (m_type != Json_object)
    {
        return false;
    }

    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

void Json::remove(int index)
{
    if (m_type != Json_array)
    {
        return;
    }
    size_t size = m_value.m_array->size();
    if (index < 0 || index >= size)
    {
        return;
    }

    m_value.m_array->at(index).clear();
    m_value.m_array->erase(m_value.m_array->begin() + index);
}

void Json::remove(const char *key)
{
    std::string str(key);
    remove(str);
}

void Json::remove(const std::string &key)
{
    if (m_type != Json_object)
    {
        return;
    }

    auto it = m_value.m_object->find(key);
    if (it == m_value.m_object->end())
    {
        return;
    }
    else
    {
        it->second.clear();
        m_value.m_object->erase(key);
    }
}

void Json::append(const Json &value)
{
    if (m_type != Json_array)
    {
        clear();
        m_type = Json_array;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(value);
}

void Json::operator=(const Json &other)
{
    clear();
    copy(other);
}

bool Json::operator==(const Json &other) const
{
    if (m_type != other.m_type)
    {
        return false;
    }

    switch (m_type)
    {
        case Json_null:
            return true;
        case Json_int:
            return m_value.m_int == other.m_value.m_int;
        case Json_double:
            return m_value.m_double == other.m_value.m_double;
        case Json_string:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case Json_array:
            return *(m_value.m_array) == *(other.m_value.m_array);
        case Json_object:
            return *(m_value.m_object) == *(other.m_value.m_object);
        default:
            break;
    }
    return false;
}

bool Json::operator!=(const Json &other) const
{
    return !(*this == other);
}

bool Json::operator<(const Json &other) const
{
    if (m_type != other.m_type)
    {
        return false;
    }
    switch (m_type)
    {
        case Json_null:
            return false;
        case Json_int:
            return m_value.m_int < other.m_value.m_int;
        case Json_double:
            return m_value.m_double < other.m_value.m_double;
        case Json_bool:
            return m_value.m_bool < other.m_value.m_bool;
        case Json_string:
            {
                int res = m_value.m_string->compare(*other.m_value.m_string);
                if (res < 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        case Json_array:
            {
                auto thisSize = m_value.m_array->size();
                auto otherSize = other.m_value.m_array->size();
                if (thisSize != otherSize)
                    return thisSize < otherSize;
                return *(m_value.m_array) < *(other.m_value.m_array);
            }
        case Json_object:
            {
                auto thisSize = m_value.m_object->size();
                auto otherSize = other.m_value.m_object->size();
                if (thisSize != otherSize)
                    return thisSize < otherSize;
                return (*m_value.m_object) < (*other.m_value.m_object);
            }
        default:
            break;
    }
    return false;
}

bool Json::operator<=(const Json &other) const
{
    return !(other < *this);
}

bool Json::operator>=(const Json &other) const
{
    return !(*this < other);
}

bool Json::operator>(const Json &other) const
{
    return other < *this;
}

Json &Json::operator[](int index)
{
    if (m_type != Json_array)
    {
        clear();
        m_type = Json_array;
        m_value.m_array = new std::vector<Json>();
    }

    if (index < 0)
    {
        throw std::logic_error("function Json::operator [int] index less than 0");
    }

    int size = (m_value.m_array)->size();
    if (index >= 0 && index < size)
    {
        return m_value.m_array->at(index);
    }

    if (index >= size)
    {
        for (int i = size; i <= index; i++)
        {
            m_value.m_array->push_back(Json());
        }
    }

    return m_value.m_array->at(index);
}

Json &Json::operator[](const char *key)
{
    std::string name = key;
    return (*this)[name];
}

Json &Json::operator[](const std::string &key)
{
    if (m_type != Json_object)
    {
        clear();
        m_type = Json_object;
        m_value.m_object = new std::map<std::string, Json>();
    }

    return (*m_value.m_object)[key];
}

Json::operator bool()
{
    if (m_type != Json_bool)
    {
        throw std::logic_error("function Json::operator (bool) requires bool value");
    }
    return m_value.m_bool;
}

Json::operator int()
{
    if (m_type != Json_int)
    {
        throw std::logic_error("function Json::operator (int) requires int value");
    }
    return m_value.m_int;
}

Json::operator double()
{
    if (m_type != Json_double)
    {
        throw std::logic_error("function Json::operator (double) requires double value");
    }
    return m_value.m_double;
}

Json::operator std::string()
{
    if (m_type != Json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }
    return *(m_value.m_string);
}

void Json::parse(const std::string &str)
{
    Parser parser;
    parser.load(str);
    *this = parser.parse();
}

std::string Json::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
        case Json_null:
            ss << "null";
            break;
        case Json_bool:
            if (m_value.m_bool)
            {
                ss << "true";
            }
            else
            {
                ss << "false";
            }
            break;
        case Json_int:
            ss << m_value.m_int;
            break;
        case Json_double:
            ss << m_value.m_double;
            break;
        case Json_string:
            ss << "\"" << *m_value.m_string << "\"";
            break;
        case Json_array:
            {
                ss << "[";
                for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); it++)
                {
                    if (it != m_value.m_array->begin())
                    {
                        ss << ", ";
                    }
                    ss << it->str();
                }
                ss << "]";
            }
            break;
        case Json_object:
            {
                ss << "{";
                for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); it++)
                {
                    if (it != m_value.m_object->begin())
                    {
                        ss << ", ";
                    }
                    ss << "\"" << it->first << "\" : " << it->second.str();
                }
                ss << "}";
            }
            break;
        default:
            break;
    }
    return ss.str();
}

} // namespace myJson