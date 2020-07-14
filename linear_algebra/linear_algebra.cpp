#include <cmath>
#include "linear_algebra.h"

namespace xod
{
    int guassian_elimination(double *matrix, int row_count, int col_count)
    {
        int row_swap_count = 0;
        for (int row = 0; row < row_count; row++)
        {
            const int expected_pivot_col = row;
            double pivot = matrix[row * col_count + expected_pivot_col];
            int actual_pivot_col = expected_pivot_col;
            while (!(pivot = matrix[row * col_count + actual_pivot_col]))
                actual_pivot_col++;

            // check for swapping instances
            int row_to_swap_with = row;

            for (int i = row + 1; i < row_count; ++i)
            {
                for (int j = expected_pivot_col; j <= actual_pivot_col; ++j)
                {
                    const double rival_entry = matrix[i * col_count + j];
                    if (pivot < rival_entry)
                    {
                        row_to_swap_with = i;
                        pivot = rival_entry;
                        actual_pivot_col = j;
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
                    const double tmp = matrix[row * col_count + j];
                    matrix[row * col_count + j] = matrix[row_to_swap_with * col_count + j];
                    matrix[row_to_swap_with * col_count + j] = tmp;
                }
            }

            // eliminate pivot term in the lower rows
            // Given {a,b,c,m;d,e,f,n;g,h,i,o}
            // r2 -> r2 - (d/a) * r1
            for (int i = row + 1; i < row_count; ++i)
            {
                const double rival_pivot = matrix[i * col_count + actual_pivot_col];
                const double factor = rival_pivot / (double)pivot;
                for (int j = actual_pivot_col; j < col_count; ++j)
                {
                    matrix[i * col_count + j] -= factor * matrix[row * col_count + j];
                }
            }
        }

        return row_swap_count;
    }

    std::vector<double> guass_jordan_elimination(double *matrix, int row_count, int col_count)
    {
        guassian_elimination(matrix, row_count, col_count);

        // do backward reduction starting from the down to top
        for (int row = row_count - 1; row >= 0; --row)
        {
            int pivot_column = row;
            double pivot = matrix[row * col_count + pivot_column];

            if (pivot == 0)
            {
                // throw error
            }

            // eliminate pivot term in the upper rows
            for (int i = row - 1; i >= 0; --i)
            {
                double rival_pivot = matrix[i * col_count + pivot_column];
                double factor = rival_pivot / (double)pivot;
                for (int j = 0; j < col_count; ++j)
                {
                    matrix[i * col_count + j] -= factor * matrix[row * col_count + j];
                }
            }

            // divide row by pivot
            double divisor = 1 / (double)pivot;
            for (int j = 0; j < col_count; ++j)
            {
                matrix[row * col_count + j] *= divisor;
            }
        }

        std::vector<double> solutions;

        for (int i = 0; i < row_count; i++)
        {
            solutions.push_back(matrix[i * col_count + (col_count - 1)]);
        }

        return solutions;
    }

    double determinant(double *matrix, int size)
    {
        int swap_count = guassian_elimination(matrix, size, size);
        double determinant = std::pow(-1, swap_count);
        for (int i = 0; i < size; i++)
        {
            determinant *= matrix[i * size + i];
        }
        return determinant;
    }
} // namespace xod
