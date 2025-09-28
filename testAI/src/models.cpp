#include<bits/stdc++.h>
#include "models.hpp"

using namespace std;


In stringToIn(string s){
    if(s == "path") return PATH;
    if(s == "query") return QUERY;
    if(s == "header") return HEADER;
    if(s == "cookie") return COOKIE;
    throw std::invalid_argument("Invalid method string");
}

string methodToString(Method m) {
    switch (m) {
        case GET: return "get";
        case POST: return "post";
        case PUT: return "put";
        case DELETE: return "delete";
        default: return "UNKNOWN";
    }
}

Method stringToMethod(string s ){
    if(s == "get") return GET;
    if(s == "post") return POST;
    if(s == "put") return PUT;
    if(s == "delete") return DELETE;
    throw std::invalid_argument("Invalid method string");
}

string typeToString(Type t){
    switch(t) {
        case OBJECT: return "object";
        case NUMBER: return "number";
        case ARRAY : return "array";
        case STRING : return "str";
        case BOOLEAN: return "boolean";
        default: return "UNKNOWN";
    }
}

Type stringToType(string s ){
    if(s == "object") return OBJECT;
    if(s == "number"|| s=="float" || s=="int") return NUMBER;
    if(s == "array") return ARRAY;
    if(s == "string") return STRING;
    if(s == "boolean") return BOOLEAN;
    throw std::invalid_argument("Invalid type string");
}