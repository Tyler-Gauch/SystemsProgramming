#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;


int main(int argc, char ** argv)
{
	string input;
	string help = "--help";
	for(int i = 1; i < argc; i++)
	{
		if(!help.compare(argv[i]))
		{
			cout << "USAGE: cat [OPTIONS]... [FILE]..." << endl;
			cout << "Concatenate FILE(s) or standard input to standard output" << endl;
			cout << "With no FILE read standard input" << endl;
			cout << endl;
			cout << "--help\tShow this help dialogue" << endl;
			return 0;
		}
	}

	if(argc == 1)
	{
		while(getline(cin, input))
		{
			cout << input << endl;
		}
	}else
	{
		for(int i = 1; i < argc; i++)
		{
			ifstream file;
			file.open(argv[i], ios::in);
			struct stat s;
			if(file.good() && stat(argv[i], &s) == 0)
			{
				
				if(s.st_mode & S_IFREG)
				{
					while(getline(file, input))
					{
						cout << input << endl;
					}
				}else
				{
					cerr << "cat: '" << argv[i] << "': Is a directory" << endl;
				}
				file.close();
			}else
			{
				cerr << "cat: '" << argv[i] << "': No such file or directory" << endl;
			}
		}	
	}

	return 0;
}
