#include<iostream>
#include<fstream>
#include<filesystem>
#include<bits/stdc++.h>
#include "json.hpp"
#include "models.hpp"
#include "writePythonfile.hpp"

using json = nlohmann::json;
using namespace std;


string baseUrl;


void extractParams(vector<Parameter>&p, json paramList ){
    for (const auto& param : paramList) {
        Parameter p1(param["name"],param["schema"]["type"]);
        p.push_back(p1);
    }
}

void createTool(vector<FuncDef>& tools, string path, json obj,string method){
    FuncDef tool;
    tool.setName(obj.at("operationId"));
    tool.setSummary(obj.at("summary"));
    tool.setDescription(obj.at("description"));
    tool.setMethod(method);
    tool.setUrl(path);
    vector<Parameter> p;
    if(obj.contains("parameters")){
        extractParams(p,obj.at("parameters"));
    }
    if(obj.contains("requestBody")){
        string ref = obj["requestBody"]["content"]["application/json"]["schema"]["$ref"];
        string last;
        size_t pos = ref.find_last_of('/');
        if (pos != std::string::npos) {
            last = ref.substr(pos + 1);
        }
        tool.setSchemaName(last);
    }
    tool.setParameters(p);
    cout<<"tool created with name: "<<tool.getName()<<", description: "<<tool.getDescription()<<", method: "<<method<<endl;
    tools.push_back(tool);
}

void addFunctions(vector<FuncDef>& tools, string path, json obj){
    string funcUrl = baseUrl;
    if(path!="" && path!="/"){
        funcUrl+= path;
    }
    for(auto it = obj.begin();it!=obj.end();it++){
        createTool(tools,funcUrl,it.value(),it.key());
    }
}

void extractProperties(vector<Parameter>& params, json props){
    for(auto it = props.begin();it!=props.end();it++){
        Parameter p(it.key(),it.value().at("type"));
        params.push_back(p);
    }
}

void addSchema(vector<Schema>& schemas, string name, json obj){
    Schema sch;
    sch.setName(name);
    vector<Parameter> params;
    extractProperties(params,obj.at("properties"));
    sch.setParameters(params);
    cout<<"schema created with name: "<<sch.getName()<<" and "<< sch.getParameters().size()<<" parameters"<<endl;
    schemas.push_back(sch);
}

string writeRequirements(){
    return "mcp-server\nrequests\npydantic";
}


int main(int argc, char* argv[]){
    string filename;
    if(argc < 2){
        filename = "pustak.json"; // Default filename
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

    baseUrl = j.at("servers")[0].at("url");
    vector<FuncDef> tools;
    vector<Schema> schemas;
    const auto& schemasObj = j.at("components").at("schemas");
    for(auto it = schemasObj.begin();it!= schemasObj.end();it++){
        cout<<it.key()<<endl;
        addSchema(schemas,it.key(),it.value());
    }
    const auto& pathsObj = j.at("paths");
    for(auto it = pathsObj.begin();it!=pathsObj.end();it++){
        cout<<it.key()<<endl;
        addFunctions(tools,it.key(),it.value());
    }
    system("mkdir build");
    ofstream reqFile("build/requirements.txt");
    if(reqFile.is_open()){
        reqFile<<writeRequirements();
        reqFile.close();
    }
    ofstream src("build/mcp_srv_src.py");
    if(src.is_open()){
        src<<write_python_file(tools,schemas);
        src.close();
    }
    filesystem::copy_file("run_mcp.ps1","build/run_mcp.ps1");
    cout<<"server created successfully"<<endl;
    
}