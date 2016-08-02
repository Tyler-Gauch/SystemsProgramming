#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char ** argv){

	bool countLines = false;
	bool countWords = false;
	bool countChars = false;
	vector<FILE*> files;
	vector<string> fileNames;

	for(int i = 1; i < argc; i++)
	{
		string in(argv[i]);
		if(in == "-l")
		{
			countLines = true;
		}else if(in == "-c")
		{
			countChars = true;
		}else if(in == "-w")
		{
			countWords = true;
		}else if(in == "-h"){
			puts("USAGE: wc [-OPTIONS..] [FILES..]");
			puts("");
			puts("-l : count lines");
			puts("-w : count words");
			puts("-c : count characters");
			puts("-  : read stdin");
			exit(0);
		}else{
			if(in != "-"){
				FILE * file;
				if((file = fopen(argv[i], "r")) != NULL){
					files.push_back(file);
					fileNames.push_back(in);
				}else{
					fputs("Couldn't open file", stderr);
					exit(-3);
				}
			}
			
		}
	}

	if(!countLines && !countWords && !countChars)
	{
		countLines = true;
		countWords = true;
		countChars = true;
	}

	int totalWords = 0;
	int totalChars = 0;
	int totalLines = 0;

	if(files.size() == 0)
	{
		int words = 0;
		int chars = 0;
		int lines = 0;
		int result;
		char * buffer;
		buffer = (char*) malloc (sizeof(char)*100);

		//read stdin
		// copy the file into the buffer:
		result = fread (buffer,sizeof(char),100,stdin);
		for(int j = 0; j < result; j++)
		{
			switch(buffer[j])
			{
				case '\n':
					if(countLines)
						lines++;
				case '\r':
				case '\t':
				case ' ':
				case '\v':
				case '\f':
					if(countWords && j != 0 && !isspace(buffer[j-1]))
					{
						words++;
					}
				default:
					if(countChars)
						chars++;
					if(j+1 == result && countWords && !isspace(buffer[j]))
					{	
						printf("Found last word\n");
						words++;
					}
					break;
			}
		}

		// if(countWords && !isspace(buffer[result-1]))
		// {	
		// 	words++;
		// }

		totalWords += words;
		totalLines += lines;
		totalChars += chars;

		if(countLines)
		{
			printf("%d", lines);
			if(!countWords)
			{
				printf("\n");
			}else{
				printf("\t");
			}
		}
		if(countWords)
		{
			printf("%d", words);
			if(!countChars)
			{
				printf("\n");
			}else{
				printf("\t");
			}
		}
		if(countChars)
		{
			printf("%d\n", chars);
		}
	}

	for(unsigned i = 0; i < files.size(); i++)
	{
		int words = 0;
		int chars = 0;
		int lines = 0;
		int result;

		fseek(files[i], 0, SEEK_END);
		long fileSize = ftell(files[i]);
		rewind(files[i]);
		char * buffer;

		buffer = (char*) malloc (sizeof(char)*fileSize);
		if (buffer == NULL) 
		{
			fputs ("Memory error",stderr); 
			exit (-1);
		}

		// copy the file into the buffer:
		result = fread (buffer,sizeof(char),fileSize,files[i]);
		for(int j = 0; j < result; j++)
		{
			switch(buffer[j])
			{
				case '\n':
					if(countLines)
						lines++;
				case '\r':
				case '\t':
				case ' ':
				case '\v':
				case '\f':
					if(countWords && j != 0 && !isspace(buffer[j-1]))
					{
						words++;
					}
				default:
					if(countChars)
						chars++;
					
					break;
			}
		}

		if(countWords && !isspace(buffer[result-1]))
		{	
			words++;
		}

		totalWords += words;
		totalLines += lines;
		totalChars += chars;

		if(countLines)
		{
			printf("\t%d\t", lines);
		}
		if(countWords)
		{
			printf("%d\t", words);
		}
		if(countChars)
		{
			printf("%d\t", chars);
		}
		printf("%s\n", fileNames[i].c_str());
	}

	if(files.size() > 1)
	{
		if(countLines)
		{
			printf("\t%d\t", totalLines);
		}
		if(countWords)
		{
			printf("%d\t", totalWords);
		}
		if(countChars)
		{
			printf("%d\t", totalChars);
		}
		printf("total\n");
	}

}

