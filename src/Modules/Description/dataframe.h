#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <vector>
#include <deque>
#include <list>
#include <fstream>

#include "objectdescription.h"

using namespace std;

class DataFrame
{

    deque<vector<double>> _table;
    deque<string> _header;
    deque<string> _names;
    string _name;

public:
    DataFrame(string name = "Default");
    DataFrame(const DataFrame& dataframe);
    DataFrame(DataFrame&& dataframe);
    void operator=(const DataFrame& dataframe);
    void operator=(DataFrame&& dataframe);
    void reset();

    size_t cols() const {return _header.size();}
    size_t rows() const {return _names.size();}

    void setHeader(const list<string>& header_list);
    void newObject(const string& name, const vector<double> &values);
    void newObject(const string& name, const list<double> &values);
    void newObject(const char name[], initializer_list<double> values);
    void newObject(const ObjectDescription& object);

    bool write(const string& path);
};

#endif // DATAFRAME_H
