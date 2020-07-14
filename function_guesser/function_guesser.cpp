#include "./function_guesser.h"

namespace xod
{
    std::string guess_function(std::vector<int> inputs, std::vector<int> outputs)
    {
        // try linear function
        const int entry_count = inputs.size();
        int current_depth = 2;

        for (;;)
        {
            int page_index = 0;
            int row_count = current_depth;
            int col_count = current_depth + 1;
            int matrix[row_count * col_count];

            // build linear simultaneous equations into matrices
            for (int i = page_index; i < current_depth; ++i)
            {
                // build augmented matrix for input and output set at i
                if (current_depth == 2)
                {
                    matrix[i * row_count + 0] = inputs[i];
                    matrix[i * row_count + 1] = 1;
                    matrix[i * row_count + 2] = outputs[i];
                }
            }

            

            break;
        }

        return "";
    }
} // namespace xod