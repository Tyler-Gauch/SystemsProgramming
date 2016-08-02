#include "../inc/zip.h"
#include "../inc/main.h"

int main(int argc, char * argv[]){
	
	if(argc != 2){
		std::cerr << "Invalid arguments" << std::endl;
		std::cerr << "USAGE: " << argv[0] << " <FILE>" << std::endl;
		return -1;
	}

	std::string fileName(argv[1]);

	Zip zip(fileName);

	zip.zip();

	return 0;
}