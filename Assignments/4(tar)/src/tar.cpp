#include <string>
#include <iostream>
#include <set>
#include <map>
#include <list>
#include <vector>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <utime.h>

void create(FILE *archive, string dir, set<ino_t>& inodes, bool verb);
void write_info(FILE *archive, string file, struct stat *info, set<ino_t>& inodes);

void extract(FILE *archive, bool verb);
bool read_info(FILE *archive, string& file, struct stat *info);

void usage();

struct fileType{
    struct stat info;
    string name;
};

void create(FILE *archive, string dir, set<ino_t>& inodes, bool verb)
{
  struct stat finfo;
  DIR *d;
  struct dirent *de;
  list<string> subdirs;
  list<string>::iterator subdir;

  if(lstat(dir.c_str(), &finfo) != 0)
  {
    fprintf(stderr, "%s: file not found\n", dir.c_str());
    return;
  }
  if(!S_ISDIR(finfo.st_mode))
  {
    fprintf(stderr, "%s: not a directory\n", dir.c_str());
    return;
  }
  inodes.insert(finfo.st_ino);
  if(verb) { fprintf(stderr, "%s: processing\n", dir.c_str()); }
  write_info(archive, dir, &finfo, inodes);

  d = opendir(dir.c_str());
  if(d == NULL) { perror(dir.c_str()); exit(1); }

  for(de = readdir(d); de != NULL; de = readdir(d))
  {
    if(strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) { continue; }

    string full_path = dir + "/" + de->d_name;
    if(lstat(full_path.c_str(), &finfo) != 0)
    {
      fprintf(stderr, "%s: file not found\n", full_path.c_str());
      continue;
    }

    if(S_ISDIR(finfo.st_mode))
    {
      subdirs.push_back(full_path);
    }
    else if(S_ISLNK(finfo.st_mode))
    {
      fprintf(stderr, "%s: skipping symbolic link\n", full_path.c_str());
    }
    else if(S_ISREG(finfo.st_mode))
    {
      if(verb) { fprintf(stderr, "%s: processing\n", full_path.c_str()); }
      cout << "RegFile" << endl;
      write_info(archive, full_path, &finfo, inodes);
    }
    else
    {
      fprintf(stderr, "%s: skipping unknown file type\n", full_path.c_str());
    }
  }
  closedir(d);
  
  for(subdir=subdirs.begin(); subdir!=subdirs.end(); subdir++)
  {
    create(archive, *subdir, inodes, verb);
  }
}

void write_info(FILE *archive, string file, struct stat *info, set<ino_t>& inodes)
{
  if(fprintf(archive, "%s\n", file.c_str()) != (int)file.length()+1)
  {

    perror("fprintf");
    exit(1);
  }
  if(fwrite(info, sizeof(struct stat), 1, archive) != 1)
  {
    perror("fwrite");
    exit(1);
  }
  if(inodes.find(info->st_ino) == inodes.end())
  {
    inodes.insert(info->st_ino);

    FILE *f = fopen(file.c_str(), "r");
    if(f == NULL) { perror("fopen"); exit(1); }

    char *buf = (char *)malloc(info->st_size * sizeof(char));

    if(fread(buf, sizeof(char), info->st_size, f) != (unsigned int)info->st_size)
    {
      perror("fread");
      exit(1);
    }
    fclose(f);

    if(fwrite(buf, sizeof(char), info->st_size, archive) != (unsigned int)info->st_size)
    {
      perror("fwrite");
      exit(1);
    }

    free(buf);
  }
}

void extract(FILE *archive, bool verb) {
  vector<fileType> dirs;
	string file;
	struct stat info;
	while(read_info(archive, file, &info))
	{
		if(file == "." || file == "..")
		{
			cerr << "Skipping |" << file << "|" << endl;
			continue;
		}
		if(verb) cout << "Processing " << file << "..." << endl;

		if(S_ISDIR(info.st_mode))
		{
      struct fileType ft;
      ft.info = info;
      ft.name = file;
      dirs.push_back(ft);
			if(mkdir(file.c_str(), info.st_mode) < 0){
				perror("Directory Creation Failed");
				exit(1);
			}
		}
		else if(S_ISREG(info.st_mode))
		{
			if(verb) cout << file << endl;
			char buf[info.st_size];
			if(fread(buf, sizeof(char), (unsigned int)info.st_size, archive) != (unsigned int)info.st_size)
			{
				perror("File Contents");
			}
			if(verb) cout << "\tRead File Contents" << endl;
			FILE * newF = fopen(file.c_str(), "w");
			if(newF == NULL)
			{
				perror("Couldn't create file: ");
				exit(1);
			}else{
				if(verb) cout << "\tCreated File" << endl;
				if(chmod(file.c_str(), info.st_mode) < 0){
					perror("File Permissions failed");
					exit(1);
				}
				if(fwrite(buf, sizeof(char), info.st_size, newF) != (unsigned int)info.st_size){
					perror("Couldn't write contents to file: ");
					exit(1);
				}
        fclose(newF);
        
        struct utimbuf time;
        time.actime = info.st_atime;
        time.modtime = info.st_mtime;
				if(utime(file.c_str(), &time) < 0)
				{
					perror("File time change failed");
					exit(1);
				}
				
				if(verb) cout << "\tContents written to file" << endl << endl;
			}
		}else{
			cout << "\tUnsupported file type" << endl;
		}
	}

  for(int i = dirs.size()-1; i > -1; i--)
  {
      if(chmod(dirs[i].name.c_str(), dirs[i].info.st_mode) < 0){
        perror("File Permissions failed");
        exit(1);
      }
      
      struct utimbuf time;
      time.actime = dirs[i].info.st_atime;
      time.modtime = dirs[i].info.st_mtime;

      if(utime(dirs[i].name.c_str(), &time)){
        perror("File time change failed");
        exit(1);
      }
  }
}

bool read_info(FILE *archive, string& file, struct stat *info)
{
  char buf[PATH_MAX];
  //read up to new line
  if(fscanf(archive, "%s\n", buf) != 1)
  {
    return false;
  }
  file = buf;
  //read in stat info
  if(fread(info, sizeof(struct stat), 1, archive) != 1)
  {
    perror("fread2");
    exit(1);
  }
  return true;
}

void usage()
{
  cerr << "usage to create an archive:  tar c[v] ARCHIVE DIRECTORY" << endl;
  cerr << "usage to extract an archive: tar x[v] ARCHIVE" << endl << endl;
  cerr << "  the v parameter is optional, and if given then every file name" << endl;
  cerr << "  will be printed as the file is processed" << endl;
  exit(1);
}

int main(int argc, char **argv)
{
  bool verbose;
  FILE *arch;

  if(argc < 3 || argc > 4) { usage(); } 
  
  if(argc == 4)
  { 
    if(strcmp(argv[1],"c") == 0) { verbose = false; }
    else if(strcmp(argv[1],"cv") == 0) { verbose = true; }
    else { usage(); }

    string archive = argv[2];
    string dir_name = argv[3];
  
    while(dir_name.at(dir_name.length()-1) == '/')
    {
      dir_name = dir_name.erase(dir_name.length()-1,1);
    }
    size_t dslash = dir_name.find("//");
    while(dslash != string::npos)
    {
      dir_name = dir_name.erase(dslash,1);
      dslash = dir_name.find("//");
    }

    if(archive == "-") { arch = stdout; }
    else
    {
      arch = fopen(archive.c_str(), "w");
      if(arch == NULL) { perror(archive.c_str()); exit(1); }
    }
    
    set<ino_t> inodes;
    create(arch, dir_name, inodes, verbose);
  }
  else if(argc == 3)
  { 
    if(strcmp(argv[1],"x") == 0) { verbose = false; }
    else if(strcmp(argv[1],"xv") == 0) { verbose = true; }
    else { usage(); }

    string archive = argv[2];

    if(archive == "-") { arch = stdin; }
    else
    {
      arch = fopen(archive.c_str(), "r");
      if(arch == NULL) { perror(archive.c_str()); exit(1); }
    }
    
    extract(arch, verbose);
  }

  return 0;
}