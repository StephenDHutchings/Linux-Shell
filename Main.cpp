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
char* modified_user_input;
CommandObject a_command;

int main()
{

    user_input = StartShell();
    
    a_command.main_command = user_input;
    
    commands.push(a_command);
    
    BuiltInCommands(commands);
    
    if(user_input == "tutorial"){ //|| "tutorial"){
        TutorialStart();}
    
    else{
        
    }

   
    return 0;
}
