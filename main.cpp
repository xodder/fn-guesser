#include <windows.h>
#include <iostream>
#include <cmath>
#include "argh/argh.h"
#include "function_guesser/function_guesser.h"

template <typename elem_t>
static std::ostream &operator<<(std::ostream &out, std::vector<elem_t> v)
{
    out << "[";
    for (int i = 0; i < v.size(); ++i)
    {
        out << v[i] << ((i + 1) % v.size() ? "," : "");
    }
    out << "]";
    return out;
}

std::vector<double> parse_list(const char *str)
{
    int length = strlen(str);
    int start_pos = 0, end_pos = 0;
    std::vector<double> result;

    while (end_pos <= length)
    {
        while (!isdigit(str[start_pos]) && str[start_pos] != '.')
            end_pos = ++start_pos;

        if (!isdigit(str[end_pos]) && str[end_pos] != '.')
        {
            const char *begin = str + start_pos;
            char *end = (char *)str + end_pos;
            double value = strtod(begin, &end);
            result.push_back(value);
            start_pos = ++end_pos;
        }

        ++end_pos;
    }

    return result;
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(CP_UTF8);

    xod::argh args(argc, argv);

    if (args.has("i") && args.has("o"))
    {
        std::vector<double> inputs = parse_list(args.get("i"));
        std::vector<double> outputs = parse_list(args.get("o"));
        std::string func = xod::guess_function(inputs, outputs);
        std::cout << func << std::endl;
    }
    else
    {
        if (!args.has("i")) 
        {
            std::cout << "inputs list is missing" << std::endl;
            std::cout << "   define with " << "-i [x1,x2,...,xn]" << std::endl;
        }
        if (!args.has("o"))
        {
            std::cout << "outputs list is missing" << std::endl;
            std::cout << "   define with " << "-o [y1,y2,...,yn]" << std::endl;
        }
    }

    return 0;
}