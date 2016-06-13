#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

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
            if (str.find("???") != str.npos)
            {
                continue;
            }

            while (str.find(_str) != str.npos)
            {
                str.replace(str.find(_str), _str.size(), "");
            }

            int _count = 0;
            string str2 = "";
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

                if (str[i] != '\'' && str[i] != '\"' && str[i] != '<' && str[i] != '>')
                {
                    str2 += str[i];
                }
            }
            out << str2 << endl;
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

    filename = "data/After4/all_4_to.csv";
    ofstream out4_to(filename.c_str());
    out4_to << "Source,Target,Type,Id,Label,Weight" << endl;

    filename = "data/After4/all_4_cc.csv";
    ofstream out4_cc(filename.c_str());
    out4_cc << "Source,Target,Type,Id,Label,Weight" << endl;

    filename = "data/After4/all_4_bcc.csv";
    ofstream out4_bcc(filename.c_str());
    out4_bcc << "Source,Target,Type,Id,Label,Weight" << endl;

    map<string, int> map4_to, map4_cc, map4_bcc;

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

            if (line[0] == "" || line[1] == "")
            {
                continue;
            }

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

                    map4_to[line[1] + "," + vec3[ix]] ++;
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

                    map4_cc[line[1] + "," + vec5[ix]] ++;
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

                    map4_bcc[line[1] + "," + vec7[ix]] ++;
                }
            }
        }
        in.close();
        out2.close();
        out3.close();
    }
    out4.close();


    int k = 0;
    _Id = 0;
    for (map<string, int>::iterator iter = map4_to.begin(); iter != map4_to.end(); ++iter)
    {
       k++;
       if (k != 1)
       {
           ++_Id;
           out4_to << iter->first << ",Directed," << _Id << ",," << iter->second << endl;
       }
    }
    out4_to.close();

    k = 0;
    _Id = 0;
    for (map<string, int>::iterator iter = map4_cc.begin(); iter != map4_cc.end(); ++iter)
    {
        k++;
        if (k != 1)
        {
            ++_Id;
            out4_cc << iter->first << ",Directed," << _Id << ",," << iter->second << endl;
        }
    }
    out4_cc.close();

    k = 0;
    _Id = 0;
    for (map<string, int>::iterator iter = map4_bcc.begin(); iter != map4_bcc.end(); ++iter)
    {
        k++;
        if (k != 1)
        {
            ++_Id;
            out4_bcc << iter->first << ",Directed," << _Id << ",," << iter->second << endl;
        }
    }
    out4_bcc.close();
}

typedef pair<string, int> PAIR;

struct cmp  //自定义比较规则
{
    bool operator() (const PAIR& P1, const PAIR& P2)  //注意是PAIR类型，需要.firt和.second。这个和map类似
    {
        return P1.second > P2.second;
    }
};

void HourSpilt()
{
    // Formate :
    // Subject,From (display),From (address),To (display),To (address),Cc (display),Cc (address),Bcc (display),Bcc (address),Creator Name,Importance,Date Sent,Date Received,Size,Attachment Names
    // 00:00-01:00/username/mailcount
    // 2015/6/6 0:47

    vector<string> filenames = getfilenames(), line, sentdata, get_sentHour;
    string hours[25] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12",
                        "13", "14", "15", "16", "17", "18", "19", "10", "21", "22", "23", "24"};
    string filename, str, _str;
    stringstream stream;
    map<string, int> mapHour, topNsender, topNreceiver;
    map<int, int> _mapJsonValue;
    map<string, map<int, int>> mapJson, _mapJson_out;
    for (auto iter : filenames)
    {
        cout << iter << endl;
        filename = "data/After2/" + iter + "_2.csv";
        ifstream in(filename.c_str());

        getline(in, str);
        while (getline(in, str))
        {
            line = spiltSymbol(str, ",");
            if (line[11].size() != 0)
            {
                sentdata = spiltSymbol(line[11], " ");
                if (sentdata.size() == 2)
                {
                    get_sentHour = spiltSymbol(sentdata[1], ":");
                    int _tmp = 0;
                    stream << get_sentHour[0];
                    stream >> _tmp;
                    stream.clear();
                    _str = hours[_tmp] + ":00-" + hours[_tmp] + ":59" + "/" + line[1];
                    mapHour[_str]++;

                    _mapJsonValue = mapJson[line[1]];
                    _mapJsonValue[_tmp]++;
                    mapJson[line[1]] = _mapJsonValue;

                    topNsender[line[1]]++;

                    if (line[3] != "")
                    {
                        topNreceiver[line[3]]++;
                    }
                    else if (line[5] != "")
                    {
                        topNreceiver[line[5]]++;
                    }
                    else if (line[7] != "")
                    {
                        topNreceiver[line[7]]++;
                    }
                }
            }

        }
        in.close();
    }

    ofstream out5("data/after5/hour_information.txt");
    cout << "hour_information" << endl;
    for (map<string, int>::iterator iter = mapHour.begin(); iter != mapHour.end(); ++iter)
    {
        out5 << iter->first << "/" << iter->second << endl;
    }
    out5.close();


    ofstream out5_json("data/after5/hour_information_json.txt");
    cout << "hour_information_json" << endl;
    ifstream in5_json("data/internal_staff.txt");
    string _str2;
    map<int, int> _mapJsonValue2;

    while (getline(in5_json, str))
    {
        line = spiltSymbol(str, ",");
        _str = (line.size() == 2 ? line[1] : line[0]);
        for (map<string, map<int, int>>::iterator iter = mapJson.begin(); iter != mapJson.end(); ++iter)
        {
            _str2 = iter->first;
            _mapJsonValue = iter->second;
            if (_str2.find(_str) != _str2.npos)
            {
                _mapJsonValue2 = _mapJson_out[_str];
                for (map<int, int>::iterator _iter = _mapJsonValue.begin(); _iter != _mapJsonValue.end(); ++_iter)
                {
                    _mapJsonValue2[_iter->first] += _iter->second;
                }
                _mapJson_out[_str] = _mapJsonValue2;
            }
        }
    }

    out5_json << "[" << endl;
    for (map<string, map<int, int>>::iterator iter = _mapJson_out.begin(); iter != _mapJson_out.end(); ++iter)
    {
        int _count = 0;
        out5_json << "  {" << endl;
        out5_json << "    \"send_count\":" << endl;
        out5_json << "    [" << endl;
        _mapJsonValue = iter->second;
        for (map<int, int>::iterator _iter = _mapJsonValue.begin(); _iter != _mapJsonValue.end(); ++_iter)
        {
            out5_json << "      [" << endl;
            out5_json << "        " << _iter->first << ",\n        " << _iter->second << endl;
            out5_json << "      ]" << (_iter == --_mapJsonValue.end() ? "" : ",") << endl;
            _count += _iter->second;

        }
        out5_json << "    ]," << endl;
        out5_json << "    \"total\": " << _count << "," << endl;
        out5_json << "    \"name\": \"" << iter->first << "\"" << endl;
        out5_json << "  }" << (iter == --_mapJson_out.end() ? "" : ",") << endl;
    }

    out5_json << "]" << endl;

    in5_json.close();
    out5_json.close();




    vector<PAIR>scoreVector;
    ofstream out5_sender("data/after5/topNsender.txt");
    cout << "topNsender" << endl;
    vector<PAIR> topNsender_v;
    for(map<string, int>::iterator iter = topNsender.begin(); iter != topNsender.end(); iter++)  //这边本来是使用vector直接初始化的，当时由于vc 6.0 编译器问题，只能这样写，而且还有非法内存。。
    {
        topNsender_v.push_back(*iter);
    }
    stable_sort (topNsender_v.begin(), topNsender_v.end(), cmp());  //需要指定cmp
    for(int i = 0; i != topNsender_v.size(); i++)  //也要按照vector的形式输出
    {
        out5_sender << topNsender_v[i].first << "," << topNsender_v[i].second << endl;
    }
    out5_sender.close();


    ofstream out5_receiver("data/after5/topNreceiver.txt");
    cout << "topNreceiver" << endl;
    vector<PAIR> topNreceiver_v;
    for(map<string, int>::iterator iter = topNreceiver.begin(); iter != topNreceiver.end(); iter++)  //这边本来是使用vector直接初始化的，当时由于vc 6.0 编译器问题，只能这样写，而且还有非法内存。。
    {
        topNreceiver_v.push_back(*iter);
    }
    stable_sort (topNreceiver_v.begin(), topNreceiver_v.end(), cmp());  //需要指定cmp
    for(int i = 0; i != topNreceiver_v.size(); i++)  //也要按照vector的形式输出
    {
        out5_receiver << topNreceiver_v[i].first << "," << topNreceiver_v[i].second << endl;
    }
    out5_receiver.close();


    ofstream out5_send_receive_61("data/after5/out5_send_receive_61.txt");
    cout << "out5_send_receive" << endl;
    ifstream in5_send_receive_61("data/internal_staff.txt");
    while (getline(in5_send_receive_61, str))
    {
        line = spiltSymbol(str, ",");
        _str = (line.size() == 2 ? line[1] : line[0]);
        out5_send_receive_61 << _str << "\t" << topNsender[_str] << "\t" << topNreceiver[_str] << endl;
    }
    in5_send_receive_61.close();
    out5_send_receive_61.close();

}



int main()
{
    //deal_parserline();

    //spit_semicolons();

    HourSpilt();

    return 0;
}

