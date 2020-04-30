#include "Functions.h"
#include <unistd.h>
#include <stdio.h>
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
     Parses inputs, out putting a series of command objects to execute
     
     inputs:
     typed_string: string of commands that the user has typed
     
     output:
     queue of CommandObject objects, each representing a command that needs to run
     */
    queue<CommandObject> commands;
    queue<string> piped_commands = PipeSeparator(typed_string);
    
    for (int i = 0; i < piped_commands.size(); i++) {
        commands.push(CommandObject(piped_commands.front()));
        piped_commands.pop();
    }
    
    return commands;
}

string StartShell(){
    string user_input = " ";
    
    cout << "Welcome to TutorialShell!" << endl;
    cout << "You may use this Shell as a learning tool by entering the command Tutorial" << endl;
    cout << "Otherwise, enter any other valid command" << endl;
    cout << "> "; 
    getline(cin, user_input);
    return user_input;
}

void TutorialStart(){
    string user_input = " ";
    
    cout << "Welcome to the TutorialShell" << endl;
    cout << "This program is meant to teach users that are unfamiliar with shells the basic commands and functions of a shell" << endl;
    cout << "In a few simple words, shells are a way in which a user can interact with the services provided by the operating system " << endl;
    cout << "This shell will present you with an interface that you can use to type commands" << endl;
    cout << endl << endl << endl;
    cout << "To begin with, we'll start with some basic commands that will allow you to interact with the operating system" << endl;
    cout << "Please enter the word start to begin the tutorial" << endl;
    cout << "If you would like to exit the tutorial, enter the command Exit at anytime" << endl;
    cout << "> ";
    cin >> user_input;
    
    if (user_input == "start" || "Start") {
        }
    else if (user_input == "Exit" || "exit"){
        return;}
    else{
        cout << "Command not recognized" << endl;
    }
    
    return;       
}

char* StringToChar(string input_string){
    /*
     Function to convert a C++ string to a char*
     Modifications in the string are reflected in the c string
     Used when c_string needs to be passed as an argument 
     * 
     Input: C++ string
     * 
     Output: Pointer to c_string
     */
    
    char* c_string = const_cast<char*>(input_string.c_str());

    
    return c_string;
}

void CurrentWorkingDirectory(queue<CommandObject> user_input){
    /*
     Input: 
     * 
     Ouput:
     */
    char cwd[1024];
    CommandObject an_object;
    
    an_object = user_input.back();
    
    if(an_object.main_command == "cwd"){
        getcwd(cwd, sizeof(cwd));
        cout << cwd;
    }
    
    return;
}
