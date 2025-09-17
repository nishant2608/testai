#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

string write_function(){
    string line1 = "def add_numbers(numbers: list) -> float:\n";
    string line2 = "\treturn sum(numbers)\n";
    return line1 + line2;
}

string write_main(vector<int> args){
    string line1 = "if __name__ == \"__main__\":\n";
    string list_str = "[";
    for(size_t i=0; i<args.size(); i++){
        list_str += to_string(args[i]);
        if(i!=args.size()-1) list_str+=", ";
    }
    list_str += "]";
    string line2 = "\ta=" + list_str + "\n";
    string line3 = "\tresult = add_numbers(a)\n";
    string line4 = "\tprint(f\"The sum is: {result}\")\n";
    return line1 + line2 + line3 + line4;
}

vector<int> parse_arguments(ifstream &arg_file){
    string line;
    vector<int> args;
    if(arg_file.is_open()){
        while(getline(arg_file, line)){
            stringstream ss(line);
            string number;
            while(getline(ss, number, ',')){
                args.push_back(stoi(number));
            }
        }
        arg_file.close();
    }
    return args;
}


int main(int argc, char* argv[]) {
    string filename;
    if(argc < 2){
        filename = "arguments.txt"; // Default filename
    }
    else{
        filename = argv[1];
    }
    ifstream arg_file(filename);
    vector<int> args = parse_arguments(arg_file);
    std::ofstream file("example.py"); // Creates or overwrites example.py
    if (file.is_open()) {
        file << write_function();
        file << "\n";
        file << write_main(args);
        file.close();
        std::cout << "File created successfully.\n";
        system("python example.py");
        cout<<"Execution finished\n";
        cout<< "Enter any character to exit:";
        char exit_char;
        cin>>exit_char;
    } else {
        std::cout << "Failed to create file.\n";
    }
    return 0;
}
