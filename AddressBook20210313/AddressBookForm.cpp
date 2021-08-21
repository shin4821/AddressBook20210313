//AddressBookForm.cpp
#include"AddressBookForm.h"
#include"FindingForm.h"
#include"AddressBook.h"
#include<afxcmn.h>//CListCtrl
#include<afxdb.h>

BEGIN_MESSAGE_MAP(AddressBookForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnRecordButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT, OnCorrectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnEraseButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ARRANGE, OnArrangeButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERSONALS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

AddressBookForm::AddressBookForm(CWnd* parent)
	:CDialog(AddressBookForm::IDD, parent) {
	this->addressBook = NULL;
}

void AddressBookForm::Load() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.name, Personal.address, Personal.telephoneNumber, Personal.emailAddress "
		"FROM Personal;";

	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;

	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, name);
		rs.GetFieldValue((short)1, address);
		rs.GetFieldValue((short)2, telephoneNumber);
		rs.GetFieldValue((short)3, emailAddress);

		this->addressBook->Record((LPCTSTR)name, (LPCTSTR)address, (LPCTSTR)telephoneNumber, (LPCTSTR)emailAddress);
		rs.MoveNext();
	}
	rs.Close();
	db.Close();
}

void AddressBookForm::Insert(Long index) {
	CDatabase db;
	CString sql;
	CString code;
	Personal personal = this->addressBook->GetAt(index);

	code = GetCode();
	sql.Format("INSERT INTO Personal(code, name, address, telephoneNumber, emailAddress) "
		"VALUES('%s','%s','%s','%s','%s');", (LPCTSTR)code, personal.GetName().c_str(),
		personal.GetAddress().c_str(), personal.GetTelephoneNumber().c_str(),
		personal.GetEmailAddress().c_str());
	
	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	db.ExecuteSQL(sql);
	db.Close();
}

void AddressBookForm::Modify(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal;";
	Long i = 0;
	Personal personal = this->addressBook->GetAt(index);

	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;
	while (!rs.IsEOF() && i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	sql.Format("UPDATE Personal SET address='%s', telephoneNumber='%s', emailAddress='%s' WHERE code='%s';",
		personal.GetAddress().c_str(), personal.GetTelephoneNumber().c_str(),
		personal.GetEmailAddress().c_str(), (LPCTSTR)code);
	
	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}

void AddressBookForm::Delete(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal;";
	Long i = 0;

	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;

	while (!rs.IsEOF() && i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	sql.Format("DELETE FROM Personal WHERE code='%s';", (LPCTSTR)code);

	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}

CString AddressBookForm::GetCode() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal ORDER BY code DESC;";
	CString code;
	int number;

	//1. 제일 큰 코드를 찾는다.
	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, code);
	}
	//2. 코드가 없으면 기본 코드를 매긴다.
	else {
		code = "P0000";
	}
	//3. 코드(문자열)의 숫자 부분만 분리한다.
	code = code.Right(4);

	//4. 숫자부분을 숫자로 표현한다.
	number = _ttoi(code);

	//5. 숫자를 늘린다.
	number++;

	//6. 해당 정수를 문자열로 변환한다.
	//7. 알파벳과 숫자를 합쳐 코드를 만든다.
	code.Format("P%04d", number);

	//8. 코드를 출력한다.
	rs.Close();
	db.Close();
	
	return code;
}

void AddressBookForm::Save() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal;";
	CString sql_;
	Personal personal;
	Long i = 0;

	db.OpenEx("DSN=AddressBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;

	//1. db의 전체 데이터를 삭제한다.
	db.ExecuteSQL("DELETE FROM Personal;");

	//2. db의 끝이 아니고 length보다 작을동안 반복한다.
	while (i < this->addressBook->GetLength()) {
		//2.1 코드를 찾는다.
		rs.GetFieldValue((short)0, code);

		//2.2 i번째 personal을 db에 입력한다.
		personal = this->addressBook->GetAt(i);
		sql_.Format("INSERT INTO Personal(code, name, address, telephoneNumber, emailAddress) "
			"VALUES('%s','%s','%s','%s','%s');", (LPCTSTR)code, personal.GetName().c_str(),
			personal.GetAddress().c_str(), personal.GetTelephoneNumber().c_str(),
			personal.GetEmailAddress().c_str());

		db.ExecuteSQL(sql_);
		rs.MoveNext();
		i++;
	}
	rs.Close();
	db.Close();
}


BOOL AddressBookForm::OnInitDialog() {
	CDialog::OnInitDialog(); //오버라이딩

	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	CString number;
	Personal personal;
	Long i = 0;

	//1. 윈도우가 생성될 때
	  //1.1 주소록을 만든다.
	  this->addressBook = new AddressBook;

	  //1.2. 전체 선택 설정한다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.3. 적재한다.
	  Load();

	  //1.4 리스트뷰 컨트롤의 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(0, "번호", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(1, "성명", LVCFMT_CENTER, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(2, "주소", LVCFMT_CENTER, 200);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(3, "전화번호", LVCFMT_CENTER, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(4, "이메일주소", LVCFMT_CENTER, 150);

	  //1.5. 적재된 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
	  while (i < this->addressBook->GetLength()) {
		  personal = this->addressBook->GetAt(i);

		  number.Format("%d", i + 1);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(i, number);

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

	  return FALSE;
}

void AddressBookForm::OnRecordButtonClicked() {
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	CString number;
	Personal personal;
	Long index;

	//2. 기재하기 버튼을 클릭했을 때
	  //2.1 성명, 주소, 전화번호, 이메일 주소을 읽는다.
	  GetDlgItem(IDC_EDIT_NAME)->GetWindowText(name);
	  GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	  GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->GetWindowText(telephoneNumber);
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);

	  //2.2 주소록에 기재한다.
	  index = this->addressBook->Record((LPCTSTR)name, (LPCTSTR)address, (LPCTSTR)telephoneNumber,
		  (LPCTSTR)emailAddress);

	  // db에 입력한다.
	  Insert(index);

	  //2.3 리스트뷰 컨트롤에 항목을 추가한다.
	  number.Format("%d", index + 1);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(index, number);

	  personal = this->addressBook->GetAt(index);
	  
	  name = CString(personal.GetName().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 1, name);

	  address = CString(personal.GetAddress().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 2, address);

	  telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 3, telephoneNumber);

	  emailAddress = CString(personal.GetEmailAddress().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 4, emailAddress);
}

void AddressBookForm::OnFindButtonClicked() {
	//3. 찾기버튼을 클릭했을 때
	  //3.1 찾기 윈도우를 출력한다.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void AddressBookForm::OnCorrectButtonClicked() {
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	Personal personal;
	Long index;

	//5. 고치기 버튼 클릭했을 때
	  //5.1 주소, 전화번호, 이메일주소를 읽는다.
	  GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	  GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->GetWindowText(telephoneNumber);
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);

	  //5.2 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();

	  //5.3. 주소록에서 고친다.
	  index = this->addressBook->Correct(index, (LPCTSTR)address, (LPCTSTR)telephoneNumber, 
		  (LPCTSTR)emailAddress);

	  // db에서 고친다.
	  Modify(index);


	  //5.4. 리스트뷰 컨트롤에서 항목을 고친다.
	  personal = this->addressBook->GetAt(index);

	  address = CString(personal.GetAddress().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 2, address);

	  telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 3, telephoneNumber);

	  emailAddress = CString(personal.GetEmailAddress().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 4, emailAddress);
}

void AddressBookForm::OnEraseButtonClicked() {
	Long index;
	CString number;

	//6. 지우기 버튼을 클릭했을 때
	  //6.1 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();
	  
	  //6.2 주소록에서 지운다.
	  this->addressBook->Erase(index);


	  // db에서 지운다.
	  Delete(index);


	  //6.3 리스트뷰 컨트롤에서 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteItem(index);

	  //6.4 지운 리스트뷰컨트롤 번호의 하위 번호에 대하여, 리스트뷰에 1씩 빼서 적는다.
	  while (index < this->addressBook->GetLength()) {
		  number.Format("%d", index + 1);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 0, number);
		  index++;
	  }
}

void AddressBookForm::OnArrangeButtonClicked() {
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	CString number;
	Personal personal;
	Long i = 0;

	//7. 정리하기 버튼을 클릭했을 때
	  //7.1 주소록을 정리한다.
	  this->addressBook->Arrange();

	  //7.2 리스트뷰 컨트롤의 모든 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteAllItems();

	  //7.3 리스트뷰 컨트롤에 주소록의 length만큼 항목을 추가한다.
	  while (i < this->addressBook->GetLength()) {
		  number.Format("%d", i + 1);

		  ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(i, number);
		  personal = this->addressBook->GetAt(i);

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

void AddressBookForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	Long index;

	//4. 항목을 더블클릭 했을때
	  //4.1 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();

	  //4.2 해당 항목의 성명, 주소, 전화번호, 이메일 주소를 읽는다.
	  name = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 1);
	  address = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 2);
	  telephoneNumber = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 3);
	  emailAddress = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 4);

	  //4.3 개인에 성명, 주소, 전화번호, 이메일 주소를 읽는다.
	  GetDlgItem(IDC_EDIT_NAME)->SetWindowText(name);
	  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->SetWindowText(telephoneNumber);
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
}

void AddressBookForm::OnClose() {
	//8. 닫기 버튼을 클릭했을 때
	  //8.1. 주소록을 없앤다.
	  if (this->addressBook != NULL) {

		//저장한다.
		Save();

		delete this->addressBook;
		this->addressBook = NULL;
	  }
	  //8.2. 윈도우를 종료한다.
	  EndDialog(0);
}






