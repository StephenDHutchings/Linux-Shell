#include "Functions.h"
#include "CommandObject.h"
#include <iostream>
#include <queue>
#include <string>
#include <unistd.h>

queue<CommandObject> commands;
string user_input;
char* modified_user_input;

int main()
{

    user_input = StartShell();
    
    if(user_input == "tutorial"){ //|| "tutorial"){
        TutorialStart(); }
    
    else{
        
    }

   
    return 0;
}
