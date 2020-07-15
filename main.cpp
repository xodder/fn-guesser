#include <windows.h>
#include <iostream>
#include "function_guesser/function_guesser.h"
#include "linear_algebra/linear_algebra.h"
#include <cmath>

void print_combinations(int n, int r)
{
    int i = 0;
    int combinations[r];

    for (i = 0; i < r; ++i)
        combinations[i] = i;

    //start from the last item
    i = r - 1;
    while (combinations[0] < n - r + 1)
    {
        std::cout << "[";
        for (int x : combinations)
            std::cout << x << ", ";
        std::cout << "]" << std::endl;

        // max value an element at i can have is (n - r + i)
        // so as long as the item at i is at its max value, move backwards i.e. decrement i
        while (i > 0 && combinations[i] == n - r + i)
            --i;

        // increment the current i
        combinations[i]++;

        // if its not the last item
        if (i < r - 1)
        {
            // the next item should be this item + 1
            combinations[i + 1] = combinations[i] + 1;
            i++; //move to next item
        }
    }
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(CP_UTF8);
    // print_combinations(5, 2);
    std::string func = xod::guess_function({1, 2, 3, 4}, {19, 20, 23, 28});
    std::cout << func << std::endl;

    return 0;
}