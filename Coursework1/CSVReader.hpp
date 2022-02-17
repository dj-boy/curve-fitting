using namespace std;

class CSVReader
{
public:
    vector<vector<double>> data;
    int n_cols;
    vector<string> col_names;

    int count_cols(string s)
    {
        int count = 1;

        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ',')
            {
                count += 1;
            }
        }
        return count;
    }

    vector<string> splitcomma(string str)
    {
        stringstream ss(str);
        vector<string> tokens;
        string temp_str;
        while (getline(ss, temp_str, ','))
        {
            tokens.push_back(temp_str);
        }
        return tokens;
    }

    CSVReader(string input_file_name)
    {
        ifstream input_file(input_file_name);
        int count = 0;

        while (input_file.good())
        {
            string line;
            getline(input_file, line);
            if (count == 0)
            {
                n_cols = count_cols(line);
                col_names = splitcomma(line);
                for (int i = 0; i < n_cols; i++)
                {
                    vector<double> row;
                    data.push_back(row);
                }
                count++;
            }
            else
            {
                vector<string> row = splitcomma(line);
                for (int i = 0; i < n_cols; i++)
                {
                    data[i].push_back(stod(row[i]));
                }
            }
        }
        this->data > data;
        this->col_names > col_names;
    }
};
