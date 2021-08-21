//FindingForm.cpp
#include"FindingForm.h"
#include"AddressBook.h"
#include"AddressBookForm.h"
#include<afxcmn.h>
BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERSONALS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd* parent)
	:CDialog(FindingForm::IDD, parent) {
	this->indexes = NULL;
	this->count = 0;
}

BOOL FindingForm::OnInitDialog() {
	CDialog::OnInitDialog();//�������̵�

	//1. �����찡 ������ ��
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	  //1.1. ����Ʈ�� ��Ʈ���� ����� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(0, "��ȣ", LVCFMT_LEFT, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(1, "����", LVCFMT_CENTER, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(2, "�ּ�", LVCFMT_RIGHT, 200);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(3, "��ȭ��ȣ", LVCFMT_RIGHT, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(4, "�̸����ּ�", LVCFMT_RIGHT, 150);

	  return FALSE;
}

void FindingForm::OnFindButtonClicked() {
	AddressBookForm* addressBookForm;
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	CString number;
	Personal personal;
	Long i = 0;

	//2. ã�� ��ư�� Ŭ������ ��
	  //2.1 ������ �д´�.
	  GetDlgItem(IDC_EDIT_NAME)->GetWindowText(name);

	  //2.2 �ּҷ� �����츦 ã�´�.
	  addressBookForm = (AddressBookForm*)CWnd::FindWindow("#32770", "�ּҷ�");

	  //2.3 �ּҷ� �������� �ּҷϿ��� ã�´�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  addressBookForm->addressBook->Find((LPCTSTR)name, &this->indexes, &this->count);

	  //2.4 ����Ʈ�� ��Ʈ���� ��� �׸��� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteAllItems();

	  //2.5 ã�� ������ŭ ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
	  i = 0;
	  while (i < count) {
		  number.Format("%d", i + 1);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(i, number);

		  personal = addressBookForm->addressBook->GetAt(indexes[i]);

		  name = CString(personal.GetName().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 1, name);

		  address = CString(personal.GetAddress().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 2, address);

		  telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 3, telephoneNumber);

		  emailAddress = CString(personal.GetEmailAddress().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 4, emailAddress);

		  i++;
	  }
}

void FindingForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	AddressBookForm* addressBookForm;
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	Long index;

	//3. �׸��� ����Ŭ�� ���� ��
	  //3.1 ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();

	  //3.2 ���� ����Ʈ�� ��Ʈ�� �׸��� ����, �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� �д´�.
	  name = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 1);
	  address = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 2);
	  telephoneNumber = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 3);
	  emailAddress = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 4);

	  //3.3 �ּҷ� �����츦 ã�´�.
	  addressBookForm = (AddressBookForm*)CWnd::FindWindow("#32770", "�ּҷ�");

	  //3.4 �ּҷ� �������� ���ο� ����, �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� ���´�.
	  addressBookForm->GetDlgItem(IDC_EDIT_NAME)->SetWindowText(name);
	  addressBookForm->GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  addressBookForm->GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->SetWindowText(telephoneNumber);
	  addressBookForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);

	  //3.5 �ּҷ� �������� ����Ʈ�� ��Ʈ�ѿ��� �ش� �׸��� �����Ѵ�.
	  ((CListCtrl*)addressBookForm->GetDlgItem(IDC_LIST_PERSONALS))->SetSelectionMark(this->indexes[index]);

	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  //3.6. �����츦 �����Ѵ�,
	  EndDialog(0);
}

void FindingForm::OnClose() {
	//4. �ݱ� ��ư�� Ŭ������ ��
	  //4.1 �ݱ� �����츦 �����Ѵ�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }
	  EndDialog(0);
}






