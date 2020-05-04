#include "Functions.h"

#include <string>
#include <queue>
#include <sstream>
#include <iostream>

using namespace std;

queue<string> PipeSeparator(string input_string) {
    /*
     Splits piped inputs into separate commands
     
     inputs:
     input_string: a command entered by the user
     
     output:
     command_strings: a queue of cleaned strings, in order, denoting separate commands to be run in series
     */
    queue<string> command_strings;
    stringstream input_stream(input_string); // Splits input to use getline's delimiter
    string token;
    
    while (getline(input_stream, token, '|')) {
        command_strings.push(token);
    }
    
    return command_strings;
}

queue<CommandObject> InputParser(string typed_string) {
    /*
     Parses inputs, outputing a series of command objects to execute
     
     inputs:
     typed_string: string of commands that the user has typed
     
     output:
     commands: queue of CommandObject objects, each representing a command that needs to run
     */
    queue<CommandObject> commands;
    queue<string> piped_commands = PipeSeparator(typed_string);
    
    while (!piped_commands.empty()) {
        commands.push(CommandObject(piped_commands.front()));
        piped_commands.pop();
    }
    
    return commands;
}
