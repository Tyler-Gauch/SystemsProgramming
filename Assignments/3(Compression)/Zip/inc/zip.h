#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <bitset>
#include <stdlib.h>

#define MAX_CODES 4096
#define ON 1
#define OFF 0
#define DEBUG OFF

class Zip{
public:
	Zip(std::string fileName);
	~Zip();
	unsigned int initDictionary();
	void zip();
  void writeCodeWord(unsigned int byte1, unsigned int byte2, std::ofstream &outFile);
private:
	std::map<std::string, unsigned int> dictionary;
	std::ifstream * file;
	std::string fileName;
};