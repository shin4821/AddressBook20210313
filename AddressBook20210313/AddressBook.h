//AddressBook.h
#ifndef _ADDRESSBOOK_H
#define _ADDRESSBOOK_H

typedef signed long int Long;
#include"Personal.h"
#include"Array.h"
#include<string>
using namespace std;

class AddressBook {
public:
	AddressBook(Long capacity = 256); //����Ʈ �Ű����� ������
	AddressBook(const AddressBook& source); //���������
	~AddressBook(); //�Ҹ���
	Long Record(string name, string address, string telephoneNumber, string emailAddress);
	void Find(string name, Long* (*indexes), Long* count);
	Long Correct(Long index, string address, string telephoneNumber, string emailAddress);
	Long Erase(Long index);
	void Arrange();
	Personal& GetAt(Long index);
	Personal& operator[](Long index);
	Personal* operator+(Long index);
	AddressBook& operator=(const AddressBook& source);
	Long GetCapacity() const;
	Long GetLength() const;



private:
	Array<Personal> personals;
	Long capacity;
	Long length;
};

int CompareNames(void* one, void* other);
int ComparePersonals(void* one, void* other);

inline Long AddressBook::GetCapacity() const {
	return this->capacity;
}

inline Long AddressBook::GetLength() const {
	return this->length;
}



#endif//_ADDRESSBOOK_H





