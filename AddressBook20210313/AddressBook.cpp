//AddressBook.cpp
#include"AddressBook.h"

AddressBook::AddressBook(Long capacity)
	:personals(capacity) { //콜론초기화

	this->capacity = capacity;
	this->length = 0;
}

AddressBook::AddressBook(const AddressBook& source)
	:personals(source.personals) {

	this->capacity = source.capacity;
	this->length = source.length;
}

AddressBook::~AddressBook() { //소멸자

}

Long AddressBook::Record(string name, string address, string telephoneNumber, string emailAddress) {
	Long index;

	//1. name, address, telephoneNumber, emailAddress를 입력받는다.
	//2. personal객체를 만든다.
	Personal personal(name, address, telephoneNumber, emailAddress);
	//3. length가 capacity보다 작으면 Store 메세지를 보낸다.
	if (this->length < this->capacity) {
		index = this->personals.Store(this->length, personal);
	}
	//4. length가 capacity보다 크거나 같으면 AppendForRear 메세지를 보낸다.
	else if (this->length >= this->capacity) {
		index = this->personals.AppendForRear(personal);
		this->capacity++;
	}
	this->length++;

	return index;
}

void AddressBook::Find(string name, Long* (*indexes), Long* count) {
	//1. name을 입력받는다.
	//2. LinearSearchDuplicate 메세지를 보낸다.
	this->personals.LinearSearchDuplicate(&name, indexes, count, CompareNames);
	//3. indexes, count를 출력한다.
}

Long AddressBook::Correct(Long index, string address, string telephoneNumber, string emailAddress) {
	Personal personal;

	//1. index, address, telephoneNumber, emailAddress를 입력받는다.
	//2. GetAt 메세지를 보낸다.
	personal = this->personals.GetAt(index);
	//3. Modify 메세지를 보낸다.
	index = this->personals.Modify(index, Personal(personal.GetName(), address, telephoneNumber, emailAddress));
	//4. index를 출력한다.
	return index;
}

Long AddressBook::Erase(Long index) {
	//1. index를 입력받는다.
	//2. Delete 메세지를 보낸다.
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

	index = addressBook.Record("고길동", "서울시 서초구", "01011112222", "ko@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("박길동", "서울시 마포구", "01022223333", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("나길동", "인천시 연수구", "01033334444", "Na@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Record("박길동", "부산시 해운대구", "01044445555", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;
	
	addressBook.Find("박길동", &indexes, &count);
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

	index = addressBook.Correct(1, "서울시 종로구", "01077778888", "park@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << endl;
	cout << personal.GetAddress() << endl;
	cout << personal.GetTelephoneNumber() << endl;
	cout << personal.GetEmailAddress() << endl;

	index = addressBook.Erase(3);
	if (index == -1) {
		cout << "지워졌습니다." << endl;
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






