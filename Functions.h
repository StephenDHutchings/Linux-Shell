#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include <queue>

using namespace std;

struct CommandObject {
    string main_command;
    vector<string> options, arguments;
};

queue<string> PipeSeparator(string input_string);
queue<CommandObject> InputParser(string typed_string);


#endif
