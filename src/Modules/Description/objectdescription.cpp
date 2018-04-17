#include "objectdescription.h"


ObjectDescription::ObjectDescription(string name, int class_mark):_name(name)
{
    description().push_back(class_mark);
}

ObjectDescription &ObjectDescription::operator<<(const double &feature_value)
{
    _description.push_back(feature_value);
    return *this;
}

ObjectDescription &ObjectDescription::operator<<(initializer_list<double> feature_values)
{
    for(double value: feature_values)
        _description.push_back(value);
    return *this;
}

ObjectDescription &ObjectDescription::operator<<(list<double> feature_values)
{
    for(double value: feature_values)
        _description.push_back(value);
    return *this;
}

string &ObjectDescription::name() {return _name;}

const string &ObjectDescription::name() const {return _name;}

double &ObjectDescription::mark() {return _description.front();}

double ObjectDescription::mark() const {return _description.front();}

list<double> &ObjectDescription::description() {return _description;}

const list<double> &ObjectDescription::description() const {return _description;}

size_t ObjectDescription::size() const {return _description.size();}

