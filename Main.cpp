#include "Functions.h"
#include "CommandObject.h"
#include <iostream>
#include <queue>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdlib.h>

queue<CommandObject> commands;
string user_input;
CommandObject a_command;

int main()
{
    StartShell();

//    if(user_input == "tutorial" || user_input == "Tutorial"){
//        TutorialStart();}
//
//    else{
//        Shell();
//    }
    Shell();

    return 0;
}
