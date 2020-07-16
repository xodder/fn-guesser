#include <map>
#include <string>
#include <cstring>
#include <vector>

namespace xod
{
    enum value_t
    {
        BOOL,
        INT,
        FLOAT,
        STRING
    };

    struct value
    {
        xod::value_t type;
        union
        {
            bool bool_val;
            char string_val[255];
            int int_val;
            float float_val;
        };
    };

    typedef std::pair<std::string, xod::value> param;

    class argh
    {

    public:
        argh(int count, char** values);
        ~argh();

    public:
        bool has(const char* key);
        const char* get(const char* key);
        const bool get_bool(const char* key);
        const int get_int(const char* key);
        const float get_float(const char* key);
        template <typename item_t>
        const std::vector<item_t> get_list(const char* key);
        void parse();
        void dump();

    private:
        inline bool is_float(const std::string &value);
        inline bool is_numeric(const std::string &value);
        inline bool is_list(const std::string &value);
        inline bool is_alpha(const char value);
        inline bool is_param(const std::string &value);
        inline bool is_short_param(const std::string &value);
        inline bool is_long_param(const std::string &value);

    private:
        const xod::value bool_value();

    private:
        xod::value parse_numeric_value(int start_position);
        xod::value parse_string_value(int start_position);
        xod::value parse_value(int start_position);
        xod::param parse_short_param();
        xod::param parse_long_param();

    private:
        int _count;
        char **_values;
        int _current_value_index = 0;
        std::map<std::string, xod::value> _named_params;
        std::vector<xod::value> _unnamed_params;
    };
} // namespace xod


/*
    --xxxx-xxx bool|modifier
    -xx <value>
    --xxx=value arg_with_value
    --xxx <value> arg_with_value
    next_string()
    pos_pointer
    ['commit', '-v']
    * defines special params
    * params => tag alias type
    * types: bool | string | number -> int,double| list
    * 
    * patterns *
    * 
    * -x <value>? | -xyz -> -x -y -z
    * --xxx-xxx <value>? | --xxx--xx=<value> long
    * xxxxxx

    * { -x: value, --xxx-xx: value, }
    * [xxxxxx, xxxxx]
    * 
    * interface
    * args = parse_args(argc, argv) [x]
    * args.has(key) [x]
    * args.get_bool(key) [x]
    * args.get_list(key) [-]
    * args.get_int(key) [x]
    * args.get_double(key) [x]
    * args.get_string(key) [x]
    * args.next_int() [-]
    * args.next_string() [-]
    * args.next_list() [-]
*/