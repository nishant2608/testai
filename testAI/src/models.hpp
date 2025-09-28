#pragma once

#include<bits/stdc++.h>
using namespace std;

enum In{
    PATH,
    QUERY,
    HEADER,
    COOKIE
};

In stringToIn(string s);

class Schema;

class Parameter{
    private:
        string name;
        In in;
        Schema* schema;
    public:
        Parameter() = default;

        // Getters
        string getName() const { return name; }
        In getIn() const { return in; }
        Schema* getSchema() const { return schema; }

        // Setters
        void setName(const string& n) { name = n; }
        void setIn(const string& i) { in = stringToIn(i); }
        void setSchema(Schema* s) { schema = s; }
};

enum Method{
    GET,
    POST,
    PUT,
    DELETE
};

string methodToString(Method m);

Method stringToMethod(string s );


enum Type{
    OBJECT,
    STRING,
    ARRAY,
    NUMBER,
    BOOLEAN
};

string typeToString(Type t);

Type stringToType(string s);

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
        Type type;
        string description;
        vector<Parameter> properties;
        vector<string> required;
        Schema* items;

    public:
        Schema() = default;

        // Getters
        string getName() const { return name; }
        Type getType() const { return type; }
        string getDescription() const { return description; }
        vector<Parameter> getProperties() const { return properties; }
        vector<string> getRequired() const { return required; }
        Schema* getItems() const { return items; }

        // Setters
        void setName(const string& n) { name = n; }
        void setType(const string& t) { type = stringToType(t); }
        void setDescription(const string& d) { description = d; }
        void setProperties(const vector<Parameter> p) { properties = p; }
        void setRequired(const vector<string>& r) { required = r; }
        void setItems(Schema* i) { items = i; }
};