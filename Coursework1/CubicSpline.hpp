//
//  CubicSpline.hpp
//  Interpolation
//
//  Created by Wael Aboulezz on 25/12/2020.
//

#ifndef CubicSpline_hpp
#define CubicSpline_hpp

using namespace std;


class band_matrix
{
private:
    std::vector< std::vector<double> > m_upper;
    std::vector< std::vector<double> > m_lower;
public:
    band_matrix() {};
    band_matrix(int dim, int n_u, int n_l);
    ~band_matrix() {};
    void resize(int dim, int n_u, int n_l);
    int dim() const;
    int num_upper() const
    {
        return m_upper.size()-1;
    }
    int num_lower() const
    {
        return m_lower.size()-1;
    }

    double & operator () (int i, int j);
    double   operator () (int i, int j) const;
    double& saved_diag(int i);
    double  saved_diag(int i) const;
    void lu_decompose();
    std::vector<double> r_solve(const std::vector<double>& b) const;
    std::vector<double> l_solve(const std::vector<double>& b) const;
    std::vector<double> lu_solve(const std::vector<double>& b,
                                 bool is_lu_decomposed=false);

};

class Spline
{
public:
    enum bd_type {
        first_deriv = 1,
        second_deriv = 2
    };

private:
    std::vector<double> m_x,m_y;
    std::vector<double> m_a,m_b,m_c;
    double  m_b0, m_c0;
    bd_type m_left, m_right;
    double  m_left_value, m_right_value;
    bool    m_force_linear_extrapolation;

public:

    Spline(): m_left(second_deriv), m_right(second_deriv),
        m_left_value(0.0), m_right_value(0.0),
        m_force_linear_extrapolation(false)
    {
        ;
    }

    std::vector<vector<double>> fitSpline(const std::vector<double>& x,const std::vector<double>& y);
    double interpolateSpline (double x) const;
    double deriv(int order, double x) const;
};

#endif

band_matrix::band_matrix(int dim, int n_u, int n_l)
{
    resize(dim, n_u, n_l);
}
void band_matrix::resize(int dim, int n_u, int n_l)
{
    assert(dim>0);
    assert(n_u>=0);
    assert(n_l>=0);
    m_upper.resize(n_u+1);
    m_lower.resize(n_l+1);
    for(size_t i=0; i<m_upper.size(); i++) {
        m_upper[i].resize(dim);
    }
    for(size_t i=0; i<m_lower.size(); i++) {
        m_lower[i].resize(dim);
    }
}
int band_matrix::dim() const
{
    if(m_upper.size()>0) {
        return m_upper[0].size();
    } else {
        return 0;
    }
}



double & band_matrix::operator () (int i, int j)
{
    int k=j-i;
    assert( (i>=0) && (i<dim()) && (j>=0) && (j<dim()) );
    assert( (-num_lower()<=k) && (k<=num_upper()) );
    if(k>=0)   return m_upper[k][i];
    else        return m_lower[-k][i];
}
double band_matrix::operator () (int i, int j) const
{
    int k=j-i;
    assert( (i>=0) && (i<dim()) && (j>=0) && (j<dim()) );
    assert( (-num_lower()<=k) && (k<=num_upper()) );
    if(k>=0)   return m_upper[k][i];
    else        return m_lower[-k][i];
}

double band_matrix::saved_diag(int i) const
{
    assert( (i>=0) && (i<dim()) );
    return m_lower[0][i];
}
double & band_matrix::saved_diag(int i)
{
    assert( (i>=0) && (i<dim()) );
    return m_lower[0][i];
}

void band_matrix::lu_decompose()
{
    int  i_max,j_max;
    int  j_min;
    double x;

    for(int i=0; i<this->dim(); i++) {
        assert(this->operator()(i,i)!=0.0);
        this->saved_diag(i)=1.0/this->operator()(i,i);
        j_min=std::max(0,i-this->num_lower());
        j_max=std::min(this->dim()-1,i+this->num_upper());
        for(int j=j_min; j<=j_max; j++) {
            this->operator()(i,j) *= this->saved_diag(i);
        }
        this->operator()(i,i)=1.0;
    }
    for(int k=0; k<this->dim(); k++) {
        i_max=std::min(this->dim()-1,k+this->num_lower());
        for(int i=k+1; i<=i_max; i++) {
            assert(this->operator()(k,k)!=0.0);
            x=-this->operator()(i,k)/this->operator()(k,k);
            this->operator()(i,k)=-x;
            j_max=std::min(this->dim()-1,k+this->num_upper());
            for(int j=k+1; j<=j_max; j++) {
                this->operator()(i,j)=this->operator()(i,j)+x*this->operator()(k,j);
            }
        }
    }
}

std::vector<double> band_matrix::l_solve(const std::vector<double>& b) const
{
    assert( this->dim()==(int)b.size() );
    std::vector<double> x(this->dim());
    int j_start;
    double sum;
    for(int i=0; i<this->dim(); i++) {
        sum=0;
        j_start=std::max(0,i-this->num_lower());
        for(int j=j_start; j<i; j++) sum += this->operator()(i,j)*x[j];
        x[i]=(b[i]*this->saved_diag(i)) - sum;
    }
    return x;
}

std::vector<double> band_matrix::r_solve(const std::vector<double>& b) const
{
    assert( this->dim()==(int)b.size() );
    std::vector<double> x(this->dim());
    int j_stop;
    double sum;
    for(int i=this->dim()-1; i>=0; i--) {
        sum=0;
        j_stop=std::min(this->dim()-1,i+this->num_upper());
        for(int j=i+1; j<=j_stop; j++) sum += this->operator()(i,j)*x[j];
        x[i]=( b[i] - sum ) / this->operator()(i,i);
    }
    return x;
}

std::vector<double> band_matrix::lu_solve(const std::vector<double>& b,
        bool is_lu_decomposed)
{
    assert( this->dim()==(int)b.size() );
    std::vector<double>  x,y;
    if(is_lu_decomposed==false) {
        this->lu_decompose();
    }
    y=this->l_solve(b);
    x=this->r_solve(y);
    return x;
}

std::vector<vector<double>> Spline::fitSpline(const std::vector<double>& x,const std::vector<double>& y)
{
    assert(x.size()==y.size());
    assert(x.size()>2);
    m_x=x;
    m_y=y;
    int n=x.size();
    // TODO: maybe sort x and y, rather than returning an error
    for(int i=0; i<n-1; i++) {
        assert(m_x[i]<m_x[i+1]);
    }
    band_matrix A(n,1,1);
    std::vector<double>  rhs(n);
    for(int i=1; i<n-1; i++) {
        A(i,i-1)=1.0/3.0*(x[i]-x[i-1]);
        A(i,i)=2.0/3.0*(x[i+1]-x[i-1]);
        A(i,i+1)=1.0/3.0*(x[i+1]-x[i]);
        rhs[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
    }
    if(m_left == Spline::second_deriv) {
        A(0,0)=2.0;
        A(0,1)=0.0;
        rhs[0]=m_left_value;
    } else if(m_left == Spline::first_deriv) {
        A(0,0)=2.0*(x[1]-x[0]);
        A(0,1)=1.0*(x[1]-x[0]);
        rhs[0]=3.0*((y[1]-y[0])/(x[1]-x[0])-m_left_value);
    } else {
        assert(false);
    }
    if(m_right == Spline::second_deriv) {
        A(n-1,n-1)=2.0;
        A(n-1,n-2)=0.0;
        rhs[n-1]=m_right_value;
    } else if(m_right == Spline::first_deriv) {
        A(n-1,n-1)=2.0*(x[n-1]-x[n-2]);
        A(n-1,n-2)=1.0*(x[n-1]-x[n-2]);
        rhs[n-1]=3.0*(m_right_value-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
    } else {
        assert(false);
    }
    m_b=A.lu_solve(rhs);
    m_a.resize(n);
    m_c.resize(n);
    for(int i=0; i<n-1; i++) {
        m_a[i]=1.0/3.0*(m_b[i+1]-m_b[i])/(x[i+1]-x[i]);
        m_c[i]=(y[i+1]-y[i])/(x[i+1]-x[i])
               - 1.0/3.0*(2.0*m_b[i]+m_b[i+1])*(x[i+1]-x[i]);
    }
    m_b0 = (m_force_linear_extrapolation==false) ? m_b[0] : 0.0;
    m_c0 = m_c[0];
    double h=x[n-1]-x[n-2];
    m_a[n-1]=0.0;
    m_c[n-1]=3.0*m_a[n-2]*h*h+2.0*m_b[n-2]*h+m_c[n-2];
    if(m_force_linear_extrapolation==true)
        m_b[n-1]=0.0;
    
    std::vector<vector<double>> C(5);
    C[0] = m_x;
    C[1] = m_a;
    C[2] = m_b;
    C[3] = m_c;
    C[4] = m_y;
    return C;
}

double Spline::interpolateSpline(double x) const
{
    size_t n=m_x.size();
    std::vector<double>::const_iterator it;
    it=std::lower_bound(m_x.begin(),m_x.end(),x);
    int idx=std::max(int(it-m_x.begin())-1, 0);

    double h=x-m_x[idx];
    double interpol;
    if(x<m_x[0]) {
        interpol=(m_b0*h + m_c0)*h + m_y[0];
    } else if(x>m_x[n-1]) {
        interpol=(m_b[n-1]*h + m_c[n-1])*h + m_y[n-1];
    } else {
        interpol=((m_a[idx]*h + m_b[idx])*h + m_c[idx])*h + m_y[idx];
    }
    return interpol;
}

double Spline::deriv(int order, double x) const
{
    assert(order>0);

    size_t n=m_x.size();
    std::vector<double>::const_iterator it;
    it=std::lower_bound(m_x.begin(),m_x.end(),x);
    int idx=std::max( int(it-m_x.begin())-1, 0);

    double h=x-m_x[idx];
    double interpol;
    if(x<m_x[0]) {
        switch(order) {
        case 1:
            interpol=2.0*m_b0*h + m_c0;
            break;
        case 2:
            interpol=2.0*m_b0*h;
            break;
        default:
            interpol=0.0;
            break;
        }
    } else if(x>m_x[n-1]) {
        switch(order) {
        case 1:
            interpol=2.0*m_b[n-1]*h + m_c[n-1];
            break;
        case 2:
            interpol=2.0*m_b[n-1];
            break;
        default:
            interpol=0.0;
            break;
        }
    } else {
        switch(order) {
        case 1:
            interpol=(3.0*m_a[idx]*h + 2.0*m_b[idx])*h + m_c[idx];
            break;
        case 2:
            interpol=6.0*m_a[idx]*h + 2.0*m_b[idx];
            break;
        case 3:
            interpol=6.0*m_a[idx];
            break;
        default:
            interpol=0.0;
            break;
        }
    }
    return interpol;
}

vector<vector<double>> getPointsSpline(const vector<double> &x, vector<double> f, int n, Spline cs)
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
        newY.push_back(cs.interpolateSpline(a_random_double));
        cout << "x = " << a_random_double << '\n';
        cout << "Cubic Spline Interpolation: " << cs.interpolateSpline(a_random_double) << '\n';
    }
    
    vector<vector<double>> newPoints(2);
    newPoints[0] = newX;
    newPoints[1] = newY;
    
    return newPoints;
}
