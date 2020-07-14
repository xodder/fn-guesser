#include <iostream>
#include <cmath>
#include "function_guesser.h"
#include "../linear_algebra/linear_algebra.h"

namespace xod
{
    std::string guess_function(std::vector<int> inputs, std::vector<int> outputs)
    {
        const int input_count = inputs.size();
        const double precision = 0.00001;

        for (int frame_size = 2; frame_size <= input_count; frame_size++)
        {
            int row_count = frame_size;
            int col_count = frame_size + 1;
            double matrix[row_count * col_count];

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
            std::vector<double> prev_solution;

            std::cout << page_end_index << ", " << input_count - frame_size << std::endl;

            // frame sliding implementation
            while (!skip_current_frame && page_end_index <= input_count)
            {
                // build simultaneous equations for page
                for (int i = page_start_index; i < page_end_index; ++i)
                {
                    // transform i into 0...n range
                    const int zeroed_i = i - page_start_index;
                    const int offset = zeroed_i * col_count;
                    // build augmented matrix for input and output set at i
                    if (frame_size == 2)
                    {
                        matrix[offset + 0] = inputs[i];
                        matrix[offset + 1] = 1;
                        matrix[offset + 2] = outputs[i];
                    }
                    else if (frame_size == 3)
                    {
                        matrix[offset + 0] = std::pow(inputs[i], 2);
                        matrix[offset + 1] = i + 1;
                        matrix[offset + 2] = 1;
                        matrix[offset + 3] = outputs[i];
                    }
                    else if (frame_size == 4)
                    {
                        matrix[offset + 0] = std::pow(inputs[i], 3);
                        matrix[offset + 1] = std::pow(inputs[i], 2);
                        matrix[offset + 2] = zeroed_i + 1;
                        matrix[offset + 3] = 1;
                        matrix[offset + 4] = outputs[i];
                    }
                }

                auto solution = xod::guass_jordan_elimination(matrix, row_count, col_count);

                // compare current solution and previous one
                if (page_start_index > 0)
                {
                    for (int i = 0; i < frame_size; i++)
                    {
                        const double diff = std::abs(prev_solution[i] - solution[i]);

                        if (diff > precision)
                        {
                            skip_current_frame = true;
                            break;
                        }
                    }
                }

                if (skip_current_frame)
                {
                    break;
                }

                prev_solution = solution;

                page_start_index++;
                page_end_index++;
            }

            if (!skip_current_frame)
            {
                // solution was right
                std::cout << "solution = [";
                for (int i = 0; i < row_count; ++i)
                {
                    std::cout << prev_solution[i] << ((i + 1) % row_count ? "," : "");
                }
                std::cout << "]" << std::endl;
                break;
            }

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