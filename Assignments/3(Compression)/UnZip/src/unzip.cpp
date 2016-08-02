#include "../inc/unzip.h"

Unzip::Unzip(std::string fileName){
	this->fileName = fileName;
	if(fileName.substr(fileName.length() - 4) != ".zip")
	{
		std::cerr << "Error: Invalid file type.  Must be a '.zip'" << std::endl;
		exit(-1);
	}
	file = new std::ifstream(fileName.c_str(), std::ios::binary);
	if(!file->is_open())
	{
		std::cerr << "Error: Couldn't open file '" << fileName << "'" << std::endl;
		exit(-2);
	}
}

Unzip::~Unzip(){
	file->close();
}

unsigned int Unzip::initDictionary(){
	unsigned int next_code;
	for(next_code = 0; next_code < 256; next_code++)
	{
		std::string tmp;
		tmp = (char)next_code;
		dictionary[next_code] = tmp;
	}
	return next_code;
}

void Unzip::unzip(){

	unsigned int lastCode = initDictionary();

	getCodeWords();

	//open the output file
	std::string outputFileName = this->fileName;
	outputFileName = this->fileName.substr(0,this->fileName.length()-4);
	std::ofstream outFile(outputFileName.c_str());

	unsigned int currentCodeWord, nextCodeWord;
	std::string currentString = "", oldString = "", tmpNext = "", tmpCurrent = "", currentCodeChar, nextCodeChar;

	currentCodeWord = this->codeWords[0];
	this->codeWords.erase(this->codeWords.begin());

	std::string tmp = "";
	tmp = currentCodeWord;
	std::string currentChar = dictionary[currentCodeWord];

	if(DEBUG == ON)
	{
		std::cout << "First Output: " << currentChar << std::endl;
	}

	outFile << currentChar;
	
	while(codeWords.size() > 0)
	{
		// std::map<unsigned int, std::string>::iterator it;
		// for(it = dictionary.begin(); it != dictionary.end(); it++)
		// {
		// 	std::cout << "dictionary[" << it->first << "] = " << it->second << std::endl;
		// }

		nextCodeWord = this->codeWords[0];
		this->codeWords.erase(this->codeWords.begin());

		if(DEBUG == ON)
		{
			std::cout << "Current: " << currentCodeWord << " Next: " << nextCodeWord << " tmpNext: " << tmpNext << " tmpCurrent: " << tmpCurrent << std::endl;
		}

		if(dictionary.find(nextCodeWord) == dictionary.end())
		{
			currentString = dictionary[currentCodeWord];
			currentString += currentChar;
		}else{
			currentString = dictionary[nextCodeWord];
		}
		outFile << currentString;
		if(DEBUG == ON)
		{
			std::cout << "OUTPUT: "  << currentString << std::endl;
		}

		currentChar = currentString[0];
		oldString = dictionary[currentCodeWord];

		if(lastCode != MAX_CODES)
		{
			dictionary[lastCode++] = oldString+currentChar;
		}

		currentCodeWord = nextCodeWord;
		if(DEBUG == ON)
		{
			std::cout << "currentChar: " << currentChar << " oldString: " << oldString << " New Entry: " << dictionary[lastCode-1] << " currentCodeWord: " << currentCodeWord << std::endl;
		}
	}
	outFile.close();
}

void get12Bits(unsigned int &in)
{
	for(int i = 12; i < 32; i++)
	{
		in &= ~(1 << i);
	}
}

void Unzip::getCodeWords(){
	//to get the next code word we need to read 3 bytes

	char c;
	std::string totalFile;
	while(file->read(&c, 1))
	{
		totalFile += c;
	}

	for(unsigned int i = 0; i < totalFile.length(); i++)
	{
		char byte1, byte2, byte3;
		byte1 = totalFile[i++];
		byte2 = totalFile[i++];
		byte3 = totalFile[i];

		int inputCombined = ((byte1 & 0x0000FF) << 16) | ((byte2 & 0x00FF) << 8) | (byte3 & 0xFF);

		unsigned int codeWordA = 0, codeWordB = 0;
		codeWordA = inputCombined >> 12;
		codeWordB = inputCombined & ((1 << 12) -1);

		if(DEBUG == ON)
		{
			std::cout << std::bitset<24>((byte1 & 0x0000FF) << 16).to_string() << " " << std::bitset<24>((byte2 & 0x00FF) << 8).to_string() << " " << std::bitset<24>(byte3 & 0xFF).to_string() << std::endl;
			std::cout << "Byte1: " << std::bitset<8>(byte1).to_string() << " Byte2: " << std::bitset<8>(byte2).to_string() << " Byte3: " << std::bitset<8>(byte3).to_string() << " inputCombined: " << std::bitset<24>(inputCombined).to_string();
			std::cout << " CodeWordA: " << std::bitset<24>(codeWordA).to_string() << " CodeWordB: " << std::bitset<24>(codeWordB).to_string() << std::endl;
		}
		if((codeWordA & 0xFFF) != 0xFFF)
		{
			codeWords.push_back(codeWordA);
		}
		
		if((codeWordB & 0xFFF) != 0xFFF)
		{
			codeWords.push_back(codeWordB);
		}

	}

	if(DEBUG == ON)
	{
		std::cout << "Code Words: ";
		for(unsigned int i = 0; i < codeWords.size(); i++)
		{
			std::cout << codeWords[i] << " ";
		}
		std::cout << std::endl;
	}	
}
