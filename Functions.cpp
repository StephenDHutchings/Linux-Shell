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
#include <array>
#include <fcntl.h>

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

string FindBinary(const char* command) {
    /*
     Finds corresponding binary to a user's command input
     
     written with help of: https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
     
     inputs:
     command: the main command portion of a user's input
     
     output:
     file_path: string with the path to the binary to execute
     */
    string file_path;
    array<char, 128> buffer;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    
    if (!pipe) {
        throw runtime_error("popen() failed. Can't get binary location.");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        file_path += buffer.data();
    }
    
    file_path.erase(remove(file_path.begin(), file_path.end(), '\n'), file_path.end());
    return file_path;
}

void TheCommands(queue<CommandObject>& commands){
    /*
     Input: A queue of CommandObjects
     * 
     Ouput: Executes a command, and implements forking for that command
     */
    
    pid_t child_pid, pipe_child_pid; //process id for the child process that will execute the command
    CommandObject a_command;
    a_command = commands.front();
    commands.pop();
    
    // Preparing for pipes
    bool to_pipe = !commands.empty();
    int pipefd[2];
    
    //to convert the command to a char to be passed to execve
    char* args[a_command.arguments.size() + 1];
    
    for (int i = 0; i < a_command.arguments.size(); i++) {
        args[i] = (char*) a_command.arguments[i].c_str();
    }
    
    args[a_command.arguments.size()] = NULL;
    
    if(a_command.main_command == "/usr/bin/cd"){
        chdir(args[1]);
        return;
    }
    
    if(a_command.main_command == "/usr/bin/which") { // special handling for which command, execve doesn't play nice
        stringstream which_command;
        which_command << "which";
        for (int i = 1; i < a_command.arguments.size(); i++) {
            which_command << " " << a_command.arguments[i];
        }
        char which_array[which_command.str().length()];
        strcpy(which_array, which_command.str().c_str());
        system((char *) which_array);
    }
    
    //forking the process
    child_pid = fork();
    
    //error handling
    if(child_pid < 0){
        cout << "Forking failed" << endl;
    }
    
    //when the fork is successful
    else if(child_pid == 0) {
        if (to_pipe) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        } else if (a_command.output_to_file || a_command.input_from_file) { // redirecting output to file
            if (a_command.input_from_file) { // read from file, <
                int fd;
                fd = open(a_command.filename.c_str(), O_RDONLY); // Setting input from file
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else { // output to file, >
                int fd;
                fd = open(a_command.filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // Setting output to file
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
        if(execve(a_command.main_command.c_str(), args, NULL) < 0)
        {cout << "Could not execute command" << endl;}
    }
    
    else {
        if (to_pipe) {
            pipe_child_pid = fork();
            if (pipe_child_pid == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                
                // Executing
                CommandObject pipe_command = commands.front();
                char* pipe_args[pipe_command.arguments.size() + 1];
                
                for (int i = 0; i < pipe_command.arguments.size(); i++) {
                    pipe_args[i] = (char*) pipe_command.arguments[i].c_str();
                }
                pipe_args[pipe_command.arguments.size()] = NULL;
                
                if (execve(pipe_command.main_command.c_str(), pipe_args, NULL) < 0) {
                    cout << "Piped command could not execute" << endl;
                }
            } else {
                wait(NULL);
            }
        }
        wait(NULL);
        return;
    }
}

void StartShell(){
    string user_input = " ";
    
    cout << "> Welcome to TutorialShell!" << endl;
    cout << "> You may use this Shell as a learning tool by entering the command Tutorial" << endl;
    cout << "> Otherwise to use this program as a typical Linux shell, enter the command start" << endl;
    cout << "> Enter the command exit at anytime to quit" << endl;
    cout << "> To view a helpful list of basic commands, enter help within the shell" << endl;
    /*cout << "> ";
    
    getline(cin, user_input);
    cout << endl;
    return user_input; */
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
        if (user_input != "tutorial" && user_input!= "Tutorial") {
            if (user_input != "exit" && user_input != "Exit" && user_input != ""){
                
                if(user_input == "help" || user_input == "Help"){
                    HelpCommand();
                }
                
                else{
                    commands = InputParser(user_input);
                    //                a_command = commands.front();
                    TheCommands(commands);
                }
            } else if(user_input == "") {
                cin.clear();
                continue;
            }
            
            else {
                done = true;
            }
        } else {
            TutorialStart();
        }
    }
    
    return;
}

void TutorialContinue(){
    /*
     * This function is the meat and potatoes of the tutorial
     * It demonstrates and has the user go through basic commands to understand them 
     */
    
    CommandObject a_command;
    queue<CommandObject> commands;
    string user_input = " ";
    
    cout << "> This tutorial is aimed at beginners with little to no experience working in a Linux environment" << endl;
    cout << "> By the end of this tutorial you should: " << endl;
    cout << "> 1) Be more comfortable working in a Linux shell" << endl;
    cout << "> 2) Have a basic knowledge and understanding of commands" << endl;
    cout << "> 3) Have a deeper knowledge and understanding of the Linux OS and how to interact with it" << endl;
    cout << endl << endl;
    
    cout << "> To begin, lets see who you are" << endl;
    cout << "> The command whoami will show the user that is currently logged in to this device" << endl;
    cout << "> Lets try it out" << endl << "> ";
    
    getline(cin, user_input);
 
    while(user_input != "whoami"){
        cout << "> Please enter the command whoami" << endl << "> ";
        getline(cin, user_input);}
     
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl;
    cout << "> One of the first basic commands is to print the current working directory" << endl;
    cout << "> The current working directory is the location in the file hierarchy system that the user is currently in" << endl;
    cout << "> This has implications for other commands that are entered into the shell" << endl;
    cout << "> For example, if a user is interested in running a program or application they must be located in the directory that holds that program" << endl;
    cout << "> To see the current directory, enter the command pwd" << endl << "> ";
    
    getline(cin, user_input);
 
    while(user_input != "pwd"){
        cout << "> Please enter the command pwd" << endl << "> ";
        getline(cin, user_input);}
     
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
    cout << "> Or the path from which to list the files" << endl;
    cout << "> This tutorial will show the basic command, as well as an example with arguments" << endl;
    cout << "> To see the files and subdirectories of the current directory, enter the command ls" << endl << "> ";
    
    user_input = "";
    
    getline(cin, user_input);
    
    while(user_input != "ls"){
    cout << "> Please enter the command ls" << endl << "> ";
    getline(cin, user_input);}

       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl << endl;
    cout << "> Now we'll add some arguments to the ls command" << endl;
    cout << "> In a Linux operating system, arguments are used to augment or modify the base command" << endl;
    cout << "> For the ls command, we'll add the following argument of -l" << endl;
    cout << "> This will make each file or subdirectory of the current directory be listed on its own line" << endl;
    cout << "> This will also provide detailed information regarding the contents "  << endl;
    cout << "> This includes: the unix file type, permissions, the number of hard links, the owner," << endl;
    cout << "> The group, size, last-modified date, and the filename " << endl;
    cout << "> The amount of information in these files may be overwhelming - don't focus on that!" << endl;
    cout << "> The main take away of this is that commands can be modified" << endl;
    cout << "> Try it out!" << endl;
    cout << "> Enter the command as: ls -l" << endl;
    cout << "> ";
    
    user_input = " ";
    
    getline(cin, user_input);
    
    while(user_input != "ls -l"){
    cout << "> Please enter the command ls -l" << endl << "> ";
    getline(cin, user_input);}

       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl << endl;
    cout << "> Commands can be given more than one argument, to see the previous command but with the the contents in reverse order" << endl;
    cout << "> Enter the command: ls -l -r" << endl;
    cout << "> ";
    
    user_input = " ";
    
    getline(cin, user_input);
    
    while(user_input != "ls -l -r"){
    cout << "> Please enter the command ls -l -r" << endl << "> ";
    getline(cin, user_input);}

       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl << endl;
    cout << "> By using the ls command to visualize in the directory, it can allow you to better manipulate the directory" << endl;
    cout << "> We'll try this out through the touch command" << endl;
    cout << "> Entering the touch command followed by a file name and extension will create that file in the directory" << endl;
    cout << "> Try creating a text file called sample.txt in the current directory" << endl;
    cout << "> The command will be: touch sample.txt" << endl << "> ";
        
    user_input = " ";
    
    getline(cin, user_input);
    
    while(user_input != "touch sample.txt"){
    cout << "> Please enter the command touch sample.txt" << endl << "> ";
    getline(cin, user_input);}

       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl << "> Now verify its there with the ls command" << endl;
    
    user_input = " ";
    
    getline(cin, user_input);
    
    while(user_input != "ls"){
    cout << "> Please enter the command ls" << endl << "> ";
    getline(cin, user_input);}

       
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl;
    cout << "> In addition to these basic commands to see the directory system of the operating system" << endl;
    cout << "> Users are also able to navigate through the directory system" << endl;
    cout << "> The primary command for this is the cd command, which stands for change directory" << endl;
    cout << "> This command takes a valid path in the directory as an argument, and then navigates to the supplied directory" << endl;
    cout << "> To use this command, enter cd followed by a pathway" << endl;
    cout << "> For example: cd /home/stephen/Desktop" << endl;
    cout << "> Try entering this command!" << endl;
    cout << "> ";
    
    user_input = " ";
    getline(cin, user_input);

    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    cout << endl;
    cout << "> Now, enter the pwd command to make sure your directory location was successfully changed" << endl;
    cout << "> ";
    
    user_input = " ";
        getline(cin, user_input);
 
    while(user_input != "pwd"){
        cout << "> Please enter the command pwd" << endl << "> ";
        getline(cin, user_input);}
     
    if(user_input == "exit")
    {
        return;
    }
    
    commands = InputParser(user_input);
    TheCommands(commands);
    
    user_input = " ";
    
    cout << endl;
    cout << "> You have reached the end of this tutorial" << endl;
    cout << "> Although this was meant to be a simple overview of a Linux shell" << endl;
    cout << "> Hopefully you have learned more about commands and how to interact with a Linux OS" << endl;
    cout << "> Through the command-line interface" << endl;
    cout << "> The next step is to play around in this program" << endl;
    cout << "> Explore the additional commands that Linux supports" << endl;
    cout << "> Refer to the help menu to gain additional information about commands" << endl << endl;
    
    Shell();
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
    
    getline(cin, user_input);
    
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

void HelpCommand(){
    
    cout << "> This is not an exhaustive list of commands, but a good starting point for beginners" << endl;
    cout << "> The list is formatted in the following way: " << endl;
    cout << "> Command <Necessary Arguments> [Optional Arguments]: Description" << endl << endl;
    cout << "> pwd: Prints the current directory" << endl;
    cout << "> ls [-1 -t -S -l -a]: Lists the files in the current directory, arguments modify how they're listed" << endl;
    cout << "> mkdir <new_directory_name>: Creates a new directory called <new_directory_name> in the current directory" << endl;
    cout << "> rmdir <a_directory>: Removes <a_directory> within the current directory, but it must be empty of other files or sub-directories" << endl;
    cout << "> touch <a_file.extension>: Create a <a_file.extension> within the current directory" << endl;
    cout << "> rm <a_file>: Removes <a_file> within the current directory" << endl;
    cout << "> mv <a_file> <a_directory>: Moves <a_file> to the <a_directory>" << endl;
    cout << "> mv <a_file> <renamed_file>: Renames <a_file> to <renamed_file> within the directory" << endl;
    cout << "> du: Shows the disk usage of the machine" << endl;
    cout << "> man <a_command>: To see the full manual for any Linux supported command" << endl;
    
    return;
}

