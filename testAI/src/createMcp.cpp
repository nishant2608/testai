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
vector<Schema*> schemas;
unordered_set<string> schemaNames;
Schema* strSchema;
Schema* numSchema;
Schema* boolSchema;

json mainJson;


Schema* addSchema(string name, json obj);

string nameFromReference(string ref){
    string last;
    size_t pos = ref.find_last_of('/');
    if (pos != std::string::npos) {
        last = ref.substr(pos + 1);
    }
    return last;
}

void extractParams(vector<Parameter>&p, json paramList ){
    for (const auto& param : paramList) {
        // Parameter p1(param["name"],param["schema"]["type"]);
        Parameter p1;
        p1.setIn(param["in"]);
        p1.setName(param["name"]);
        json schema = param["schema"];
        if(schema["type"] == "string"){
            p1.setSchema(strSchema);
        }
        else if(schema["type"]=="number"){
            p1.setSchema(numSchema);
        }
        else if(schema["type"]=="boolean"){
            p1.setSchema(boolSchema);
        }
        else{
            p1.setSchema(addSchema(p1.getName(),schema));
        }
        p.push_back(p1);
    }
}


void createTool(vector<FuncDef>& tools, string path, json obj,string method){
    FuncDef tool;
    tool.setName(obj.at("operationId"));
    tool.setSummary(obj.at("summary"));
    if(obj.contains("description")) tool.setDescription(obj.at("description"));
    tool.setMethod(method);
    tool.setUrl(path);
    vector<Parameter> p;
    if(obj.contains("parameters")){
        extractParams(p,obj.at("parameters"));
    }
    if(obj.contains("requestBody")){
        if(obj["requestBody"]["content"]["application/json"]["schema"].contains("$ref")){
            string ref = obj["requestBody"]["content"]["application/json"]["schema"]["$ref"];
            tool.setSchemaName(nameFromReference(ref));
        }
        else{
            string schemaName = tool.getSummary()+ " request body"; 
            transform(schemaName.begin(), schemaName.end(), schemaName.begin(), [](unsigned char c){ return tolower(c); });
            replace(schemaName.begin(), schemaName.end(), ' ', '_');
            tool.setSchemaName(schemaName);
            addSchema(schemaName,obj["requestBody"]["content"]["application/json"]["schema"]);
        }
        
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

// void extractProperties(vector<Parameter>& params, json props){
//     for(auto it = props.begin();it!=props.end();it++){
//         Parameter p(it.key(),it.value().at("type"));
//         params.push_back(p);
//     }
// }

Schema* addSchema(string name, json obj){
    if(obj.contains("$ref")){
        string nameRef = nameFromReference(obj.at("$ref"));
        if(schemaNames.find(nameRef)!=schemaNames.end()){
            for(Schema* sch_itr : schemas){
                if(sch_itr->getName()==nameRef){
                    return sch_itr;
                }
            }
        }
        else{
            json compSchema = mainJson["components"]["schemas"][nameRef];
            return addSchema(nameRef,compSchema);
        } 
    }
    if(schemaNames.find(name)!=schemaNames.end()){
        for(Schema* sch_itr : schemas){
            if(sch_itr->getName()==name){
                return sch_itr;
            }
        }
    }
    Schema* sch = new Schema();
    sch->setName(name);
    sch->setType(obj.at("type"));
    if(obj.contains("description"))sch->setDescription(obj.at("description"));
    if(sch->getType()== Type::OBJECT){
        if(obj.contains("required")) sch->setRequired(obj.at("required"));
        json props = obj.at("properties");
        vector<Parameter> properties;
        for(auto it = props.begin();it!=props.end();it++){
            Schema* propSchem = addSchema(it.key(),it.value());
            Parameter p1;
            p1.setName(it.key());
            p1.setSchema(propSchem);
            properties.push_back(p1);
        }
        sch->setProperties(properties);
    }
    else if(sch->getType()==Type::ARRAY){   
        json items = obj.at("items");
        Schema* itm = addSchema(name+"_items",items);
        sch->setItems(itm);
    }
    else if(sch->getType()==Type::BOOLEAN){
        return boolSchema;
    }
    else if(sch->getType()==Type::STRING){
        return strSchema;
    }
    else if(sch->getType()==Type::NUMBER){
        return numSchema;
    }
    cout<<"schema created with name: "<<sch->getName()<<" and "<< sch->getProperties().size()<<" properties"<<endl;
    schemas.push_back(sch);
    schemaNames.insert(sch->getName());
    return sch;
}

string writeRequirements(){
    return "mcp-server\nrequests\npydantic";
}


int main(int argc, char* argv[]){
    string filename;
    if(argc < 2){
        filename = "api_spec.json"; // Default filename
    }
    else{
        filename = argv[1];
    }
    ifstream file(filename);
    
    if(file.is_open()){
        file >> mainJson;
        file.close();
    }

    baseUrl = mainJson.at("servers")[0].at("url");

    strSchema = new Schema();
    strSchema->setName("strSchema");
    strSchema->setType("string");

    numSchema = new Schema();
    numSchema->setName("numSchema");
    numSchema->setType("number");

    boolSchema = new Schema();
    boolSchema->setName("boolSchema");
    boolSchema->setType("boolean");

    vector<FuncDef> tools;
    const auto& schemasObj = mainJson.at("components").at("schemas");
    for(auto it = schemasObj.begin();it!= schemasObj.end();it++){
        cout<<it.key()<<endl;
        addSchema(it.key(),it.value());
    }
    const auto& pathsObj = mainJson.at("paths");
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
    filesystem::copy_file("src/run_mcp.ps1","build/run_mcp.ps1");
    cout<<"server created successfully"<<endl;
    
}