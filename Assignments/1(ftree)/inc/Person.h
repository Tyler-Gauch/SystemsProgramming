#ifndef __PERSON__
#define __PERSON__

#include <iostream>
#include <string>
#include <vector>

class Person{

public:
	Person();
	Person(std::string firstName);
	std::string getFirstName();
	void setFirstName(std::string firstName);
	std::string getLastName();
	void setLastName(std::string lastName);
	std::string getFullName();
	std::string getSex();
	void setSex(std::string sex);
	Person* getMother();
	void setMother(Person* mother);
	Person* getFather();
	void setFather(Person* father);
	void print();
	void addChild(Person * child);
	std::vector<Person*> getChildren();
	bool isMale();
	bool isFemale();
private:
	std::string firstName;
	std::string lastName;
	std::string sex;
	Person * mother;
	Person * father;
	std::vector<Person*> children;
};


#endif
