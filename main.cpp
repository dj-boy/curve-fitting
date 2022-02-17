#include "Coursework1/Coursework1lib.h"
using namespace std;

int main()
{
    vector<vector<double>> v = {{2, 1, -1}, {1, 4, 3}, {-1, 2, 7}};
    Matrix A = Matrix(v);
    vector<double> b = {0, 14, 30};

    SolveGaussElimination x = SolveGaussElimination(A, b, true);

    SolveSeidle y = SolveSeidle(A, b, 30, true);

    v = {{2, -1, 0}, {1, -3, 1}, {-1, 1, -3}};
    A = Matrix(v);
    b = {2, -2, 6};

    x = SolveGaussElimination(A, b, true);

    y = SolveSeidle(A, b, 30, true);

    v = {{5, 6, 7}, {6, 3, 9}, {7, 9, 10}};
    A = Matrix(v);
    b = {18, 18, 26};

    x = SolveGaussElimination(A, b, true);

    y = SolveSeidle(A, b, 30, false);

    v = {{10, 2, -1, 2}, {1, 5, 1, 0}, {1, -2, -5, 1}, {3, 0, 0, 9}};
    A = Matrix(v);
    b = {-4, 1, 2, 10};

    x = SolveGaussElimination(A, b, true);

    y = SolveSeidle(A, b, 30, false);

    v = {{-1, 1, -1, 1}, {1, 1, 1, 1}, {8, 4, 2, 1}, {27, 9, 3, 1}};
    A = Matrix(v);
    b = {1, 1, -2, 1};

    x = SolveGaussElimination(A, b, true);

    y = SolveSeidle(A, b, 30, false);

    PlynomialRegression P(2);
    CSVReader csv = CSVReader("./datasets/part_2/a/2_a_dataset_1.csv");
    P.fit(csv.data[0], csv.data[1], 1, true);
    vector<double> y_guass = P.predict(0, 10, 0.1);

    P.fit(csv.data[0], csv.data[1], 2, true);
    vector<double> y_seidle = P.predict(0, 10, 0.1);

    PlynomialRegression P_2(2);
    CSVReader csv_2 = CSVReader("./datasets/part_2/a/2_a_dataset_2.csv");
    P_2.fit(csv_2.data[0], csv_2.data[1], 1, true);
    vector<double> y_guass2 = P_2.predict(0, 10, 1);

    P_2.rmse(csv_2.data[1], y_guass2);


    P_2.fit(csv_2.data[0], csv_2.data[1], 2, true);
    vector<double> y_seidle2 = P_2.predict(0, 10, 1);

    CSVReader csv_3 = CSVReader("./datasets/kaggle_Housing.csv");
    MultipleLinearRegression M;
    M.fit(csv_3.data[2], csv_3.data[1], csv_3.data[0], 1, true);
    vector<double> y_housing = M.predict(csv_3.data[2], csv_3.data[1]);


    std::vector<std::pair<std::string, std::vector<double>>> dataset_1 = read_csv("./datasets/part_3/3_dataset_1.csv");
    std::vector<std::pair<std::string, std::vector<double>>> dataset_2 = read_csv("./datasets/part_3/3_dataset_2.csv");

    vector<double> x1 = dataset_1.at(0).second;
    vector<double> y1 = dataset_1.at(1).second;
    vector<double> x2 = dataset_2.at(0).second;
    vector<double> y2 = dataset_2.at(1).second;

    NewtonInterpolator Newton;

    vector<double> a1 = Newton.fit(x1, y1);
    cout << "Newton Interpolation Polynomial: ";
    Newton.writeNewtonPolynomial(a1);

    vector<double> a2 = Newton.fit(x2, y2);
    cout << "Newton Interpolation Polynomial: ";
    Newton.writeNewtonPolynomial(a2);

    std::vector<string> colnames(2);
    colnames[0] = "Newton Coefficients Dataset 1";
    colnames[1] = "Newton Coefficients Dataset 2";
    std::vector<vector<double>> coeff(2);
    coeff[0] = a1;
    coeff[1] = a2;
    write_csv("Newton Polynomial Coefficients.csv", colnames, coeff);

    std::vector<string> colnamesnewp(2);
    colnamesnewp[0] = "X";
    colnamesnewp[1] = "Y";

    vector<vector<double>> newPointsNewton2D1 = getPointsNewton(x1, y1, x1.size(), Newton, a1);
    write_csv("Newton 2 Times Dataset1.csv", colnamesnewp, newPointsNewton2D1);

    vector<vector<double>> newPointsNewton4D1 = getPointsNewton(x1, y1, x1.size() * 3, Newton, a1);
    write_csv("Newton 4 Times Dataset1.csv", colnamesnewp, newPointsNewton4D1);

    vector<vector<double>> newPointsNewton2D2 = getPointsNewton(x2, y2, x2.size(), Newton, a2);
    write_csv("Newton 2 Times Dataset2.csv", colnamesnewp, newPointsNewton2D2);

    vector<vector<double>> newPointsNewton4D2 = getPointsNewton(x2, y2, x2.size() * 3, Newton, a2);
    write_csv("Newton 4 Times Dataset2.csv", colnamesnewp, newPointsNewton4D2);

    Spline s;
    vector<vector<double>> cSpline1 = s.fitSpline(x1, y1);

    vector<vector<double>> newPointsSpline2D1 = getPointsSpline(x1, y1, x1.size(), s);
    write_csv("Spline 2 Times Dataset1.csv", colnamesnewp, newPointsSpline2D1);

    vector<vector<double>> newPointsSpline4D1 = getPointsSpline(x1, y1, x1.size() * 3, s);
    write_csv("Spline 4 Times Dataset1.csv", colnamesnewp, newPointsSpline4D1);

    vector<vector<double>> newPointsSpline2D2 = getPointsSpline(x2, y2, x2.size(), s);
    write_csv("Spline 2 Times Dataset2.csv", colnamesnewp, newPointsSpline2D2);

    vector<vector<double>> newPointsSpline4D2 = getPointsSpline(x2, y2, x2.size() * 3, s);
    write_csv("Spline 4 Times Dataset2.csv", colnamesnewp, newPointsSpline4D2);


    std::vector<string> colnamesSpline1(5);
    colnamesSpline1[0] = "Spline x Dataset 1";
    colnamesSpline1[1] = "Spline a Dataset 1";
    colnamesSpline1[2] = "Spline b Dataset 1";
    colnamesSpline1[3] = "Spline c Dataset 1";
    colnamesSpline1[4] = "Spline y Dataset 1";

    write_csv("Cubic Spline Interpolator Dataset 1.csv", colnamesSpline1, cSpline1);


    vector<vector<double>> cSpline2 = s.fitSpline(x2, y2);

    std::vector<string> colnamesSpline2(5);
    colnamesSpline2[0] = "Spline x Dataset 2";
    colnamesSpline2[1] = "Spline a Dataset 2";
    colnamesSpline2[2] = "Spline b Dataset 2";
    colnamesSpline2[3] = "Spline c Dataset 2";
    colnamesSpline2[4] = "Spline y Dataset 2";

    write_csv("Cubic Spline Interpolator Dataset 2.csv", colnamesSpline2, cSpline2);
    return (0);
}