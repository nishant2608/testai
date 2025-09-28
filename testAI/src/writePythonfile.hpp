#include<bits/stdc++.h>
#include "models.hpp"
using namespace std;


string write_import_statements();

string write_schema(Schema sch);

string write_tool(FuncDef tool);

string write_main();

string write_python_file(vector<FuncDef>& tools,vector<Schema*>& schemas);