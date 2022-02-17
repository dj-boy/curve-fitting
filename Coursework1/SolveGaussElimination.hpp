using namespace std;

class SolveGaussElimination : public SolverBase
{
public:

    SolveGaussElimination(Matrix A, vector<double> b, bool verbose=false)
    {
        bool singular_flag = A.flag_singular();
        bool ill_conditioned_flag = A.flag_ill_conditioned();
        bool size_correct = A.augment_matrix(b);

        bool valid_solution = (singular_flag && ill_conditioned_flag && size_correct);

        if (valid_solution)
        {
            A = rearrange_rows(A, A.dim);
            if (verbose)
            {
                cout << "Matrix Rearranged" << endl;
                print_aug_matrix(A);
            }

            A.scale_matrix();
            if (verbose)
            {
                cout << "Matrix Scaled" << endl;
                print_aug_matrix(A);
            }

            A = convert_to_triangular(A);

            A.scale_matrix();

            bool determinant_check_ = determinant_check(A);
            valid_solution = valid_solution && determinant_check_;

            if (valid_solution)
            {
                A.row_echelon_form();

                if (verbose)
                {
                    cout << "Matrix in Row Echelon Form" << endl;
                    print_aug_matrix(A);
                }

                solution = get_solution(A);

                if (verbose)
                {
                    cout << "The Solution is: " << endl;
                    print_solution(solution, A.dim);
                }
            }
            else
            {
                cout << "The determinant is equal to Zero!" << endl;
            }
        }
        else if (!size_correct)
        {
            cout << "The sizes of the vector and Matrix provided do not match" << endl;
        }
        else if (!ill_conditioned_flag)
        {
            cout << "This Matrix is ill-conditioned";
        }
        else
        {
            cout << "This Matrix is Singular";
        }
        this->solution > solution;
    }
};
