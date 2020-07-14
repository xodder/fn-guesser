#include <iostream>
#include "function_guesser/function_guesser.h"
#include "linear_algebra/linear_algebra.h"
#include <cmath>

int main(int argc, char **argv)
{
    xod::guess_function({1, 2, 3, 4}, {5, 10, 17, 26});

    return 0;
}