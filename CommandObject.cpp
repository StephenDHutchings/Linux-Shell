//
//  CommandObject.cpp
//  Linux-Shell
//
//  Created by Mohamed Abdel-Hafiz on 3/13/20.
//  Copyright © 2020 Mohamed Abdel-Hafiz. All rights reserved.
//

#include <stdio.h>
#include "CommandObject.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <filesystem>

CommandObject::CommandObject() {
    main_command = "";
    arguments = vector<string>();
    input_from_file = false;
    output_to_file = false;
    filename = "";
}

CommandObject::CommandObject(string input_command) : CommandObject() {
    /*
     Creates the CommandObject from a given input string representing a full command (command and arguments) as it would be typed in the terminal.
     
     inputs:
     input_command: a string representing a command + arguments, as it would be typed in the terminal.
     
     output:
     None. Populates CommandObject fields.
     */
    
    // Checking for file inputs and outputs (<, >)
    size_t split_index = input_command.find_first_of("<>");
    stringstream partial_string;
    
    if (split_index != string::npos) { // Found an instance of < or >
        // Storing file name
        filename = input_command.substr(split_index + 1, input_command.length() - split_index + 1);
        
        // Setting proper input/ouput flag
        if (input_command[split_index] == '>') {
            output_to_file = true;
        } else if (input_command[split_index] == '<') {
            input_from_file = true;
        }
        
        // Removing filename portion of input string
        input_command = input_command.substr(0, split_index - 1);
    }
    
    // Split remaining command string into main command and arguments
    istringstream temporary_stream(input_command);
    vector<string> input_words((istream_iterator<string>(temporary_stream)), istream_iterator<string>());
    // ^^^^ Vector of individual words ^^^^
    arguments = input_words; // execve expects the command + arguments in the vector, not just the arguments. No point in removing the first word.
    
    // Building main command absolute filepath
    if (arguments[0].substr(0, 2) == "./") { // command referring to executable in working directory
        string working_directory = __fs::filesystem::current_path();
        stringstream filepath_stream;
        
        working_directory = working_directory.substr(1, working_directory.length() - 2);
        filepath_stream << working_directory << arguments[0].substr(1, arguments[0].length() - 1);
        main_command = filepath_stream.str();
    } else { // built-in, use /bin/ for base path
        stringstream filepath_stream;
        
        filepath_stream << "/bin/" << arguments[0];
        main_command = filepath_stream.str();
    }
}
