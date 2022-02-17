using namespace std;

class SolveSeidle : public SolverBase
{
public:
    int n_iterations;

    SolveSeidle(Matrix A, vector<double> b, int n, bool verbose=false)
    {
        vector<double> x;

        A = rearrange_rows(A, A.dim);

        // A.scale_matrix();

        bool valid_solution = A.flag_diagonally_dominant();

        if (!valid_solution)
        {
            cout << "This matrix is not in diagonally dominant form. Convergence is not guaranteed!" << endl;
        }

        n_iterations = n;

        for (int i = 0; i < A.dim; i++)
        {
            x.push_back(0);
            solution.push_back(0);
        }

        while (n_iterations > 0)
        {
            for (int i = 0; i < A.dim; i++)
            {
                solution[i] = (b[i] / A.get_row(i)[i]);
                for (int j = 0; j < A.dim; j++)
                {
                    if (j != i)
                    {
                        solution[i] = solution[i] - ((A.get_row(i)[j] / A.get_row(i)[i]) * x[j]);
                        x[i] = solution[i];
                    }
                }
            }
            if (verbose)
            {
                cout << "iteration " << (n - n_iterations + 1) << ": " << endl;
                print_solution(x, A.dim);
                cout << endl
                     << endl;
            }
            else if (n_iterations % 5 == 0)
            {
                cout << "iteration " << (n - n_iterations + 1) << ": " << endl;
                print_solution(x, A.dim);
                cout << endl
                     << endl;
            }
            --n_iterations;
        }
        print_solution(solution, A.dim);
        this->solution > solution;
    }
};