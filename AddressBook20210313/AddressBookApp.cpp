//AddressBookApp.cpp
#include"AddressBookApp.h"
#include"AddressBookForm.h"// 메인 함수를 만들어야 함.

BOOL AddressBookApp::InitInstance() {
	AddressBookForm addressBookForm;

	this->m_pMainWnd = &addressBookForm; // main 윈도우 연결
	addressBookForm.DoModal();

	return TRUE;
}

AddressBookApp addressBookApp; // 객체 만들어주기. 중괄호 밖에서 해야함.









