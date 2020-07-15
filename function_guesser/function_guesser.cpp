#include <sstream>
#include <iostream>
#include <cmath>
#include "function_guesser.h"
#include "../linear_algebra/linear_algebra.h"

// #define VERBOSE 0

namespace xod
{
    std::string _to_equation(std::vector<double> coefficients)
    {
        std::stringstream result;
        int coefficient_count = coefficients.size();
        bool is_first = true;

        for (int i = 0; i < coefficient_count; i++)
        {
            const double coefficient = coefficients[i];
            const int exponent = coefficient_count - i - 1;

            if (coefficient != 0)
            {
                if (coefficient < 0)
                {
                    result << (!is_first ? " - " : "-");
                }
                else
                {
                    result << (!is_first ? " + " : "");
                }

                if (coefficient != 1)
                {
                    result << std::abs(coefficient);
                }

                if (exponent > 0)
                {
                    result << 'x';
                    switch (exponent)
                    {
                    case 1:
                        break;
                    case 2:
                        result << "²";
                        break;
                    case 3:
                        result << "³";
                        break;
                    case 4:
                        result << "⁴";
                        break;
                    case 5:
                        result << "⁵";
                        break;
                    case 6:
                        result << "⁶";
                        break;
                    case 7:
                        result << "⁷";
                        break;
                    case 8:
                        result << "⁸";
                        break;
                    case 9:
                        result << "⁹";
                        break;
                    default:
                        result << '^' << exponent;
                        break;
                    }
                }

                is_first = false;
            }
        }

        return result.str();
    }

    std::string guess_function(std::vector<double> inputs, std::vector<double> outputs)
    {
        const int input_size = inputs.size();
        const double min_allowed_error = 4;
        const double max_allowed_error = 1e-10;
        double smallest_error = INT_MAX;
        std::vector<double> best_coefficients;

        for (int sample_size = 1; sample_size <= input_size; sample_size++)
        {
            const int row_count = sample_size;
            const int col_count = sample_size + 1;
            double matrix[row_count * col_count];
            double sample_indices[sample_size]; // combinations of inputs to use
            const int max_sample_index = input_size - sample_size + 1;
            bool skip_to_next_sample_size = false;
            int pos = 0;

            // using combinations of all possible indices of sample_size
            // build first page indices for current_frame
            for (pos = 0; pos < sample_size; ++pos)
                sample_indices[pos] = pos;

            pos = sample_size - 1; // start from last index
            // tries every combination of [sample_size] inputs
            while (sample_indices[0] < max_sample_index)
            {
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
                // ax^n-1 + bx^n-2 +...+ h = y
                // coefficients = [a, b, ..., h]
                auto coefficients = xod::guass_jordan_elimination(matrix, row_count, col_count);

                // eliminate extremely small coefficients
                for (int j = 0; j < row_count; ++j)
                {
                    if (std::abs(coefficients[j]) < max_allowed_error)
                        coefficients[j] = 0;
                }

#ifdef VERBOSE
                std::cout << "coefficients = [";
                for (int i = 0; i < sample_size; ++i)
                {
                    std::cout << coefficients[i] << ((i + 1) % sample_size ? "," : "");
                }
                std::cout << "]" << std::endl;
#endif

                // test current coefficients by
                // comparing the output of the function using the current coefficients
                // to the actual output given for each input
                double biggest_error_in_combination = 0;

                for (int i = 0; i < input_size; ++i)
                {
                    const double input = inputs[i];
                    const double actual_output = outputs[i];
                    double computed_output = 0;

                    for (int j = 0; j < sample_size; ++j)
                    {
                        // exponent = (sample_size - j - 1) inverts the series such that
                        // 0,1,...,n-2,n-1 becomes n-1,n-2,...,1,0
                        const int exponent = sample_size - j - 1;
                        computed_output += (coefficients[j] * std::pow(input, exponent)); // a_j * x^i
                    }

                    double error = std::abs(computed_output - actual_output);

                    if (error > biggest_error_in_combination)
                    {
                        biggest_error_in_combination = error;
                    }
#ifdef VERBOSE
                    std::cout << computed_output << ", " << actual_output << std::endl;
                    std::cout << "input = " << input << std::endl;
                    std::cout << "error = " << error << std::endl;
                    std::cout << "---------------------" << std::endl;
#endif
                }

                if (biggest_error_in_combination < smallest_error)
                {
                    smallest_error = biggest_error_in_combination;
                    best_coefficients = coefficients;
                }

#ifdef VERBOSE
                std::cout << "biggest = " << biggest_error_in_combination << std::endl;
                std::cout << "smallest = " << smallest_error << std::endl;
                std::cout << "---------------------" << std::endl;
#endif
                if (biggest_error_in_combination > min_allowed_error || smallest_error == 0)
                {
                    break;
                }

                // combination implementation
                // max_sample_index_at_pos = input_size - sample_size + pos;
                while (pos > 0 && sample_indices[pos] == (input_size - sample_size + pos))
                    --pos; //go backwards

                sample_indices[pos]++;

                // if its not the last index in sample_indices
                if (pos < sample_size - 1)
                {
                    // the next index in sample_indices should be this current_sample_index + 1
                    sample_indices[pos + 1] = sample_indices[pos] + 1;
                    ++pos; //move forwards
                }
            }
            if (smallest_error < max_allowed_error)
            {
#ifdef VERBOSE
                std::cout << "best = [";
                for (int i = 0; i < row_count; ++i)
                {
                    std::cout << best_coefficients[i] << ((i + 1) % row_count ? "," : "");
                }
                std::cout << "]" << std::endl;
                std::cout << "done!" << std::endl;
#endif
                break;
            }
        }

        if (smallest_error > max_allowed_error)
        {
#ifdef VERBOSE
            std::cout << "the result is sub-optimal" << std::endl;
#endif
        }

        return _to_equation(best_coefficients);
    }
} // namespace xod