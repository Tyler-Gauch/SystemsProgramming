#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <bitset>
#include <vector>
#include <stdlib.h>

#define MAX_CODES 4096
#define ON 1
#define OFF 0
#define DEBUG OFF
/*
Initialize Dictionary to hold first 256 codes
CurrentString = first input character
While there are more input characters
  CurrentChar = next input character
  If CurrentString+CurrentChar is in Dictionary
    CurrentString = CurrentString+CurrentChar
  Else
    CodeWord = CurrentString's code in Dictionary
    Output CodeWord
    Add CurrentString+CurrentChar to Dictionary
    CurrentString = CurrentChar
  End If
End While
CodeWord = CurrentString's code in Dictionary
Output CodeWord
*/

class Unzip{
public:
	Unzip(std::string fileName);
	~Unzip();
	unsigned int initDictionary();
	void unzip();
  void getCodeWords();
private:
	std::map<unsigned int, std::string> dictionary;
	std::ifstream * file;
  std::string fileName;
  std::vector<unsigned int> codeWords;
};