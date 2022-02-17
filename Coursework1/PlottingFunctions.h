#include "../Matplot/matplotlibcpp.h"
#include <algorithm>
#include <iterator>

namespace plt = matplotlibcpp;

std::vector<vector<double>> get_vertical_line(double x_scatter, double y_scatter, std::vector<double> x, std::vector<double> y)
{
    std::vector<vector<double>> vec_out;
    for (int i = 0; i < x.size(); ++i)
    {
        if ((x_scatter - x[i]) < 0.001)
        {
            std::vector<double> x_out = {x_scatter, x_scatter};
            std::vector<double> y_out = {y_scatter, y[i]};
            vec_out = {x_out, y_out};
            break;
        }
    }
    return (vec_out);
}

class PlotLineScatter
{
public:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> x_scatter;
    std::vector<double> y_scatter;
    std::string filename;

    void gen_plot()
    {
        vector<vector<double>> line;
        plt::figure_size(1200, 780);
        plt::named_plot("Fitted Line", x, y);
        plt::scatter(x_scatter, y_scatter, 40, {{"label", "Original Points"}, {"color", "red"}, {"marker","X"}});
        for (int i = 0; i < x_scatter.size(); ++i)
        {
            line = get_vertical_line(x_scatter[i], y_scatter[i], x, y);
            plt::plot(line[0], line[1], "--r");
        }
        plt::legend();
        plt::save("./" + filename);
    }

    PlotLineScatter(std::vector<double> x_, std::vector<double> y_, std::vector<double> x_scatter_, std::vector<double> y_scatter_, std::string filename_)
    {
        x = x_;
        y = y_;
        x_scatter = x_scatter_;
        y_scatter = y_scatter_;
        filename = filename_;
    }
};


std::vector<std::pair<double, double>> zip(const std::vector<double> a, const std::vector<double> b)
{
    std::vector<std::pair<double,double>> zipped;
    for(int i=0; i<a.size(); ++i)
    {
        zipped.push_back(std::make_pair(a[i], b[i]));
    }
    return(zipped);
}

std::vector<std::vector<double>> unzip(const std::vector<std::pair<double, double>> zipped)
{
    std::vector<double> a;
    std::vector<double> b;
    for(int i=0; i<zipped.size(); i++)
    {
        a.push_back(zipped[i].first);
        b.push_back(zipped[i].second);
    }
    std::vector<std::vector<double>> sorted_vecs;
    sorted_vecs.push_back(a);
    sorted_vecs.push_back(b);
    return(sorted_vecs);
}

bool compareFunc(std::pair<double, double> &a, std::pair<double, double> &b){
    return a.first < b.first;
}

class PlotInterpolation
{
public:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> x_interpolated;
    std::vector<double> y_interpolated;
    std::string filename;

    void gen_plot()
    {
        vector<vector<double>> line;
        plt::figure_size(1200, 780);
        std::vector<double> x_original = x;
        std::vector<double> y_original = y;
        
        //combine given and interpolated points
        x.insert(x.end(), x_interpolated.begin(), x_interpolated.end());
        y.insert(y.end(), y_interpolated.begin(), y_interpolated.end());
        
        // Zip the vectors together
        std::vector<std::pair<double, double>> zipped;
        zipped = zip(x, y);

        std::sort(zipped.begin(), zipped.end(), compareFunc);


        // Write the sorted pairs back to the new vectors
        std::vector<std::vector<double>> sorted_vecs = unzip(zipped);
        std::vector<double> x_sorted = sorted_vecs[0];
        std::vector<double> y_sorted = sorted_vecs[1];
        
        plt::named_plot("After Interpolation", x_sorted, y_sorted);
        
        plt::scatter(x_interpolated, y_interpolated, 20, {{"label", "Interpolated Points"}, {"color", "red"}});
        plt::scatter(x_original, y_original, 40, {{"label", "Original Points"}, {"color", "black"}, {"marker","X"}});
        plt::legend();
        plt::save("./" + filename);
    }

    PlotInterpolation(std::vector<double> x_, std::vector<double> y_, std::vector<double> x_interpolated_, std::vector<double> y_interpolated_, std::string filename_)
    {
        x = x_;
        y = y_;
        x_interpolated = x_interpolated_;
        y_interpolated = y_interpolated_;
        filename = filename_;
    }
};


class PlotResiduals
{
public:
    std::vector<double> y_true;
    std::vector<double> y_predicted;
    std::string filename;

    void gen_plot()
    {
        vector<vector<double>> line;
        plt::figure_size(1200, 780);
        
        vector<double> x;
        vector<double> y;
        vector<double> vec_of_zeroes;

        for (double i = 0; i<y_true.size(); i += 1)
        {
            x.push_back(i);
            y.push_back(y_true[i] - y_predicted[i]);
            vec_of_zeroes.push_back(0);
        }
        
        
        
        plt::scatter(x, y, 40, {{"label", "Original Points"}, {"color", "red"}, {"marker","X"}});
        plt::plot(x, vec_of_zeroes);
        plt::save("./" + filename);
    }

    PlotResiduals(std::vector<double> y_true_, std::vector<double> y_predicted_, std::string filename_)
    {
        y_true = y_true_;
        y_predicted = y_predicted_;
        filename = filename_;
    }
};
