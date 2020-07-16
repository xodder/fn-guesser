#include <iostream>
#include "argh.h"

xod::argh::argh(int count, char **values)
{
    _count = count;
    _values = values;
    parse();
}

xod::argh::~argh()
{
    // remove vars
}

bool xod::argh::has(const char *key)
{
    return _named_params.count(key) == 1;
}

#define ARGH_GETTER_FUNC(name, return_type, value_type, value_getter, type_name, def_return) \
    const return_type xod::argh::name(const char *key)                                       \
    {                                                                                        \
        if (xod::argh::has(key))                                                             \
        {                                                                                    \
            xod::value value = _named_params[key];                                           \
            if (value.type == value_type)                                                    \
            {                                                                                \
                return _named_params[key].value_getter;                                      \
            }                                                                                \
            else                                                                             \
            {                                                                                \
                std::cerr << key << " is not a "                                             \
                          << type_name                                                       \
                          << " value" << std::endl;                                          \
            }                                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            std::cerr << key << " is not defined" << std::endl;                              \
        }                                                                                    \
        return def_return;                                                                   \
    }

ARGH_GETTER_FUNC(get, char *, xod::value_t::STRING, string_val, "string", "\0")
ARGH_GETTER_FUNC(get_bool, bool, xod::value_t::BOOL, bool_val, "bool", 0)
ARGH_GETTER_FUNC(get_int, int, xod::value_t::INT, int_val, "int", -1)
ARGH_GETTER_FUNC(get_float, float, xod::value_t::FLOAT, float_val, "float", -1)

#undef ARGH_VALUE_GETTER_FUNC

template <typename item_t>
const std::vector<item_t> xod::argh::get_list(const char *key)
{
    std::string string_val = _named_params[key].string_val;
    const int string_val_length = string_val.size();

    if (string_val[0] == '[' && string_val[string_val_length - 1] == ']') 
    {
        
    }
    else
    {
        // parse error
    }
    
}

inline bool xod::argh::is_float(const std::string &value)
{
    const int length = value.size();

    for (int i = 0; i < length; i++)
    {
        if (value[i] == '.')
            return true;
    }

    return false;
}

inline bool xod::argh::is_numeric(const std::string &value)
{
    const int length = value.size();
    for (int i = 0; i < length; i++)
    {
        if ((value[i] < '0' || value[i] > '9') && value[i] != '.')
            return false;
    }

    return true;
}

inline bool xod::argh::is_list(const std::string &value)
{
    return value[0] == '[';
}

inline bool xod::argh::is_alpha(const char value)
{
    return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z');
}

inline bool xod::argh::is_param(const std::string &value)
{
    return value[0] == '-';
}

inline bool xod::argh::is_short_param(const std::string &value)
{
    return value[0] == '-' && is_alpha(value[1]);
}

inline bool xod::argh::is_long_param(const std::string &value)
{
    return value[0] == '-' && value[1] == '-';
}

const xod::value xod::argh::bool_value()
{
    xod::value result;
    result.type = BOOL;
    result.bool_val = true;
    return result;
}

xod::value xod::argh::parse_numeric_value(int start_position)
{
    std::string current_value = &_values[_current_value_index][start_position];
    xod::value result;

    if (is_float(current_value))
    {
        char *end_ptr;
        result.type = FLOAT;
        result.float_val = strtof(current_value.c_str(), &end_ptr);
    }
    else
    {
        result.type = INT;
        result.int_val = 0;

        int position = 0;
        while (current_value[position] >= '0' && current_value[position] <= '9')
        {
            result.int_val = result.int_val * 10 + (current_value[position] - '0');
            position++;
        }
    }

    _current_value_index++;

    return result;
}

xod::value xod::argh::parse_string_value(int start_position)
{
    char string_val[255] = "";

    bool is_first = true;
    std::string str_val = "";
    std::string current_value = &_values[_current_value_index][start_position];

    while (_current_value_index < _count && !is_param(current_value))
    {
        if (!is_first)
        {
            str_val.append(" ");
            strcat(string_val, " ");
        }
        else
        {
            is_first = false;
        }

        strcat(string_val, current_value.c_str());
        str_val.append(current_value);

        // dont be tempted to remove the if statement and embed the increment,
        // it's a big bug
        _current_value_index++;
        if (_current_value_index <= _count - 1)
        {
            current_value = _values[_current_value_index];
        }
    }

    xod::value result;
    result.type = STRING;
    strcpy(result.string_val, string_val);
    return result;
}

xod::value xod::argh::parse_value(int start_position = 0)
{
    std::string current_value = &_values[_current_value_index][start_position];

    if (is_numeric(current_value))
    {
        return parse_numeric_value(start_position);
    }
    else
    {
        return parse_string_value(start_position);
    }
}

xod::param xod::argh::parse_short_param()
{
    std::string value = _values[_current_value_index];

    if (value.size() == 2)
    {
        xod::param result;
        result.first = value[1];
        _current_value_index++;

        // lookahead to see if there is a value
        if (_current_value_index < _count)
        {
            auto next_value = _values[_current_value_index];
            if (!is_param(next_value))
            {
                result.second = parse_value();
            }
            else
            {
                result.second = bool_value();
            }
        }
        else
        {
            result.second = bool_value();
        }

        return result;
    }
    else
    {
        _current_value_index++;
        //error for now, multiple options -xyz
        throw "multiple short params not supported yet.";
    }
}

xod::param xod::argh::parse_long_param()
{
    int position = 2;
    std::string current_value = _values[_current_value_index];
    std::string key = "";
    xod::param result;

    while (current_value[position] != '\0' && current_value[position] != '=')
    {
        key += current_value[position++];
    }
    result.first = key;

    if (current_value[position] == '=')
    {
        result.second = parse_value(position + 1);
    }
    else
    {
        _current_value_index++;
        // lookahead to see if there is a value
        if (_current_value_index < _count)
        {
            auto next_value = _values[_current_value_index];
            if (!is_param(next_value))
            {
                result.second = parse_value();
            }
            else
            {
                result.second = bool_value();
            }
        }
        else
        {
            result.second = bool_value();
        }
    }

    return result;
}

void xod::argh::parse()
{
    _current_value_index = 1;

    while (_current_value_index < _count)
    {
        std::string current_value = _values[_current_value_index];

        if (is_short_param(current_value))
        {
            _named_params.emplace(parse_short_param());
        }
        else if (is_long_param(current_value))
        {
            _named_params.emplace(parse_long_param());
        }
        else
        {
            _unnamed_params.push_back(parse_value());
        }
    }
}

static std::ostream &operator<<(std::ostream &dest, const xod::value &val)
{
    switch (val.type)
    {
    case xod::value_t::BOOL:
        dest << "[bool] " << val.bool_val;
        break;
    case xod::value_t::INT:
        dest << "[int] " << val.int_val;
        break;
    case xod::value_t::FLOAT:
        dest << "[float] " << val.float_val;
        break;
    default:
        dest << "[string] " << val.string_val;
        break;
    }
    return dest;
}

void xod::argh::dump()
{
    std::cout << "---named-params" << std::endl;
    for (xod::param p : _named_params)
    {
        std::cout << p.first << " = " << p.second << std::endl;
    }

    std::cout << "---unnamed-params" << std::endl;
    for (xod::value v : _unnamed_params)
    {
        std::cout << v << std::endl;
    }
}