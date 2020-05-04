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
#include <sys/wait.h>

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

void TheCommands(queue<CommandObject>& commands){
    /*
     Input: 
     * 
     Ouput:
     */
    
    pid_t child_pid;
    int status;
    CommandObject a_command;
    a_command = commands.front();
    commands.pop();
    char* args[a_command.arguments.size() + 1];
    
    for (int i = 0; i < a_command.arguments.size(); i++) {
      args[i] = (char*) a_command.arguments[i].c_str();
    }
    
    args[a_command.arguments.size()] = NULL;
    
    child_pid = fork();
    
    if(child_pid < 0){
        cout << "Forking failed" << endl;
    }
            
    else if(child_pid == 0) {
   
        if(a_command.main_command == "/bin/pwd" || a_command.main_command == "/bin/ls"){
            if(execve(a_command.main_command.c_str(), args, NULL) < 0)
            {cout << "Could not execute command" << endl;}
        }
    }
    
    else {
        wait(NULL);
        return;
    }
    /*
    else if(an_object.main_command == "cd"){
        argument = an_object.arguments[0];
        args[0] = StringToChar(argument);
        chdir(args[0]);
    }
     */
}

string StartShell(){
    string user_input = " ";
    
    cout << "> Welcome to TutorialShell!" << endl;
    cout << "> You may use this Shell as a learning tool by entering the command Tutorial" << endl;
    cout << "> Otherwise to use this program as a typical Linux shell, enter any other valid command" << endl;
    cout << "> "; 
    getline(cin, user_input);
    cout << endl;
    return user_input;
}

void Shell()
{
    string user_input;
    queue<CommandObject> commands;
    CommandObject a_command;
    bool done = false;
    
    while (!done) {
        cout << "> ";
        getline(cin, user_input);
        if (user_input != "exit" && user_input != "Exit"){
            commands = InputParser(user_input);
            a_command = commands.front();
            TheCommands(commands);
        } else {
            done = true;
        }
    }
    
    return;
}

void TutorialContinue(){
    
    CommandObject a_command;
    queue<CommandObject> commands;
    string user_input = " ";
    
    cout << "> One of the first basic commands is to print the current working directory" << endl;
    cout << "> The current working directory is the location in the file hierarchy that the user is currently in" << endl;
    cout << "> This has implications for other commands that are entered into the shell" << endl;
    cout << "> For example, if a user is interested in running a program or application they must be located in the directory that holds that program" << endl;
    cout << "> To see the current directory, enter the command pwd" << endl << "> ";
 
    cin >> user_input;
     
    if(user_input == "exit")
    {
        return;
    }
    
  
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << "> This the directory that you are currently in" << endl << endl << endl;
    cout << "> The next command that we will look at is the ls command" << endl;
    cout << "> The ls command is used to list folders and files in the current directory" << endl;
    cout << "> This command can be modified with different arguments that customize the way the contents of the directory are listed " << endl;
    cout << "> or the path from which to list the files in" << endl;
    cout << "> This tutorial will show the basic command, as well as an example with arguments" << endl;
    cout << "> To see the contents of the current directory, enter the command ls" << endl << "> ";
    
    user_input = " ";
    cin >> user_input;
       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    
    TheCommands(commands);
    cout << endl;
    
    cout << "> In addition to these basic commands to see the directory system of the operating system, users are also able to navigate through the directory system" << endl;
    cout << "> The primary command for this is the cd command, which stands for change directory" << endl;
    cout << "> This command takes a valid path in the directory as an argument, and then navigates to the supplied directory" << endl;
    cout << "> To use this command, enter cd followed by a pathway" << endl;
    cout << "> For example: cd /home/stephen/Desktop" << endl;
    cout << "> Try entering this command!" << endl;
    
    cin >> user_input;
    
    return;
}

void TutorialStart(){
    string user_input = " ";
    
    cout << "> Welcome to the TutorialShell" << endl;
    cout << "> This program is meant to teach users that are unfamiliar with shells the basic commands and functions of a shell" << endl;
    cout << "> In a few simple words, shells are a way in which a user can interact with the services provided by the operating system " << endl;
    cout << "> This shell will present you with an interface that you can use to type commands" << endl;
    cout << endl;
    cout << "> To begin with, we'll start with some basic commands that will allow you to interact with the operating system" << endl;
    cout << "> Please enter the word Start to begin the tutorial" << endl;
    cout << "> If you would like to exit the tutorial, enter the command Exit at anytime" << endl;
    cout << "> ";
    cin >> user_input;
    cout << endl;
    
    if (user_input == "start" || user_input == "Start") {
        TutorialContinue();
        }
    else if (user_input == "Exit" || user_input == "exit"){
        return;}
    else{
        cout << "> Command not recognized" << endl;
    }
    
    return;       
}

