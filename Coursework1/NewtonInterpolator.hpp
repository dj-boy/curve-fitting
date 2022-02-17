//
//  NewtonInterpolator.hpp
//  Interpolation
//
//  Created by Wael Aboulezz on 23/12/2020.
//

#ifndef NewtonInterpolator_hpp
#define NewtonInterpolator_hpp

using namespace std;

class NewtonInterpolator
{
public:
    vector<double> fit(const vector<double> &x, vector<double> f);
    void writeNewtonPolynomial(const vector<double> &a);
    double interpolate(const vector<double> &a, double xval);
};
#endif /* NewtonInterpolator_hpp */



vector<double> NewtonInterpolator::fit(const vector<double> &x, vector<double> f)    
{
    int N = x.size();
    vector<double> c(N), temp(N);

    c[0] = f[0];
    for (int i = 1; i < N; i++)
    {
      for (int j = 0; j < N - i; j++) temp[j] = (f[j+1] - f[j]) / (x[j+i] - x[j]);
      f = temp;
      c[i] = f[0];
    }
    vector<double> a(N, 0.0);
    vector<double> p(N), prev(N);
    p[0] = 1;
    a[0] = c[0] * p[0];
    for (int i = 1; i < N; i++)
    {
       prev = p;
       p[0] = -x[i-1] * prev[0];
       a[0] += c[i] * p[0];
       for (int j = 1; j <= i; j++)
       {
          p[j] = prev[j-1] - x[i-1] * prev[j];
          a[j] += c[i] * p[j];
       }
    }

    return a;
}

void NewtonInterpolator::writeNewtonPolynomial(const vector<double> &a)
{
   int N = a.size();
   cout << a[0];
   for (int i = 1; i < N; i++) cout << showpos << a[i] << "x^" << noshowpos << i;
   cout << '\n';
}


//void NewtonInterpolator::addPoints(const vector<double> &x, vector<double> f)
//{
//   int N = a.size();
//   cout << a[0];
//   for (int i = 1; i < N; i++) cout << showpos << a[i] << "x^" << noshowpos << i;
//   cout << '\n';
//}

double NewtonInterpolator::interpolate(const vector<double> &a, double xval)
{
    int N = a.size();
    double result = a[N-1];
    for (int i = N-2; i >= 0; i--) result = a[i] + xval * result;
    return result;
}

vector<vector<double>> getPointsNewton(const vector<double> &x, vector<double> f, int n, NewtonInterpolator Newton, vector<double> coeff)
{
    double lower_bound = get_min_element(x);
    double upper_bound = get_max_element(x);
    
    vector<double> newX = x;
    vector<double> newY = f;

    uniform_real_distribution<double> unif(lower_bound,upper_bound);
    default_random_engine re;
    for (int i = 0; i < n; i++)
    {
        double a_random_double = unif(re);
        newX.push_back(a_random_double);
        newY.push_back(Newton.interpolate(coeff,a_random_double));
        cout << "x = " << a_random_double << '\n';
        cout << "Newton Interpolation: " << Newton.interpolate(coeff,a_random_double) << '\n';
    }
    
    vector<vector<double>> newPoints(2);
    newPoints[0] = newX;
    newPoints[1] = newY;
    
    return newPoints;
}