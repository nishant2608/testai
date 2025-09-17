#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include "json.hpp"


using json = nlohmann::json;
using namespace std;


class Parameter{
    private:
        string name;
        string type;
        string description;
    public:
        Parameter(string n, string t, string d): name(n), type(t), description(d) {}
        string getName() const { return name; }
        string getType() const { return type; }
        string getDescription() const { return description; }
        void setName(const string& n) { name = n; }
        void setType(const string& t) { type = t; }
        void setDescription(const string& d) { description = d; }
};

class Return_Type{
    private:
        string type;
        string description;
    public:
        Return_Type(string t, string d): type(t), description(d) {}
        string getType() const { return type; }
        string getDescription() const { return description; }
        void setType(const string& t) { type = t; }
        void setDescription(const string& d) { description = d; }
};

class FuncDef{
    private:
        string name;
        string description;
        vector<Parameter> parameters;
        Return_Type return_type;
    public:
        FuncDef() = default;
        FuncDef(string n, string d, vector<Parameter> p, Return_Type r): name(n), description(d), parameters(p), return_type(r) {}
        string getName() const { return name; }
        string getDescription() const { return description; }
        vector<Parameter> getParameters() const { return parameters; }
        Return_Type getReturnType() const { return return_type; }
        void setName(const string& n) { name = n; }
        void setDescription(const string& d) { description = d; }
        void setParameters(const vector<Parameter>& p) { parameters = p; }
        void setReturnType(const Return_Type& r) { return_type = r; }
};

FuncDef mapJsonToFuncDef(const json& j){
    string name = j.at("name").get<string>();
    string description = j.at("description").get<string>();
    
    vector<Parameter> parameters;
    for(const auto& param : j.at("parameters")){
        string param_name = param.at("name").get<string>();
        string param_type = param.at("type").get<string>();
        string param_description = param.at("description").get<string>();
        parameters.emplace_back(param_name, param_type, param_description);
    }
    
    const auto& ret = j.at("return_type");
    string return_type = ret.at("type").get<string>();
    string return_description = ret.at("description").get<string>();
    Return_Type returnType(return_type, return_description);
    
    return FuncDef(name, description, parameters, returnType);
}

int main(int argc, char* argv[]){
    string filename;
    if(argc < 2){
        filename = "test.json"; // Default filename
    }
    else{
        filename = argv[1];
    }
    ifstream file(filename);
    json j;
    
    if(file.is_open()){
        file >> j;
        file.close();
    }
    FuncDef func = mapJsonToFuncDef(j);
    cout<<"Function Name: " << func.getName() << endl;
    cout<<"Description: " << func.getDescription() << endl;
    cout<<"Parameters: " << endl;
    for(const auto& param : func.getParameters()){
        cout<<"\tName: " << param.getName() << ", Type: " << param.getType() << ", Description: " << param.getDescription() << endl;
    }
    cout<<"Return Type: " << func.getReturnType().getType() << ", Description: " << func.getReturnType().getDescription() << endl;
    return 0;
}