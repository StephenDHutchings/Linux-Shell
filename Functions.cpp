#include "Functions.h"

#include <string>
#include <queue>
#include <sstream>
#include <iostream>

using namespace std;

queue<string> PipeSeparator(string input_string) {
    queue<string> command_strings;
    stringstream input_stream(input_string);
    string token;
    
    while (getline(input_stream, token, '|')) {
        command_strings.push(token);
    }
    
    return command_strings;
}
