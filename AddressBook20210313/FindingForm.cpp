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
	CDialog::OnInitDialog();//오버라이딩

	//1. 윈도우가 생성될 때
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	  //1.1. 리스트뷰 컨트롤의 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(0, "번호", LVCFMT_LEFT, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(1, "성명", LVCFMT_CENTER, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(2, "주소", LVCFMT_RIGHT, 200);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(3, "전화번호", LVCFMT_RIGHT, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(4, "이메일주소", LVCFMT_RIGHT, 150);

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

	//2. 찾기 버튼을 클릭했을 때
	  //2.1 성명을 읽는다.
	  GetDlgItem(IDC_EDIT_NAME)->GetWindowText(name);

	  //2.2 주소록 윈도우를 찾는다.
	  addressBookForm = (AddressBookForm*)CWnd::FindWindow("#32770", "주소록");

	  //2.3 주소록 윈도우의 주소록에서 찾는다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  addressBookForm->addressBook->Find((LPCTSTR)name, &this->indexes, &this->count);

	  //2.4 리스트뷰 컨트롤의 모든 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteAllItems();

	  //2.5 찾은 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
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

	//3. 항목을 더블클릭 했을 때
	  //3.1 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();

	  //3.2 읽은 리스트뷰 컨트롤 항목의 성명, 주소, 전화번호, 이메일 주소를 읽는다.
	  name = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 1);
	  address = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 2);
	  telephoneNumber = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 3);
	  emailAddress = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 4);

	  //3.3 주소록 윈도우를 찾는다.
	  addressBookForm = (AddressBookForm*)CWnd::FindWindow("#32770", "주소록");

	  //3.4 주소록 윈도우의 개인에 성명, 주소, 전화번호, 이메일 주소를 적는다.
	  addressBookForm->GetDlgItem(IDC_EDIT_NAME)->SetWindowText(name);
	  addressBookForm->GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  addressBookForm->GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->SetWindowText(telephoneNumber);
	  addressBookForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);

	  //3.5 주소록 윈도우의 리스트뷰 컨트롤에서 해당 항목을 선택한다.
	  ((CListCtrl*)addressBookForm->GetDlgItem(IDC_LIST_PERSONALS))->SetSelectionMark(this->indexes[index]);

	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  //3.6. 윈도우를 종료한다,
	  EndDialog(0);
}

void FindingForm::OnClose() {
	//4. 닫기 버튼을 클릭했을 때
	  //4.1 닫기 윈도우를 종료한다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }
	  EndDialog(0);
}






