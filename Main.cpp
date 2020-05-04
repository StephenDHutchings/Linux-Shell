#include "Functions.h"
#include "CommandObject.h"
#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

int main()
{
    InputParser("test 1|test2 -a -f|test3 > blah|test4 -1 < yaa");
    return 0;
}
