#include "../inc/Person.h"

Person::Person(){ 
	this->firstName = "unknown";
	this->mother = NULL;
	this->father = NULL;
	this->sex = "";
}

Person::Person(std::string firstName){
	this->firstName = firstName;
	this->mother = NULL;
	this->father = NULL;
	this->sex = "";
}
std::string Person::getFirstName(){
	return this->firstName;
}

void Person::setFirstName(std::string firstName)
{
	this->firstName = firstName;
}

std::string Person::getLastName(){
	return this->lastName;
}

void Person::setLastName(std::string lastName){
	this->lastName = lastName;
}

std::string Person::getFullName(){
	return this->firstName + " " + this->lastName;
}

std::string Person::getSex(){
	return this->sex;
}

void Person::setSex(std::string sex){
	this->sex = sex;
}

Person* Person::getMother(){
	return this->mother;
}

void Person::setMother(Person * mother){
	this->mother = mother;
}

Person* Person::getFather(){
	return this->father;
}

void Person::setFather(Person * father){
	this->father = father;
}

void Person::print(){
	std::cout << this->getFirstName() << std::endl;
	if(this->isMale())
	{
		std::cout << "\tSex: Male" << std::endl;
	}else if(this->isFemale())
	{
		std::cout << "\tSex: Female" << std::endl;
	}else{
		std::cout << "\tSex: unknown" << std::endl;
	}

	if(this->father != NULL)
		std::cout << "\tFather: " << this->father->getFirstName() << std::endl;
	else
		std::cout << "\tFather: unknown" << std::endl;
	
	if(this->mother != NULL)
		std::cout << "\tMother: " << this->mother->getFirstName() << std::endl;
	else
		std::cout << "\tMother: unknown" << std::endl;

	std::cout << "\tChildren: ";
	if(this->children.size() > 0)
	{
		for(unsigned i = 0; i < this->children.size(); i++)
		{
			std::cout << this->children[i]->getFirstName() << " ";
		}
	}else{
		std::cout << "none";
	}
	std::cout << std::endl;
}

void Person::addChild(Person * child){
	for(unsigned i = 0; i < this->children.size(); i++)
	{
		if(this->children[i]->getFullName() == child->getFullName())
		{
			return;
		}		
	}
	this->children.push_back(child);
}

std::vector<Person*> Person::getChildren(){
	return this->children;
}

bool Person::isMale(){
	return (this->sex == "M" || this->sex == "m" || this->sex == "male" || this->sex == "Male");
}
bool Person::isFemale(){
	return (this->sex == "F" || this->sex == "f" || this->sex == "female" || this->sex == "female");
}