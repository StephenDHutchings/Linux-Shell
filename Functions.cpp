#include "Functions.h"
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <queue>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdlib.h>

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

void BuiltInCommands(queue<CommandObject> user_input){
    /*
     Input: 
     * 
     Ouput:
     */
    
    CommandObject an_object;
    string argument;
    char *args[2];
    char cwd[1024];
    args[0] = "/bin/ls";
    args[1] = NULL;
    
    an_object = user_input.back();
    
    if(an_object.main_command == "cwd"){
        getcwd(cwd, sizeof(cwd));
        cout << cwd;
        cout << endl;
    }
    
    else if(an_object.main_command == "ls"){
        execv(args[0], args);
       
    }
    
    else if(an_object.main_command == "cd"){
        argument = an_object.arguments[0];
        args[0] = StringToChar(argument);
        chdir(args[0]);
    }
 
    
    return;
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

void TutorialContinue(){
    
    CommandObject a_command;
    queue<CommandObject> commands;
    string user_input = "";
    
    cout << "One of the first basic commands is to print the current working directory" << endl;
    cout << "The current working directory is the location in the file hierarchy that the user or program is currently in" << endl;
    cout << "This has implications for other commands that are entered into the shell" << endl;
    cout << "For example, if a user is interested in running a program or application they must be located in the directory that holds that program" << endl;
    cout << "To see the current directory, enter the command cwd" << endl;
    
    while (user_input != "cwd" || user_input != "exit"){
        cin >> user_input;
    }
    
    if(user_input == "exit")
    {
        return;
    }
    
    a_command.main_command = user_input;
    commands.push(a_command);
    
    BuiltInCommands(commands);
    
    cout << "This the directory that you are currently in" << endl;
    
    cout << "The next command that we will look at is the ls command" << endl;
    cout << "The ls command is used to list folders and files in the current directory" << endl;
    cout << "This command can modified with different arguments that customize the way the contents of the directory are listed or the path from which to list the files in" << endl;
    cout << "This tutorial will show the basic command" << endl;
    cout << "To see the contents of the current directory, enter the command ls";
    
    while (user_input != "ls" || user_input != "exit"){
        cin >> user_input;
    }
       
    if(user_input == "exit")
    {
        return;
    }
    
    a_command.main_command = user_input;
    commands.push(a_command);
    
    BuiltInCommands(commands);
    
    cout << "In addition to these basic commands to see the directory system of the operating system, users are also able to navigate through the directory system" << endl;
    cout << "The primary command for this is the cd command, which stands for change directory" << endl;
    cout << "This command takes a valid path in the directory as an argument, and then navigates to the supplied directory" << endl;
    cout << "To use this command, enter cd followed by a pathway" << endl;
    cout << "For example: cd /home/stephen/Desktop" << endl;
    cout << "Try entering this command!" << endl;
    
    cin >> user_input;
    
    return;
}

void TutorialStart(){
    string user_input = " ";
    
    cout << "Welcome to the TutorialShell" << endl;
    cout << "This program is meant to teach users that are unfamiliar with shells the basic commands and functions of a shell" << endl;
    cout << "In a few simple words, shells are a way in which a user can interact with the services provided by the operating system " << endl;
    cout << "This shell will present you with an interface that you can use to type commands" << endl;
    cout << endl;
    cout << "To begin with, we'll start with some basic commands that will allow you to interact with the operating system" << endl;
    cout << "Please enter the word start to begin the tutorial" << endl;
    cout << "If you would like to exit the tutorial, enter the command Exit at anytime" << endl;
    cout << "> ";
    cin >> user_input;
    cout << endl;
    
    if (user_input == "start" || "Start") {
        TutorialContinue();
        }
    else if (user_input == "Exit" || "exit"){
        return;}
    else{
        cout << "Command not recognized" << endl;
    }
    
    return;       
}

