//
//  CommandObject.h
//  Linux-Shell
//
//  Created by Mohamed Abdel-Hafiz on 3/13/20.
//  Copyright © 2020 Mohamed Abdel-Hafiz. All rights reserved.
//

#ifndef CommandObject_h
#define CommandObject_h

#include <vector>
#include <string>

using namespace std;

struct CommandObject {
    string main_command;
    vector<string> arguments;
    bool output_to_file;
    
    // Constructors
    CommandObject();
    CommandObject(string input_command);
};

#endif /* CommandObject_h */
