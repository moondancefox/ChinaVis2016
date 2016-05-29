#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<string> getfilenames()
{
    vector<string> filenames;
    ifstream in("data/before_file_names.txt");
    string str;

    while (getline(in, str))
    {
        filenames.push_back(str);
    }
    in.close();
    return filenames;
}

void deal_parserline()
{
    vector<string> filenames = getfilenames();
    string filename, str, _str = "/O=HACKINGTEAM/OU=EXCHANGE ADMINISTRATIVE GROUP (FYDIBOHF23SPDLT)/CN=RECIPIENTS/CN=";
    for (vector<string>::iterator iter = filenames.begin(); iter != filenames.end(); ++iter)
    {
        cout << *iter << endl;

        filename = "data/Before/" + *iter + ".csv";
        ifstream in(filename.c_str());
        filename = "data/After/" + *iter + "_1.csv";
        ofstream out(filename.c_str());

        while (getline(in, str))
        {
            while (str.find(_str) != str.npos)
            {
                str.replace(str.find(_str), _str.size(), "");
            }

            int _count = 0;
            for (int i = 0; i < (int)str.size(); ++i)
            {
                if (str[i] == '\"')
                {
                    _count = (_count + 1) % 2;
                }

                if (str[i] == ',' && _count == 1)
                {
                    str[i] = ' ';
                }
            }
            out << str << endl;
        }
        in.close();
        out.close();
    }
}

vector<string> spiltSymbol(string _tmpstr, string symbol)
{
    int pos;
    vector<string> vec;
    while (_tmpstr.find(symbol) != _tmpstr.npos)
    {
        pos = _tmpstr.find(symbol);
        vec.push_back(_tmpstr.substr(0, pos));
        _tmpstr.replace(0, pos + 1, "");
    }

    vec.push_back(_tmpstr);
    return vec;
}

void spit_semicolons()
{
    vector<string> filenames = getfilenames();
    string filename, str;

    filename = "data/After4/all_4.csv";
    ofstream out4(filename.c_str());
    out4 << "Source,Target,Type,Id,Label,Weight" << endl;
    int _Id = 0;

    for (auto iter : filenames)
    {
        cout << iter << endl;

        filename = "data/After/" + iter + "_1.csv";
        ifstream in(filename.c_str());
        filename = "data/After2/" + iter + "_2.csv";
        ofstream out2(filename.c_str());
        filename = "data/After3/" + iter + "_3.csv";
        ofstream out3(filename.c_str());


        vector<string> line, vec3, vec4, vec5, vec6, vec7, vec8;
        getline(in, str);
        out2 << str << endl;
        out3 << "Source,Target,Type,Id,Label,Weight" << endl;
        int Id = 0;
        while (getline(in, str))
        {
            line = spiltSymbol(str, ",");

            vec3 = spiltSymbol(line.at(3), ";");
            vec4 = spiltSymbol(line.at(4), ";");
            vec5 = spiltSymbol(line.at(5), ";");
            vec6 = spiltSymbol(line.at(6), ";");
            vec7 = spiltSymbol(line.at(7), ";");
            vec8 = spiltSymbol(line.at(8), ";");

            if (vec3.size() == vec4.size())
            {
                for (vector<string>::size_type ix = 0; ix != vec3.size(); ++ix)
                {
                    if (vec3[ix] == "")
                    {
                        break;
                    }

                    out2 << line[0] << "," << line[1] << "," << line[2] << ",";
                    out2 << vec3[ix] << "," << vec4[ix] << "," << "," << "," << "," << ",";
                    out2 << line[9] << "," << line[10] << "," << line[11] << "," << line[12] << "," << line[13] << "," << line[14] << endl;

                    Id++;
                    out3 << line[1] << "," << vec3[ix] << ",Directed," << Id << "," << Id << "," << line [10] << endl;
                    _Id++;
                    out4 << line[1] << "," << vec3[ix] << ",Directed," << _Id << "," << _Id << "," << line [10] << endl;
                }
            }


            if (vec5.size() == vec6.size())
            {
                for (vector<string>::size_type ix = 0; ix != vec5.size(); ++ix)
                {
                    if (vec5[ix] == "")
                    {
                        break;
                    }
                    out2 << line[0] << "," << line[1] << "," << line[2] << ",";
                    out2 << "," << "," << vec5[ix] << "," << vec6[ix] << "," << "," << ",";
                    out2 << line[9] << "," << line[10] << "," << line[11] << "," << line[12] << "," << line[13] << "," << line[14] << endl;

                    Id++;
                    out3 << line[1] << "," << vec5[ix] << ",Directed," << Id << "," << Id << "," << line [10] << endl;
                    _Id++;
                    out4 << line[1] << "," << vec5[ix] << ",Directed," << _Id << "," << _Id << "," << line [10] << endl;
                }
            }


            if (vec7.size() == vec8.size())
            {
                for (vector<string>::size_type ix = 0; ix != vec7.size(); ++ix)
                {
                    if (vec7[ix] == "")
                    {
                        break;
                    }
                    out2 << line[0] << "," << line[1] << "," << line[2] << ",";
                    out2 << "," << "," << vec7[ix] << "," << vec8[ix] << "," << "," << "," ;
                    out2 << line[9] << "," << line[10] << "," << line[11] << "," << line[12] << "," << line[13] << "," << line[14] << endl;

                    Id++;
                    out3 << line[1] << "," << vec7[ix] << ",Directed," << Id << "," << Id << "," << line [10] << endl;
                    _Id++;
                    out4 << line[1] << "," << vec7[ix] << ",Directed," << _Id << "," << _Id << "," << line [10] << endl;
                }
            }
        }
        in.close();
        out2.close();
        out3.close();
    }
    out4.close();
}

int main()
{
    //deal_parserline();

    spit_semicolons();

    return 0;
}

