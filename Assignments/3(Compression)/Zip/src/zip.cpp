#include "../inc/zip.h"

//Store the filename and attempt to open the file.
Zip::Zip(std::string fileName){
	this->fileName = fileName;
	file = new std::ifstream(fileName.c_str());
	if(!file->is_open())
	{
		std::cout << "Error: Couldn't open file '" << fileName << "'" << std::endl;
		exit(-1);
	}
}

Zip::~Zip(){
	file->close();
}

//Initialize the Dictionary with the first 256 ascii key words
unsigned int Zip::initDictionary(){
	unsigned int next_code;
	for(next_code = 0; next_code < 256; next_code++)
	{
		std::string tmp;
		tmp = (char)next_code;
		dictionary[tmp] = next_code;
	}

	return next_code;
}

//splot two 12 bit code words int 3 bytes and write them to the output file 
void Zip::writeCodeWord(unsigned int byte1, unsigned int byte2, std::ofstream &outFile){
	int outputCombined = 0;
	outputCombined = (byte1 << 12) | (byte2);
	char output1 = (outputCombined >> 16);
	char output2 = (outputCombined >> 8);
	char output3 = (outputCombined >> 0);

	outFile << output1;
	outFile << output2;
	outFile << output3;
	
	std::cout << "Byte1: " << byte1 << " Byte2: " << byte2 << " out1: " << (unsigned int)output1 << " out2: " << (unsigned int)output2 << " out3: " << (unsigned int)output3 << std::endl; 

}

//compress the input file given using the LZW compression algorithm
void Zip::zip(){

	//initialize the dictionary
	unsigned int lastCode = initDictionary();

	//read in the file contents into memory
	//This is pretty inefficient for large files
	//but I am to lazy to be efficient right now
	char c;
	std::string totalFile;
	while(file->read(&c, 1))
	{
		totalFile += c;
	}

	//open the output file using the current filename and .zip
	std::string outputFileName = this->fileName;
	outputFileName += ".zip";
	std::ofstream outFile(outputFileName.c_str());

	//compress the file with LZW
	unsigned int i = 0;
	std::string currentString = "";
	currentString += totalFile[i];
	
	int write = 0;
	unsigned int byte1;
	unsigned int byte2;

	for(i = 1; i < totalFile.length(); i++)
	{
		char currentChar = totalFile[i];
		//if we have the string in the dictionary dont write it yet
		if(dictionary.find(currentString + currentChar) != dictionary.end())
		{
			currentString += currentChar;
		}else{
			//if we didn't have the string in the dictionary then 
			//we write the last know string that was in the dictionary
			unsigned int codeWord = dictionary[currentString];

			//check if we need to write or store the value
			//we need to write using 2 bytes at a time
			if(write == 0)
			{
				byte1 = codeWord;
				write++;
			}else if(write == 1)
			{
				write = 0;
				byte2 = codeWord;
				//now we write becuase we have 2 bytes
				writeCodeWord(byte1, byte2, outFile);
			}

			//if we have maxed out the number of codes allowed then stop
			if(lastCode != MAX_CODES)
			{
				dictionary[currentString+currentChar] = lastCode++;
			}
			currentString = currentChar;
		}
	}

	unsigned int codeWord = dictionary[currentString];
	//if we have an even number of bytes just write
	if(write == 1)
	{
		writeCodeWord(byte1, codeWord, outFile);
	}else{
		//if we had an odd number of bytes append 0xFFF to the end of the file
		writeCodeWord(codeWord, 0xFFF, outFile);
	}

	outFile.close();
}