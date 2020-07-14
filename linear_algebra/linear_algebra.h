#include <vector>
namespace xod
{
    int guassian_elimination(double *matrix, int row_count, int col_count);
    std::vector<double> guass_jordan_elimination(double *matrix, int row_count, int col_count);
    double determinant(double *matrix, int size);
} // namespace xod
