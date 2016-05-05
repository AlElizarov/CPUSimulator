#ifndef mycomp
#define mycomp

#include <string>
#include <vector>

std::string inputProgramForCompile();
std::vector<std::string> inputProgrammsForExecute();
void validatePath(std::string*, std::ifstream&, std::string&, std::string);
void complileMode();
void executeMode();
char choiceMode();

#endif
