#include <windows.h>
#include <iostream>
#include "function_guesser/function_guesser.h"


int main(int argc, char **argv)
{
    SetConsoleOutputCP(CP_UTF8);
    std::string func = xod::guess_function({1, 2, 3, 4}, {19, 20, 23, 28});
    std::cout << func << std::endl;

    return 0;
}