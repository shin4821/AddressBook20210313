//AddressBookApp.cpp
#include"AddressBookApp.h"
#include"AddressBookForm.h"// ���� �Լ��� ������ ��.

BOOL AddressBookApp::InitInstance() {
	AddressBookForm addressBookForm;

	this->m_pMainWnd = &addressBookForm; // main ������ ����
	addressBookForm.DoModal();

	return TRUE;
}

AddressBookApp addressBookApp; // ��ü ������ֱ�. �߰�ȣ �ۿ��� �ؾ���.









