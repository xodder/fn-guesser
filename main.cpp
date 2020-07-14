#include <iostream>
#include "function_guesser/function_guesser.h"
#include <cmath>

// entries should be an augumented matrix
template <typename number_t>
int guassian_elimination(number_t *entries, int row_count, int col_count)
{
    int row_swap_count = 0;
    for (int row = 0; row < row_count; row++)
    {
        int expected_pivot_column = row;
        number_t pivot = entries[row * col_count + expected_pivot_column];
        int actual_pivot_column = expected_pivot_column;
        while (!(pivot = entries[row * col_count + actual_pivot_column]))
            actual_pivot_column++;

        // check for swapping instances
        int row_to_swap_with = row;

        for (int i = row + 1; i < row_count; ++i)
        {
            for (int j = expected_pivot_column; j <= actual_pivot_column; ++j)
            {
                number_t rival_entry = entries[i * col_count + j];
                if (pivot < rival_entry)
                {
                    row_to_swap_with = i;
                    pivot = rival_entry;
                    actual_pivot_column = j;
                    break;
                }
            }
        }

        if (pivot == 0)
        {
            // throw error
        }

        if (row != row_to_swap_with)
        {
            // do swapping
            row_swap_count++;
            for (int j = 0; j < col_count; ++j)
            {
                number_t tmp = entries[row * col_count + j];
                entries[row * col_count + j] = entries[row_to_swap_with * col_count + j];
                entries[row_to_swap_with * col_count + j] = tmp;
            }
        }

        // eliminate pivot term in the lower rows
        // Given {a,b,c,m;d,e,f,n;g,h,i,o}
        // r2 -> r2 - (d/a) * r1
        for (int i = row + 1; i < row_count; ++i)
        {
            number_t rival_pivot = entries[i * col_count + actual_pivot_column];
            double factor = rival_pivot / (double)pivot;
            for (int j = actual_pivot_column; j < col_count; ++j)
            {
                entries[i * col_count + j] -= factor * entries[row * col_count + j];
            }
        }
    }

    return row_swap_count;
}

// entries should be an augumented matrix
template <typename number_t>
void guass_jordan_elimination(number_t *entries, int row_count, int col_count)
{
    guassian_elimination<number_t>(entries, row_count, col_count);

    // do backward reduction starting from the down to top
    for (int row = row_count - 1; row >= 0; --row)
    {
        int pivot_column = row;
        number_t pivot = entries[row * col_count + pivot_column];

        if (pivot == 0)
        {
            // throw error
        }

        // eliminate pivot term in the upper rows
        for (int i = row - 1; i >= 0; --i)
        {
            number_t rival_pivot = entries[i * col_count + pivot_column];
            double factor = rival_pivot / (double)pivot;
            for (int j = 0; j < col_count; ++j)
            {
                entries[i * col_count + j] -= factor * entries[row * col_count + j];
            }
        }

        // divide row by pivot
        double divisor = 1 / (double)pivot;
        for (int j = 0; j < col_count; ++j)
        {
            entries[row * col_count + j] *= divisor;
        }
    }
}

// entries should be a square matrix
double determinant_with_guassian_elimination(double *entries, int size)
{
    int swap_count = guassian_elimination<double>(entries, size, size);
    double determinant = std::pow(-1, swap_count);
    for (int i = 0; i < size; i++)
    {
        determinant *= entries[i * size + i];
    }
    return determinant;
}

int main(int argc, char **argv)
{
    // xod::guess_function({1, 2, 3}, {2, 3, 4});
    float entries[] = {2, 1, -1, 8, -3, -1, 2, -11, -2, 1, 2, -3};
    guass_jordan_elimination<float>(entries, 3, 4);
    for (int i = 0; i < 3; ++i)
    {
        std::cout << "[";
        for (int j = 0; j < 4; ++j)
        {
            std::cout << entries[i * 4 + j] << ((j + 1) % 4 ? "," : "");
        }
        std::cout << "]" << std::endl;
    }

    double values[] = {2, 1, -1, -3, -1, 2, -2, 1, 2};
    double det = determinant_with_guassian_elimination(values, 3);
    std::cout << "det = " << det << std::endl;

    return 0;
}