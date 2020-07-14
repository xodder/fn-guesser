#include <iostream>
#include <cmath>
#include "function_guesser.h"
#include "../linear_algebra/linear_algebra.h"

namespace xod
{
    std::string guess_function(std::vector<int> inputs, std::vector<int> outputs)
    {
        const int input_count = inputs.size();
        const double max_error = 1e-10;

        for (int frame_size = 1; frame_size <= input_count; frame_size++)
        {
            int row_count = frame_size;
            int col_count = frame_size + 1;
            double matrix[row_count * col_count];

            // (4, 2) -> [0,1], [0,2], [0,3], [1, 2], [1, 3], [2, 3]
            // for (int i = 0; i < input_count; ++i)
            // {
            // }

            // frame sliding
            // frame_size = 2
            // | x x x       x x x       x x x
            // | x x x  -> | x x x  ->   x x x
            //   x x x     | x x x     | x x x
            //   x x x       x x x     | x x x
            //  p_s_i = 0     = 1        = 2
            //  p_s_i = page_start_index

            int page_start_index = 0;
            int page_end_index = page_start_index + frame_size;
            bool skip_current_frame = false;
            double smallest_error;
            std::vector<double> best_solution;
            bool is_first_test = true;

            // frame sliding implementation
            while (!skip_current_frame && page_end_index < input_count)
            {
                // build simultaneous equations for page
                for (int i = page_start_index; i < page_end_index; ++i)
                {
                    // transform i into 0...n range
                    const int zeroed_i = i - page_start_index;
                    const int offset = zeroed_i * col_count;

                    // build augmented matrix for input and output set at i
                    for (int j = 0; j < frame_size; ++j)
                    {
                        matrix[offset + j] = std::pow(inputs[i], frame_size - j - 1);
                    }
                    matrix[offset + frame_size] = outputs[i]; // augment with output
                }

                auto solution = xod::guass_jordan_elimination(matrix, row_count, col_count);

                // compare the output of the function from the current solution
                // to the actual output given for each input
                double biggest_error = 0;

                for (int i = 0; i < input_count; ++i)
                {
                    const double input = inputs[i];
                    const double actual_output = outputs[i];
                    double output_from_solution = 0;

                    for (int j = 0; j < row_count; ++j)
                    {
                        output_from_solution += (solution[i] * std::pow(input, j)); // a_i * x^i
                    }

                    double error = std::abs(output_from_solution - actual_output);

                    if (error > biggest_error)
                    {
                        biggest_error = error;
                    }
                }

                if (biggest_error < smallest_error || is_first_test)
                {
                    best_solution = solution;
                    smallest_error = biggest_error;
                }

                is_first_test = false;

                // std::cout << page_start_index << ", " << page_end_index << std::endl;

                std::cout << "solution = [";
                for (int i = 0; i < row_count; ++i)
                {
                    std::cout << best_solution[i] << ((i + 1) % row_count ? "," : "");
                }
                std::cout << "]" << std::endl;

                if (page_end_index == input_count - 1 && smallest_error < max_error)
                {
                    return "";
                }

                /* for (int i = 0; i < frame_size; i++)
                    {
                        const double diff = std::abs(prev_solution[i] - solution[i]);

                        if (diff > precision)
                        {
                            skip_current_frame = true;
                            break;
                        }
                    } */

                if (skip_current_frame)
                {
                    break;
                }

                page_start_index++;
                page_end_index++;
            }

            // if (!skip_current_frame)
            // {
            //     // solution was right
            //     std::cout << "solution = [";
            //     for (int i = 0; i < row_count; ++i)
            //     {
            //         std::cout << prev_solution[i] << ((i + 1) % row_count ? "," : "");
            //     }
            //     std::cout << "]" << std::endl;
            //     break;
            // }

            /* for (int i = page_start_index; i < page_end_index; ++i)
            {
                std::cout << "[";
                for (int j = 0; j < col_count; ++j)
                {
                    std::cout << matrix[i * col_count + j] << ((j + 1) % col_count ? "," : "");
                }
                std::cout << "]" << std::endl;
            } */
        }

        return "";
    }
} // namespace xod