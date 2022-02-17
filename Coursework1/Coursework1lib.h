#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <assert.h>

double get_max_element(std::vector<double> vec)
{
    double max;
    double temp;
    max = vec[0];
    for (int i = 0; i < vec.size(); ++i)
    {
        temp = vec[i];

        if (temp > max)
        {
            max = temp;
        }
    }
    return (max);
}

double get_min_element(std::vector<double> vec)
{
    double min;
    double temp;
    min = vec[0];
    for (int i = 0; i < vec.size(); ++i)
    {
        temp = vec[i];

        if (temp < min)
        {
            min = temp;
        }
    }
    return (min);
}

#include "Matrix.hpp"
#include "SolverBase.hpp"
#include "SolveGaussElimination.hpp"
#include "SolveSeidle.hpp"
#include "CSVReader.hpp"
#include "RegressionBase.hpp"
#include "PolynomialRegression.hpp"
#include "MultipleLinearRegression.hpp"
#include "NewtonInterpolator.hpp"
#include "CubicSpline.hpp"

void write_csv(string filename, vector<string> colnames, vector<vector<double>> vals)
{
    ofstream outputFile;
    outputFile.open(filename);
    for (int i = 0; i < colnames.size(); i++)
    {
        outputFile << colnames[i] << ",";
        for (int j = 0; j < vals[i].size(); j++)
        {
            outputFile << vals[i].at(j) << ",";
        }
        outputFile << endl;
    }
    outputFile.close();
}

vector<pair<string, vector<double>>> read_csv(string filename)
{
    vector<pair<string, vector<double>>> result;
    ifstream myFile(filename);
    if (!myFile.is_open())
        throw runtime_error("Could not open file");
    string line, colname;
    double val;
    if (myFile.good())
    {
        getline(myFile, line);
        stringstream ss(line);
        while (getline(ss, colname, ','))
        {
            result.push_back({colname, vector<double>{}});
        }
    }
    while (getline(myFile, line))
    {
        stringstream ss(line);
        int colIdx = 0;
        while (ss >> val)
        {
            result.at(colIdx).second.push_back(val);
            if (ss.peek() == ',')
                ss.ignore();
            colIdx++;
        }
    }
    myFile.close();
    return result;
}