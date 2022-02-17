using namespace std;

class PlynomialRegression : public RegressionBase
{

public:

    vector<double> row_algorithm_left(double x, int n_row)
    {
        vector<double> vec;
        for (int i = 0; i < m; i++)
        {
            if ((i == 0) && (n_row == 0))
            {
                vec.push_back(0);
            }
            else
            {
                double x_ = x;
                for (int j = 1; j < n_row + i; j++)
                {
                    x_ *= x;
                }
                vec.push_back(x_);
            }
        }
        return (vec);
    }

    double row_algorithm_right(double x, double y, int n_row)
    {
        if (n_row == 0)
        {
            return y;
        }
        else
        {
            double x_ = x;
            for (int j = 1; j < n_row; j++)
            {
                x_ *= x;
            }
            return x_ * y;
        }
    }

    void fit(vector<double> x, vector<double> y, int solver, bool verbose=false)
    {
        n = x.size();
        vector<vector<double>> mat = dummy_mat;
        vector<double> b = dummy_vec;

        for (int i = 0; i < n; i++)
        {
            double x_ = x[i];
            vector<double> row;

            double y_ = y[i];
            double b_;
            for (int j = 0; j < m; j++)
            {
                row = row_algorithm_left(x_, j);
                for (int k = 0; k < m; k++)
                {
                    mat[j][k] += row[k];
                }

                b_ = row_algorithm_right(x_, y_, j);
                b[j] += b_;
            }
        }
        mat[0][0] = n;
        Matrix A = Matrix(mat);
        if (solver == 1) 
        {
            SolveGaussElimination sol = SolveGaussElimination(A, b, verbose);
            solution = sol.solution;
        } else if (solver == 2)
        {
            int it;
            cout << "Please Specify the number of iterations" << endl;
            cin >> it;
            SolveSeidle sol = SolveSeidle(A, b, it, verbose);
            solution = sol.solution;
        } else {
            cout << "Invalid choice for a solver (1 for Gauss Elimination, 2 for Gauss Seidle) Gauss Elimination chosen by default" << endl;
            SolveGaussElimination sol = SolveGaussElimination(A, b, verbose);
            solution = sol.solution;
        }
        this->solution > solution;
    }

    vector<double> predict(double start, double end, double step, bool verbose=false)
    {
        vector<double> y;
        
        for (double i = start; i <= end; i += step)
        {
            double y_ = solution[0];
            for (int j = 1; j < m; j++)
            {
                double y__ = i * solution[j];
                for (int k = 1; k < j; k++)
                {
                    y__ *= i;
                }
                y_ += y__;
            }
            y.push_back(y_);
        }
        if (verbose)
        {
            cout << "( ";
            for (int k = 0; k < 100; k++)
            {
                cout << y[k] << ", ";
            }
            cout << ")" << endl;
        }
        return(y);
    }

    PlynomialRegression(int m_)
    {
        m = m_ + 1;
        for (int i = 0; i < m; i++)
        {
            dummy_vec.push_back(0);
        }
        for (int i = 0; i < m; i++)
        {
            dummy_mat.push_back(dummy_vec);
        }
        this->m > m;
        this->dummy_mat > dummy_mat;
        this->dummy_vec > dummy_vec;
    }
};
