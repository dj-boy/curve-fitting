using namespace std;

class RegressionBase
{

public:
    vector<double> polynomial_coeffs;
    vector<double> dummy_vec;
    vector<vector<double>> dummy_mat;

    int m; // order of polynomial
    double n;

    vector<double> solution;

    double sqrt(double x)
    {
        double square_root = x;

        while ((square_root - (x / square_root)) > x * 10e-5)
        {
            square_root = (square_root + (x / square_root)) / 2;
        }
        return square_root;
    }

    double rmse(std::vector<double> y_true, std::vector<double> y_predict)
    {
        double rmse_val = 0;
        for (int i = 0; i < y_true.size(); ++i)
        {
            rmse_val += (y_predict[i] - y_true[i]) * (y_predict[i] - y_true[i]);
        }
        return sqrt((rmse_val / y_predict.size()));
    }

    RegressionBase()
    {
    }
};
