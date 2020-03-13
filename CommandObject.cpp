//
//  CommandObject.cpp
//  Linux-Shell
//
//  Created by Mohamed Abdel-Hafiz on 3/13/20.
//  Copyright © 2020 Mohamed Abdel-Hafiz. All rights reserved.
//

#include <stdio.h>
#include "CommandObject.h"

#include <vector>
#include <string>

CommandObject::CommandObject() {
    main_command = "";
    arguments = vector<string>();
    output_to_file = false;
}

CommandObject::CommandObject(string input_command) {
    // TODO
}
