//AddressBook.cpp
#include"AddressBook.h"

AddressBook::AddressBook(Long capacity)
	:personals(capacity) { //�ݷ��ʱ�ȭ

	this->capacity = capacity;
	this->length = 0;
}

AddressBook::AddressBook(const AddressBook& source)
	:personals(source.personals) {

	this->capacity = source.capacity;
	this->length = source.length;
}

AddressBook::~AddressBook() { //�Ҹ���

}

Long AddressBook::Record(string name, string address, string telephoneNumber, string emailAddress) {
	Long index;

	//1. name, address, telephoneNumber, emailAddress�� �Է¹޴´�.
	//2. personal��ü�� �����.
	Personal personal(name, address, telephoneNumber, emailAddress);
	//3. length�� capacity���� ������ Store �޼����� ������.
	if (this->length < this->capacity) {
		index = this->personals.Store(this->length, personal);
	}
	//4. length�� capacity���� ũ�ų� ������ AppendForRear �޼����� ������.
	else if (this->length >= this->capacity) {
		index = this->personals.AppendForRear(personal);
		this->capacity++;
	}
	this->length++;

	return index;
}

void AddressBook::Find(string name, Long* (*indexes), Long* count) {
	//1. name�� �Է¹޴´�.
	//2. LinearSearchDuplicate �޼����� ������.
	this->personals.LinearSearchDuplicate(&name, indexes, count, CompareNames);
	//3. indexes, count�� ����Ѵ�.
}

Long AddressBook::Correct(Long index, string address, string telephoneNumber, string emailAddress) {
	Personal personal;

	//1. index, address, telephoneNumber, emailAddress�� �Է¹޴´�.
	//2. GetAt �޼����� ������.
	personal = this->personals.GetAt(index);
	//3. Modify �޼����� ������.
	index = this->personals.Modify(index, Personal(personal.GetName(), address, telephoneNumber, emailAddress));
	//4. index�� ����Ѵ�.
	return index;
}

Long AddressBook::Erase(Long index) {
	//1. index�� �Է¹޴´�.
	//2. Delete �޼����� ������.
	index = this->personals.Delete(index);
	this->capacity--;
	this->length--;
	
	return index;
}

void AddressBook::Arrange() {
	this->personals.SelectionSort(ComparePersonals);
}

Personal& AddressBook::operator[](Long index) {
	return this->personals[index];
}

Personal* AddressBook::operator+(Long index) {
	return this->personals + index;
}

AddressBook& AddressBook::operator=(const AddressBook& source) {
	this->personals = source.personals;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}


Personal& AddressBook::GetAt(Long index) {
	return this->personals.GetAt(index);
}

int CompareNames(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	string* other_ = (string*)other;

	return one_->GetName().compare(*other_);
}

int ComparePersonals(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	Personal* other_ = (Personal*)other;

	return one_->GetName().compare(other_->GetName());
}




#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	AddressBook addressBook(3);
	Personal personal;
	Long(*indexes);
	Long count;
	Long i;
	Long index;

	index = addressBook.Record("��浿", "����� ���ʱ�", "01011112222", "ko@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("�ڱ浿", "����� ������", "01022223333", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("���浿", "��õ�� ������", "01033334444", "Na@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("�ڱ浿", "�λ�� �ؿ�뱸", "01044445555", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;
	
	addressBook.Find("�ڱ浿", &indexes, &count);
	i = 0;
	while (i < count) {
		personal = addressBook.GetAt(indexes[i]);
		cout << personal.GetName() << endl;
		cout << personal.GetAddress() << endl;
		cout << personal.GetTelephoneNumber() << endl;
		cout << personal.GetEmailAddress() << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;
	}

	index = addressBook.Correct(1, "����� ���α�", "01077778888", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Erase(3);
	if (index == -1) {
		cout << "���������ϴ�." << endl;
	}

	cout << "Arrange" << endl;
	
	addressBook.Arrange();
	i = 0;
	while (i < addressBook.GetLength()) {
		personal = addressBook.GetAt(i);
		cout << personal.GetName() << endl;
		cout << personal.GetAddress() << endl;
		cout << personal.GetTelephoneNumber() << endl;
		cout << personal.GetEmailAddress() << endl;
		i++;
	}

	cout << addressBook[1].GetName() << endl;
	cout << addressBook[1].GetAddress() << endl;
	cout << addressBook[1].GetTelephoneNumber() << endl;
	cout << addressBook[1].GetEmailAddress() << endl;


	
	return 0;
}






