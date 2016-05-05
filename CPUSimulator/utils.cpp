#include <string>
#include "Utils.h"

using namespace std;

string cutOffSubstrFromStr(string& str, char character) {
	int beforeDot = 0;
	for (char c : str)
	{
		if (c == character)
			break;
		beforeDot++;
	}
	return str.substr(0, beforeDot);
}

int leftShift(int shifted, int shiftValue) {
	int result = shifted;
	for (int i = 0; i < shiftValue; i++) {
		result = result << 1;
	}
	return result;
}

string reverse(string str)
{
	string reversed = str;
	int tmp;

	for (int i = 0, j = reversed.length() - 1; i < j; i++, j--)
	{
		tmp = reversed[i];
		reversed[i] = reversed[j];
		reversed[j] = tmp;
	}
	return reversed;
}

string myTrim(string str)
{
	string trimmed = ltrim(str);
	trimmed = reverse(trimmed);
	trimmed = ltrim(trimmed);
	trimmed = reverse(trimmed);
	return trimmed;
}

string ltrim(string str) {
	int afterSpaces = 0;
	for (char c : str)
	{
		if (!isspace(c))
			break;
		afterSpaces++;
	}
	return str.substr(afterSpaces, (str.length() - afterSpaces));
}

bool findCharInString(string& str, char character) {
	for (char character : str) {
		if (character == character)
			return true;
	}
	return false;
}