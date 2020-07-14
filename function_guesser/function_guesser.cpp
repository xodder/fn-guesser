#include <iostream>
#include <cmath>
#include "function_guesser.h"
#include "../linear_algebra/linear_algebra.h"

namespace xod
{
    std::string guess_function(std::vector<int> inputs, std::vector<int> outputs)
    {
        // try linear function
        const int input_count = inputs.size();
        std::vector<std::vector<double>> solutions_per_page;
        const double precision = 0.00001;

        for (;;)
        {
            int page_size = 3;
            int row_count = page_size;
            int col_count = page_size + 1;


            int page_start_index = 0;
            int page_end_index = page_start_index + page_size;
            double matrix[row_count * col_count];

            if (page_end_index > input_count || page_size > 4)
            {
                break; //quit
            }

            // build linear simultaneous equations into matrices
            for (int i = page_start_index; i < page_end_index; ++i)
            {
                // transform i into 0...n range
                const int zeroed_i = i - page_start_index;
                const int offset = zeroed_i * col_count;
                // build augmented matrix for input and output set at i
                if (page_size == 2)
                {
                    matrix[offset + 0] = inputs[i];
                    matrix[offset + 1] = 1;
                    matrix[offset + 2] = outputs[i];
                }
                else if (page_size == 3)
                {
                    matrix[offset + 0] = std::pow(inputs[i], 2);
                    matrix[offset + 1] = i + 1;
                    matrix[offset + 2] = 1;
                    matrix[offset + 3] = outputs[i];
                }
                else if (page_size == 4)
                {
                    matrix[offset + 0] = std::pow(inputs[i], 3);
                    matrix[offset + 1] = std::pow(inputs[i], 2);
                    matrix[offset + 2] = zeroed_i + 1;
                    matrix[offset + 3] = 1;
                    matrix[offset + 4] = outputs[i];
                }
            }

            for (int i = page_start_index; i < page_end_index; ++i)
            {
                std::cout << "[";
                for (int j = 0; j < col_count; ++j)
                {
                    std::cout << matrix[i * col_count + j] << ((j + 1) % col_count ? "," : "");
                }
                std::cout << "]" << std::endl;
            }

            auto solutions = xod::guass_jordan_elimination(matrix, row_count, col_count);
            solutions_per_page.push_back(solutions);

            if (page_start_index > 0)
            {
                // compare current solution and previous one
                auto prev_solutions = solutions_per_page[page_start_index - 1];
                for (int i = 0; i < page_size; i++)
                {
                    const double diff = std::abs(prev_solutions[i] - solutions[i]);
                    
                    if (diff > precision)
                    {
                        // try out an higher page size
                        page_size++;
                        solutions_per_page.clear();
                        page_start_index = 0;
                        page_end_index = page_size;
                    }
                }
            }

            std::cout << "solution = [";
            for (int i = 0; i < row_count; ++i)
            {
                std::cout << solutions[i] << ((i + 1) % row_count ? "," : "");
            }
            std::cout << "]" << std::endl;

            break;
        }

        return "";
    }
} // namespace xod