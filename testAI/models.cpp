#include<bits/stdc++.h>
#include "models.hpp"

using namespace std;

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