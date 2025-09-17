#pragma once

#include<bits/stdc++.h>
using namespace std;

class Parameter{
    private:
        string name;
        string type;
    public:
        Parameter(string n, string t): name(n), type(t) {}
        string getName() const { return name; }
        string getType() const { return type; }
        void setName(const string& n) { name = n; }
        void setType(const string& t) { type = t; }
};

enum Method{
    GET,
    POST,
    PUT,
    DELETE
};

string methodToString(Method m);

Method stringToMethod(string s );

class FuncDef{
    private:
        string name;
        Method method;
        string url;
        string schema_name;
        string summary;
        string description;
        vector<Parameter> parameters;
    public:
        FuncDef() = default;
        FuncDef(string n, string d, vector<Parameter> p, Method m): name(n), description(d), parameters(p), method(m) {}
        string getName() const { return name; }
        string getDescription() const { return description; }
        string getSummary() const { return summary;}
        Method getMethod() const { return method;}
        string getSchemaName() const {return schema_name;}
        string getUrl() const {return url;}
        vector<Parameter> getParameters() const { return parameters; }
        void setName(const string& n) { name = n; }
        void setDescription(const string& d) { description = d; }
        void setParameters(const vector<Parameter>& p) { parameters = p; }
        void setMethod(string s) {method = stringToMethod(s);}
        void setSchemaName(const string& s) {schema_name = s;}
        void setUrl(const string& s) {url = s;}
        void setSummary(const string& s) {summary =s;}
};

class Schema{
    private:
        string name;
        vector<Parameter> parameters;

    public:
        Schema() = default;
        Schema(string n, vector<Parameter> p) : name(n), parameters(p){}
        string getName() const {return name;}
        vector<Parameter> getParameters() const { return parameters; }
        void setName(const string& n) { name = n; }
        void setParameters(const vector<Parameter>& p) { parameters = p; }
};