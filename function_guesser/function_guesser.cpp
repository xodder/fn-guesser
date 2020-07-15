#include <iostream>
#include <cmath>
#include "function_guesser.h"
#include "../linear_algebra/linear_algebra.h"

namespace xod
{
    std::string guess_function(std::vector<int> inputs, std::vector<int> outputs)
    {
        const int input_size = inputs.size();
        const double max_error = 1e-10;
        double smallest_error = INT_MAX;
        std::vector<double> best_coefficients;

        for (int sample_size = 2; sample_size <= input_size; sample_size++)
        {
            const int row_count = sample_size;
            const int col_count = sample_size + 1;
            double matrix[row_count * col_count];
            double sample_indices[sample_size]; // combinations of inputs to use
            const int max_sample_index = input_size - sample_size + 1;
            int pos = 0;

            // using combinations of all possible indices of sample_size
            // build first page indices for current_frame
            for (pos = 0; pos < sample_size; ++pos)
                sample_indices[pos] = pos;

            pos = sample_size - 1; // start from last index
            // tries every combination of [sample_size] inputs
            while (sample_indices[0] < max_sample_index)
            {
                // max_sample_index_at_pos = input_size - sample_size + pos;
                while (pos > 0 && sample_indices[pos] == (input_size - sample_size + pos))
                    --pos; //go backwards

                // std::cout << "indices=[";
                // for (int x : sample_indices)
                //     std::cout << x << ", ";
                // std::cout << "]" << std::endl;

                for (int i = 0; i < sample_size; i++)
                {
                    const int sample_index = sample_indices[i];
                    const int offset = i * col_count;

                    // build augmented matrix for input and output at sample_index
                    for (int j = 0; j < sample_size; ++j)
                    {
                        // (sample_size - j - 1) inverts the series such that
                        // 0,1,...,n-1 becomes n-1,n-2,...,1,0
                        const int exponent = sample_size - j - 1;
                        matrix[offset + j] = std::pow(inputs[sample_index], exponent);
                    }
                    matrix[offset + sample_size] = outputs[sample_index]; // augment matrix with output
                }

                // contains the coefficients of x; where x[0] = coeff. of x^sample_size-1
                auto coefficients = xod::guass_jordan_elimination(matrix, row_count, col_count);

                // eliminate extremely small coefficients
                for (int j = 0; j < row_count; ++j)
                {
                    if (coefficients[j] < max_error)
                        coefficients[j] = 0;
                }

                // test current coefficients by
                // comparing the output of the function using the current coefficients
                // to the actual output given for each input
                for (int i = 0; i < input_size; ++i)
                {
                    const double input = inputs[i];
                    const double actual_output = outputs[i];
                    double computed_output = 0;

                    std::cout << "input << " << input << std::endl;

                    std::cout << "coefficients = [";
                    for (int i = 0; i < row_count; ++i)
                    {
                        std::cout << coefficients[i] << ((i + 1) % row_count ? "," : "");
                    }
                    std::cout << "]" << std::endl;

                    // we're starting from the last item because given
                    // ax^n-1 + bx^n-2 +...+ h = y
                    // coefficients = [a, b, ..., h] // coefficients in the function
                    // so that the powers can start from 0, we'll be offsetting j by the sample_size
                    for (int j = 0; j < sample_size; ++j)
                    {
                        // exponent = (sample_size - j - 1) inverts the series such that
                        // 0,1,...,n-2,n-1 becomes n-1,n-2,...,1,0
                        const int exponent = sample_size - j - 1;
                        computed_output += (coefficients[j] * std::pow(input, exponent)); // a_j * x^i
                    }

                    std::cout << computed_output << ", " << actual_output << std::endl;

                    double error = std::abs(computed_output - actual_output);

                    if (error < smallest_error)
                    {
                        smallest_error = error;
                        best_coefficients = coefficients;
                    }
                    std::cout << "error = " << error << std::endl;
                    std::cout << "---------------------" << std::endl;
                }

                sample_indices[pos]++;

                // if its not the last page index
                if (pos < sample_size - 1)
                {
                    // the next page index should be this current_sample_index + 1
                    sample_indices[pos + 1] = sample_indices[pos] + 1;
                    ++pos; //move forwards
                }
            }

            if (smallest_error < max_error)
            {
                std::cout << "best = [";
                for (int i = 0; i < row_count; ++i)
                {
                    std::cout << best_coefficients[i] << ((i + 1) % row_count ? "," : "");
                }
                std::cout << "]" << std::endl;
                std::cout << "done!" << std::endl;
                break;
            }
        }

        return "";
    }
} // namespace xod