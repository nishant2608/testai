#include <bits/stdc++.h>
#include "models.hpp"
using namespace std;

string write_import_statements(){
    string code =
        "from mcp.server.fastmcp import FastMCP\n"
        "from pydantic import BaseModel\n"
        "import requests\n"
        "\n"
        "# Create an MCP server\n"
        "mcp = FastMCP(\"Demo\")\n";

    return code;
}

string write_schema(Schema sch){
    string code = "class " + sch.getName() + "(BaseModel):\n";
    for(Parameter param : sch.getProperties()){
        code +="\t";
        code+=param.getName();
        Schema s = *(param.getSchema());
        if(s.getType()==Type::STRING){
            code+=": str\n";
        }
        else if(s.getType()==Type::NUMBER){
            code+=": float\n";
        }
        else if(s.getType()==Type::BOOLEAN){
            code+=": bool\n";
        }
        else if(s.getType()==Type::OBJECT){
            code+=": ";
            code += s.getName();
            code += "\n";
        }
    }
    code+="\n";
    return code;
}

std::string toPythonFString(const std::string& url) {
    std::string result = url;
    return "f\"" + result + "\"";
}

string write_tool(FuncDef tool){
    string desc = tool.getSummary();
    // Convert to lowercase and replace spaces with hyphens
    transform(desc.begin(), desc.end(), desc.begin(), [](unsigned char c){ return tolower(c); });
    replace(desc.begin(), desc.end(), ' ', '-');
    string code = "@mcp.tool(\"";
    code+= desc;
    code+= "\")\n";
    string line = "def " + tool.getName() + "(";
    int i=0;
    for(Parameter param : tool.getParameters()){
        Schema sch = *(param.getSchema());
        string type = typeToString(sch.getType());
        string c="";
        if(i){
            c = ",";
        }
        string s = c +  param.getName() + ": " + type;
        line += s;
        i++;
    }
    if(!tool.getSchemaName().empty()){
        if(!tool.getParameters().empty()){
            line+=", ";
        }
        line+= "requestBody : " + tool.getSchemaName();
    }
    line+= ") -> str:";
    code+=line;
    code+="\n";

    string doc = "\t\"\"\"";
    doc+= tool.getDescription();
    doc+="\"\"\"\n";

    code+= doc;

    string line2;
    line2 = "\tresponse = requests." + methodToString(tool.getMethod()) + "(";
    string finalUrl = toPythonFString(tool.getUrl());
    line2+= finalUrl;
    if(!tool.getSchemaName().empty()){
        line2+= ",json = requestBody.model_dump()";
    }
    line2+= ")\n";
    code+=line2;
    string line3 = "\treturn response.text\n\n";
    code+=line3;
    return code;
}

string write_main(){
    std::string code = "if __name__ == \"__main__\":\n"
        "\tprint(\"Starting MCP server...\")\n"
        "\tmcp.run(transport='stdio')\n";

    return code;
}

string write_python_file(vector<FuncDef>& tools,vector<Schema*>& schemas){
    string file;
    file+= write_import_statements();
    file+="\n";
    for(Schema* sch : schemas){
        if(sch->getType() == Type::OBJECT){
            file+= write_schema(*sch);
            file+="\n";
        }
        
    }
    for(FuncDef tool : tools){
        file+=write_tool(tool);
        file+="\n";
    }
    file+=write_main();
    return file;
}