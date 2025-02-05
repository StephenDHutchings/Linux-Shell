#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include <queue>
#include "CommandObject.h"

using namespace std;

queue<string> PipeSeparator(string input_string); // Splits piped inputs into separate commands
queue<CommandObject> InputParser(string typed_string); // Parses inputs, outputing a series of command objects to execute
string FindBinary(const char* command); // Finds corresponding binary to a user's command input
void TheCommands(queue<CommandObject>& user_input);
void StartShell();
void TutorialStart();
void TutorialContinue();
void Shell();
void HelpCommand();

#endif
