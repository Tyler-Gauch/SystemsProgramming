#include <iostream>
#include "../inc/Person.h"
#include <map>

bool checkChildren(std::string name, Person * parent);

int main(int argc, char ** argv){
	
	std::map<std::string, Person> People;
	std::string keyword;
	std::string lastPerson;

	while(std::cin >> keyword){
		if(keyword == "PERSON")
		{
			std::cin >> lastPerson;
			People[lastPerson].setFirstName(lastPerson);
		}else if(keyword == "FATHER_OF"){
			std::string name;
			std::cin >> name;
			if(People[lastPerson].getSex() == "" || People[lastPerson].isMale())
			{
				if(People[name].getFather() == NULL){
						People[name].setFirstName(name);
						People[name].setFather(&People[lastPerson]);
						People[lastPerson].addChild(&People[name]);
						People[lastPerson].setSex("M");
				}
				else if(People[name].getFather()->getFirstName() != lastPerson)
					std::cerr << name << " already has a father" << std::endl;
			}else
			{
				std::cerr << lastPerson << " is defined as a Female and cannot be a father" << std::endl;
			}

		}else if(keyword == "MOTHER_OF"){
			std::string name;
			std::cin >> name;
			if(People[lastPerson].getSex() == "" || People[lastPerson].isFemale())
				{
				if(People[name].getMother() == NULL){
					People[name].setFirstName(name);
					People[name].setMother(&People[lastPerson]);
					People[lastPerson].addChild(&People[name]);
					People[lastPerson].setSex("F");
				}
				else if(People[name].getMother()->getFirstName() != lastPerson)
					std::cerr << name << " already has a mother" << std::endl;
			}else{
				std::cerr << lastPerson << " is defined as a Male and cannot be a mother" << std::endl;
			}
		}else if(keyword == "SEX")
		{
			std::string sex;
			std::cin >> sex;
			People[lastPerson].setSex(sex);
		}else if(keyword == "SON")
		{
			std::cin >> lastPerson;
			People[lastPerson].setFirstName(lastPerson);
			if(People[lastPerson].getSex() == "" || People[lastPerson].isMale())
				People[lastPerson].setSex("M");
			else
				std::cerr << lastPerson << " sex is already defined as " << People[lastPerson].getSex() << " cannot be a son" << std::endl;
		}
		else if(keyword == "DAUGHTER"){
			std::cin >> lastPerson;
			People[lastPerson].setFirstName(lastPerson);
			if(People[lastPerson].getSex() == "" || People[lastPerson].isFemale())
				People[lastPerson].setSex("F");
			else
				std::cerr << lastPerson << " sex is already defined as " << People[lastPerson].getSex() << " cannot be a daughter" << std::endl;		
		}
		else if(keyword == "FATHER")
		{
			std::string name;
			std::cin >> name;
			if(People[lastPerson].getFather() == NULL){
				People[name].setFirstName(name);
				if(People[name].getSex() == "" || People[lastPerson].isMale())
				{	
					People[name].addChild(&People[lastPerson]);
					People[name].setSex("M");
					People[lastPerson].setFather(&People[name]);
				}else{
					std::cerr << name << " is defined as a Female and cannot be a father" << std::endl;
				}
			}
			else if(People[lastPerson].getFather()->getFirstName() != name)
				std::cerr << lastPerson << " already has a father" << std::endl;
		}else if(keyword == "MOTHER")
		{
			std::string name;
			std::cin >> name;
			if(People[lastPerson].getMother() == NULL){
				People[name].setFirstName(name);
				if(People[name].getSex() == "" || People[name].isFemale())
				{
					People[name].addChild(&People[lastPerson]);
					People[name].setSex("F");
					People[lastPerson].setMother(&People[name]);
				}else{
					std::cerr << name << " is defined as a Male and cannot be a mother" << std::endl;	
				}
			}
			else if(People[lastPerson].getMother()->getFirstName() != name)
				std::cerr << lastPerson << " already has a mother" << std::endl;
		}
	}

	bool validTree = true;
	std::map<std::string, Person>::iterator it;
	for(it = People.begin(); it != People.end(); it++)
	{
		if(!checkChildren(it->second.getFirstName(), &it->second))
		{
			std::cerr << "CYCLE ERROR: " << it->second.getFirstName() << " references themselves" << std::endl;
			validTree = false;
			break;
		}
	}

	if(validTree)
	{
		for(it = People.begin(); it != People.end(); it++)
		{
			it->second.print();
		}
	}

	return 0;
}

bool checkChildren(std::string name, Person * parent){
	for(unsigned i = 0; i < parent->getChildren().size(); i++)
	{
		if(name != parent->getChildren()[i]->getFirstName())
		{
			return checkChildren(name, parent->getChildren()[i]);
		}else
		{
			return false;
		}
	}
	return true;
}