
using namespace std;

class MultipleLinearRegression : public RegressionBase
{

public:
    vector<double> element_wise_multiplication(vector<double> x1, vector<double> x2) {
        vector<double> vec;

        for (int i = 0; i < n; i++) {
            vec.push_back(x1[i]*x2[i]);
        }
        return(vec);
    }

    double sum(vector<double> vec) {
        double summation = 0;
        for (int i = 0; i < n; i++) {
            summation += vec[i];
        }
        return(summation);
    }

    void fit(vector<double> x1, vector<double> x2, vector<double> y, int solver, bool verbose=false)
    {
        vector<vector<double>> mat = dummy_mat;
        vector<double> b = dummy_vec;
        n = x1.size();
        
        double sum_x1 = sum(x1);
        double sum_x2 = sum(x2);
        vector<double> e_mult_x1 = element_wise_multiplication(x1, x1);
        double sum_x1_x1 = sum(e_mult_x1);
        vector<double> e_mult_x1_x2 = element_wise_multiplication(x1, x2);
        double sum_x1_x2 = sum(e_mult_x1_x2);
        vector<double> e_mult_x2 = element_wise_multiplication(x2, x2);
        double sum_x2_x2 = sum(e_mult_x2);
        double sum_y = sum(y);
        vector<double> e_mult_x1_y = element_wise_multiplication(x1, y);
        double sum_x1_y = sum(e_mult_x1_y);
        vector<double> e_mult_x2_y = element_wise_multiplication(x2, y);
        double sum_x2_y = sum(e_mult_x2_y);


        vector<double> temp;
        // row1
        vector<double> row1;
        row1.push_back(n);
        row1.push_back(sum_x1);
        row1.push_back(sum_x2);
        mat.push_back(row1);

        // row2
        vector<double> row2;
        row2.push_back(sum_x1);
        row2.push_back(sum_x1_x1);
        row2.push_back(sum_x1_x2);
        mat.push_back(row2);

        // row3
        vector<double> row3;
        row3.push_back(sum_x2);
        row3.push_back(sum_x1_x2);
        row3.push_back(sum_x2_x2);
        mat.push_back(row3);

        // b
        b.push_back(sum_y);
        b.push_back(sum_x1_y);
        b.push_back(sum_x2_y);

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

    vector<double> predict(vector<double> x1, vector<double> x2, bool verbose=false)
    {
        vector<double> y;
        
        for (int i = 0; i < x1.size(); i++)
        {
            y.push_back(solution[0] + solution[1]*x1[i] + solution[2]*x2[i]);
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

    MultipleLinearRegression()
    {
    }
};
