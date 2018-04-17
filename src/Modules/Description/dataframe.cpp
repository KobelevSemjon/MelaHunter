#include "dataframe.h"

DataFrame::DataFrame(std::string name):_name(name){}

DataFrame::DataFrame(const DataFrame &dataframe)
{
    _table = dataframe._table;
    _header = dataframe._header;
    _names = dataframe._names;
    _name = dataframe._name;
}

DataFrame::DataFrame(DataFrame &&dataframe)
{
    swap(*this,dataframe);
}

void DataFrame::operator=(DataFrame &&dataframe)
{
    swap(*this,dataframe);
}

void DataFrame::operator=(const DataFrame &dataframe)
{
    _table = dataframe._table;
    _header = dataframe._header;
    _names = dataframe._names;
}

void DataFrame::reset()
{
    _table.clear();
    _header.clear();
    _names.clear();
}

void DataFrame::setHeader(const list<std::string> &header_list)
{
    reset();
    _header = deque<std::string>(header_list.cbegin(), header_list.cend());
//    for (auto col: header_list)
//        _header.push_back(col);
}

void DataFrame::newObject(const std::string &name, const vector<double> &values)
{
    if (values.size()!=cols()) throw std::invalid_argument("SDataFrame: values size and header size must be equal");
    _names.push_back(name);
    _table.push_back(values);
}

void DataFrame::newObject(const std::string &name, const list<double> &values)
{
    newObject(name, std::vector<double>(values.begin(),values.end()));
}

void DataFrame::newObject(const char name[], initializer_list<double> values)
{

    newObject(string(name), std::vector<double>(values.begin(),values.end()));
}

void DataFrame::newObject(const ObjectDescription &object)
{
    if (object.size()!=cols()) throw std::invalid_argument("SDataFrame: object size and header size must be equal");
    newObject(object.name(), object.description());
}

bool DataFrame::write(const std::string &path)
{
    ofstream fout(path.c_str());
    if (fout.is_open())
    {
        fout << _name.c_str();
        for (string col: _header)
            fout << ";" << col ;

        for (size_t i=0; i<rows(); ++i)
        {
            fout << "\n" << _names.at(i);
            for (double val: _table.at(i))
                fout << ";" << val;
        }
        fout.close();
        return true;
    }
    return false;
}


