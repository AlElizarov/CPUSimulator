#ifndef computer
#define computer

#include <string>

using namespace std;

string inputProgramForCompile();
vector<string> inputProgrammsForExecute();
void validatePath(string*, ifstream&, string&, string);
void complileMode();
void executeMode();
char choiceMode();

#endif
