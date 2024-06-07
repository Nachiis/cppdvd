#include "Inc.h"
#include "Node.hpp"
#include "Users.hpp"
#include "DVD.hpp"
#include "OriLsit.hpp"
#include "Function.h"
int ListShowNum = 15;//ÿҳ��ʾ������
DVDStockList _DVDStock(ListShowNum);
DVDBorrowList _DVDBorrow(ListShowNum);
UsersList _Users(ListShowNum);
Time NowTime;
//=============���ڹ��ܺ���=============
#pragma region Function
//����������������ʾ��Ҫɾ�����ַ�����0��ʾ����
void WindowsClear(int Num = 0)
{
	if (Num)
		while (Num--)
			cout << "\b \b";
	else
		system("cls");
}
//��ʼ����ȡ��ǰʱ��
bool Init(){NowTime.SetCurrentTime();return true;}
/*��������������������ʾ���Χ�������ַ���������Ϊ0����������b >= a >= 1*/
int CinCMD(int a,int b)
{
	int cmd;
	cin >> cmd;
	while (cmd > b || cmd < a)
	{
		cout << "******************************" << endl;
		cout << "* �������������";
		Sleep(500);
		WindowsClear(9*2);
		// ���������
		cin.clear();
		while (cin.get()!='\n');
		cout << "*>>>";
		cin >> cmd;
	}
	return cmd;
}
//�ظ�����
int RetryWindows(int(*WindowsFunction)(void))
{
	//�����õĴ��ڷ���0ʱ��˵����ǰ�����˳�
	while(WindowsFunction());
	//��ʱ����ֵ
	return 0;
}
void ClearInputBuffer()
{
	// ������뻺�����еĲ�������
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
#pragma endregion
//=============����=============
#pragma region Windows
//�û���ӭ����
int UserWelcomeMenu()
{
	WindowsClear();
	
	int CMD;

	cout << "******************************" << endl;
	cout << "* ��ӭ�û�:" << _Users.Position->Data.GetName() << endl;
	cout << "* ��ǰʱ�䣺" << NowTime << endl;
	cout << "******************************" << endl;
	cout << "* ��ܰ��ʾ���밴����ʾ��������" << endl;
	cout << "******************************" << endl;
	cout << "* 1.�鿴������Ϣ" << endl;
	cout << "* 2.����Ӱ�� (�黹�뵽������Ϣ�в鿴)" << endl;
	cout << "* 3.�˳���¼" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, 3);
	ClearInputBuffer();
	//��ʱ����ֵ
	switch (CMD)
	{
	case 1:
		//�鿴������Ϣ
		RetryWindows(UserInfomation);
		return 1;
	case 2:
		//����Ӱ������Ȼ�ǽ�ȥ����ôĬ��ҳ����1
		_DVDStock.SetPage(1);
		_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
		RetryWindows(BorrowDVDMenu);
		return 1;
	case 3:
		//�˳���¼
		return 0;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}
//����û�����
int AddUsersMenu()
{
	WindowsClear();
	string Name;
	Users* nowlogin = &_Users.Position->Data;
	Users temp;
	int tempState;
	int CMD;
	cout << "******************************" << endl;
	cout << "* �������û�����" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Name;
	if (Name == "0")
	{
		cout << "******************************" << endl;
		cout << "* �û���������Ϊ0" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	temp.SetName(Name);
	ClearInputBuffer();
	//�����Ƿ����
	_Users.SingleSearchUsers(_Users.OriHead, Name);
	if (_Users.Position)
	{
		cout << "******************************" << endl;
		cout << "* �û��Ѵ���" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		temp.SetName(Name);
		cout << "* �������û����ͣ�(1/2 ����Ա/�û�)" << endl;
		cout << "* >>>";
		tempState = CinCMD(1, 2);
		temp.SetState(tempState);
		temp.SetPassword(DEFAULTPASSWORD);
		ClearInputBuffer();
		cout << "******************************" << endl;
		cout << "* ��ȷ��������Ϣ��Ӱ�����ᱻ����������Ϣ�����" << endl;
		cout << "* �û�����" << temp.GetName() << endl;
		cout << "* �û����ͣ�";
		if (temp.GetState() == 1)
			cout << "����Ա" << endl;
		else
			cout << "�û�" << endl;
		cout << "******************************" << endl;
		cout << "* ȷ����ӣ�" << endl;
		cout << "* 1.ȷ��" << endl;
		cout << "* 2.ȡ��" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//ȷ�����
			_Users.AddToHead(_Users.OriHead, temp);//�����Զ���������
			//����ҳ�룬���¹����ҳ����
			_Users.SetPage(1);
			_Users.BuildPageListFrom(_Users.OriHead);
			//position���¶�λ
			_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
			if(tempState == 2)
				_Users.CreateUser(Name);
				
			cout << "******************************" << endl;
			cout << "* ��ӳɹ�" << endl;
			cout << "* Ĭ������Ϊ��" << DEFAULTPASSWORD << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		case 2:
			//ȡ��
			return 0;
		default:
			cout << "������ô�����ģ�" << endl;
			system("pause");
			return 0;
		}
	}
}
//Ҫע�⣬��������������ʱ��position���Ѿ�ָ����Ҫ�鿴���û�����ɾ������û���ʱ�����positionû��
//���¶�λ����ôposition�ͻ�ָ�գ�����thisָ���޷������������ڵ�¼���û���������ɾ���û���ʱ��Ҫ���¶�λposition
//ɾ���û�����
int DeleteUsersMenu(Node<Users>* ListFromWhere)
{
	WindowsClear();
	int CMD;
	int Order = 0;
	Users* nowlogin = &_Users.Position->Data;
	string tempname;
	while (getchar() != '\n')
	{
		cin >> Order;
		//�������IDNum����
		_Users.SingleSearchUsers(ListFromWhere, Order);
		//��ListFromWhere�����жϣ���ֹԽ��
		//����pagehead����ֻ����ҳ�����ģ�����searchresultlist����ֻ���������������
		if ((ListFromWhere == _Users.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListFromWhere == _Users.SearchResultList && (Order <= 0 || Order > _DVDStock.GetNumofSerchResult())))
		{
			cout << "******************************" << endl;
			cout << "* �벻Ҫ�������" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if (_Users.Position)
		{
			if (_Users.Position->Data.GetName() == nowlogin->GetName())
			{
				cout << "******************************" << endl;
				cout << "* ����ɾ����ǰ��¼�û�" << endl;
				cout << "******************************" << endl;
				system("pause");
				//position���¶�λ
				_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
				return 0;
			}
			cout << "******************************" << endl;
			cout << "* ��Ҫɾ�����û�Ϊ��" << endl;
			cout << "* " << _Users.Position->Data.GetName() << endl;
			cout << "******************************" << endl;
			cout << "* ȷ��ɾ����" << endl;
			cout << "* 1.ȷ��" << endl;
			cout << "* 2.ȡ��" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//ɾ��
				tempname = _Users.Position->Data.GetName();
				if (ListFromWhere == _Users.PageHead)
					//ɾ����ҳ����Ľڵ㣬ͬʱҲɾ����Orihead��Ľڵ�
					_Users.DeleteNode(_Users.PageHead, _Users.Position);
				else if (ListFromWhere == _Users.SearchResultList)//��֪���᲻�ᴫ�˸�ʲô�������
				{
					//����ɾ����Ӧ��Orihead��ģ���Ȼ��positionָ���SearchResultListɾ�˾��Ҳ�����
					//ɾ��ģ�������������
					_Users.DeleteNode(_Users.SearchResultList, _Users.Position);
					//ɾ��ԭʼ����
					_Users.SingleSearchUsers(_Users.OriHead, tempname);
					_Users.DeleteNode(_Users.OriHead, _Users.Position);
				}
				//�ļ�ɾ
				_Users.DeleteFile(tempname);
				//��position���¶�λ
				_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
				//����ҳ�룬���¹����ҳ����
				_DVDStock.SetPage(1);
				_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
				cout << "******************************" << endl;
				cout << "* ɾ���ɹ�" << endl;
				cout << "******************************" << endl;
				
				system("pause");
				break;
			case 2:
				//ȡ��
				break;
			default:
				cout << "������ô�����ģ�" << endl;
				system("pause");
				return 0;
			}
		}
		else
		{
			//������ֹԽ���ˣ���������������ģ������Ҳ�����
			cout << "******************************" << endl;
			cout << "* ϵͳ��֪Ϊ��δ�ҵ����û�" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* ��û��ɾ���κ��û�" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* ���һ��ɾ�����û����Ϊ��" << Order << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}

//mode = 1��ȷ������mode = 2ģ���������ܽ��������û���һ���ǹ���Ա
//ģ����ȷ�����û�
int A_FSearchUsersResultMenu(int mode, string Key)
{
	WindowsClear();
	int CMD;
	if (mode == 1)
		_Users.ContinueSearchUsers(Key);
	else
		_Users.ContinueSearchUsersFuzzy(Key);
	cout << "******************************" << endl;
	cout << "* ���������" << _Users.GetNumofSerchResult() << endl;
	_Users.DisplayList(_Users.SearchResultList);
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	cout << "* 2.ɾ�� + Ӱ��ǰ������" << endl;
	cout << "* 3.�����û��� + �û�ǰ������ + ���û���" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";

	CMD = CinCMD(1, 3);
	switch (CMD)
	{
	case 1:
		//����
		_Users.DestoryList(_Users.SearchResultList);
		return 0;
	case 2:
		//ɾ��
		DeleteUsersMenu(_Users.SearchResultList);//�����ﴫ�������������������PageHead
		return 1;
	case 3:
		//�����û���
		ChangeUsersName(_Users.SearchResultList);//�����ﴫ�������������������PageHead
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}
//�����û���
int ChangeUsersName(Node<Users>* ListFromWhere)
{
	WindowsClear();
	int CMD;
	int Order = 0;
	string tempname;
	string oldname;
	Users* nowlogin = &_Users.Position->Data;
	cin >> Order;
	//�������IDNum����
	_Users.SingleSearchUsers(ListFromWhere, Order);//����position�����ˣ�����Ҫ������
	//��ListFromWhere�����жϣ���ֹԽ��
	//����pagehead����ֻ����ҳ�����ģ�����searchresultlist����ֻ���������������
	if ((ListFromWhere == _Users.PageHead && (Order <= 0 || Order > ListShowNum)) ||
		(ListFromWhere == _Users.SearchResultList && (Order <= 0 || Order > _Users.GetNumofSerchResult())))
	{
		cout << "******************************" << endl;
		cout << "* �벻Ҫ�������" << endl;
		cout << "* ������ѡ��" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	if (_Users.Position)
	{
		cin >> tempname;
		if (tempname == "0")
		{
			cout << "******************************" << endl;
			cout << "* �û���������Ϊ0" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		oldname = _Users.Position->Data.GetName();
		cout << "******************************" << endl;
		cout << "* ��Ҫ�޸ĵ��û�Ϊ��";
		cout << "* " << oldname << endl;
		cout << "* �����޸�Ϊ��";
		cout << "* " << tempname << endl;
		cout << "******************************" << endl;
		cout << "* ȷ���޸ģ�" << endl;
		cout << "* 1.ȷ��" << endl;
		cout << "* 2.ȡ��" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//�޸�
			if (ListFromWhere == _Users.PageHead)
			{
				//�޸ķ�ҳ����Ľڵ㣬ͬʱҲ�޸���Orihead��Ľڵ�
				_Users.Position->Data.SetName(tempname);
			}
			else if (ListFromWhere == _Users.SearchResultList)//��֪���᲻�ᴫ�˸�ʲô�������
			{
				//�޸������������
				_Users.Position->Data.SetName(tempname);
				//�޸�ԭʼ����
				_Users.SingleSearchUsers(_Users.OriHead, oldname);
				_Users.Position->Data.SetName(tempname);
			}
			//�ļ�������ֻ����ͨ�û������ļ�
			if (_Users.Position->Data.GetState() == VIP)
				_Users.RenameFile(oldname, tempname);
			cout << "******************************" << endl;
			cout << "* �޸ĳɹ�" << endl;
			cout << "******************************" << endl;
			//����ҳ�룬���¹����ҳ����
			_DVDStock.SetPage(1);
			_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
			//��position���¶�λ
			_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
			system("pause");
			return 0;
		case 2:
			//ȡ��
			return 0;
		default:
			cout << "������ô�����ģ�" << endl;
			system("pause");
			return 0;
		}
	}
	else
	{
		//������ֹԽ���ˣ���������������ģ������Ҳ�����
		cout << "******************************" << endl;
		cout << "* ϵͳ��֪Ϊ��δ�ҵ����û�" << endl;
		cout << "* ������ѡ��" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* ��û�и����κ��û�" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}
//��������
void ResetPassword()
{
	WindowsClear();
	int Order = 0;
	Users* nowlogin = &_Users.Position->Data;
	cin >> Order;
	ClearInputBuffer();
	//�������IDNum����
	_Users.SingleSearchUsers(_Users.PageHead, Order);
	if (_Users.Position)
	{
		_Users.Position->Data.SetPassword(DEFAULTPASSWORD);
		cout << "******************************" << endl;
		cout << "* ���óɹ�" << endl;
		cout << "* ����������Ϊ��" << DEFAULTPASSWORD << endl;
		cout << "******************************" << endl;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* δ֪���" << endl;
		cout << "******************************" << endl;
	}
	system("pause");
	//position���¶�λ
	_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
}

//�����û�������
int ArrangeUserMenu()
{
	WindowsClear();
	int CMD;
	int CMDRange = 8;
	int Order = 0;
	int SortMode = 0;
	string Key;
	string Nowname = _Users.Position->Data.GetName();
	cout << "******************************" << endl;
	cout << "*        �����û�" << endl;
	cout << "******************************" << endl;
	cout << "* ��ǰ�û�������" << _Users.GetLength() << " ��ǰҳ�룺" << _Users.GetCurrentPage() << endl;
	cout << "* ����" << _Users.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_Users.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	cout << "* 2.��ȷ�������û�����+ �ؼ���" << endl;
	cout << "* 3.ģ���������û�����+ �ؼ���" << endl;
	cout << "* 4.����    ���û����� + 1/0 ��/����" << endl;
	cout << "* 5.����û�" << endl;
	cout << "* 6.ɾ���û� + �û�ǰ������ (����ÿո����)" << endl;
	cout << "* 7.�������� + �û�ǰ������" << endl;
	cout << "* 8.�����û��� + �û�ǰ������ + ���û���" << endl;
	//�ж��Ƿ�����һҳ������һҳ
	if (_Users.GetCurrentPage() == 1 && _Users.GetCurrentPage() != _Users.GetMaxPage())
	{
		//������һҳ��
		cout << "* 9.��һҳ" << endl;
		CMDRange = 9;
	}
	else if (_Users.GetCurrentPage() != 1 && _Users.GetCurrentPage() == _Users.GetMaxPage())
	{
		//������һҳ
		cout << "* 9.��һҳ" << endl;
		CMDRange = 9;
	}
	else if (_Users.GetCurrentPage() != 1 && _Users.GetCurrentPage() != _Users.GetMaxPage())
	{
		cout << "* 9.��һҳ" << endl;
		cout << "* 10.��һҳ" << endl;
		CMDRange = 10;
	}
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);
	switch (CMD)
	{
	case 1:
		//����
		ClearInputBuffer();
		return 0;
	case 2:
		//��ȷ����
		cin >> Key;
		ClearInputBuffer();
		A_FSearchUsersResultMenu(1, Key);
		return 1;
	case 3:
		//ģ������
		cin >> Key;
		ClearInputBuffer();
		A_FSearchUsersResultMenu(2, Key);
		return 1;
	case 4:
		//��������֮���һ�position
		cin >> SortMode;
		ClearInputBuffer();
		_Users.SortUsersList(_Users.OriHead, SortMode);
		_Users.SingleSearchUsers(_Users.OriHead, Nowname);
		return 1;
	case 5:
		//����û�
		AddUsersMenu();
		return 1;
	case 6:
		//ɾ���û���ɾ���һ�position
		DeleteUsersMenu(_Users.PageHead);
		return 1;
	case 7:
		//��������
		ResetPassword();
		return 1;
	case 8:
		//�����û���
		ChangeUsersName(_Users.PageHead);
		return 1;
	case 9:
		//��һҳ
		if (_Users.GetCurrentPage() != _Users.GetMaxPage())
		{
			_Users.PageNext(_Users.OriHead);
			return 1;
		}
		//����û��break����return�����Ե�������if��ʱ�����һҳ
	case 10:
		//��һҳ
		_Users.PageLast(_Users.OriHead);
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//ɾ��Ӱ������,ListHeadfromDVDWhere��һ���Ƿ�ҳ����Ҳ�����������������
int DeleteDVDMenu(Node<DVDStock>*ListFromWhere)
{
	WindowsClear();
	int CMD;
	int Order = 0;
	while (getchar() != '\n')
	{
		cin >> Order;
		//�������IDNum����
		_DVDStock.SingleSearchDVDStock(ListFromWhere, Order);
		//��ListFromWhere�����жϣ���ֹԽ��
		//����pagehead����ֻ����ҳ�����ģ�����searchresultlist����ֻ���������������
		if ((ListFromWhere == _DVDStock.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListFromWhere == _DVDStock.SearchResultList && (Order <= 0 || Order > _DVDStock.GetNumofSerchResult())))
		{
			cout << "******************************" << endl;
			cout << "* �벻Ҫ�������" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if (_DVDStock.Position)
		{
			cout << "******************************" << endl;
			cout << "* ��Ҫɾ����Ӱ��Ϊ��" << endl;
			cout << "* " << _DVDStock.Position->Data.GetName() << endl;
			cout << "******************************" << endl;
			cout << "* ȷ��ɾ����" << endl;
			cout << "* 1.ȷ��" << endl;
			cout << "* 2.ȡ��" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//ɾ��
				if (ListFromWhere == _DVDStock.PageHead)
					//ɾ����ҳ����Ľڵ㣬ͬʱҲɾ����Orihead��Ľڵ�
					_DVDStock.DeleteNode(_DVDStock.PageHead, _DVDStock.Position);
				else if (ListFromWhere == _DVDStock.SearchResultList)//��֪���᲻�ᴫ�˸�ʲô�������
				{
					string tempname = _DVDStock.Position->Data.GetName();
					//����ɾ����Ӧ��Orihead��ģ���Ȼ��positionָ���SearchResultListɾ�˾��Ҳ�����
					//ɾ��ģ�������������
					_DVDStock.DeleteNode(_DVDStock.SearchResultList, _DVDStock.Position);
					//ɾ��ԭʼ����
					_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, tempname);
					_DVDStock.DeleteNode(_DVDStock.OriHead, _DVDStock.Position);
				}
				cout << "******************************" << endl;
				cout << "* ɾ���ɹ�" << endl;
				cout << "******************************" << endl;
				//����ҳ�룬���¹����ҳ����
				_DVDStock.SetPage(1);
				_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
				system("pause");
				break;
			case 2:
				//ȡ��
				break;
			default:
				cout << "������ô�����ģ�" << endl;
				system("pause");
				return 0;
			}	
		}
		else
		{
			//������ֹԽ���ˣ���������������ģ������Ҳ�����
			cout << "******************************" << endl;
			cout << "* ϵͳ��֪Ϊ��δ�ҵ���Ӱ��" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* ��û��ɾ���κ�Ӱ��" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* ���һ��ɾ����Ӱ�����Ϊ��" << Order << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}
//���Ӱ���˵��������һ���²˵�����Ϊ���Ӱ����Ҫ����ܶ���Ϣ
int AddDVDMenu()
{
	WindowsClear();
	string Val;
	DVDStock temp;
	Time tempTime;
	int StockNum;
	int CMD;
	cout << "******************************" << endl;
	cout << "* ��������У���������������ϵͳ�ݲ�֧��ͬ��Ӱ��" << endl;
	cout << "* ��������Ӱ������" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Val;
	temp.SetName(Val);
	ClearInputBuffer();
	//�����Ƿ����
	_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, Val);
	if(_DVDStock.Position)
	{
		cout << "******************************" << endl;
		cout << "* Ӱ���Ѵ��ڣ���ֱ��������ӵ�������" << endl;
		cout << "******************************" << endl;
		cin >> StockNum;
		ClearInputBuffer();
		_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() + StockNum);
		cout << "******************************" << endl;
		cout << "* ���óɹ�" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* ���������Ӱ����" << Val << endl;
		cout << "* ��������ң�" << endl;
		cout << "* >>>";
		cin >> Val;
		temp.SetNation(Val);
		ClearInputBuffer();
		cout << "* ���������ͣ�" << endl;
		cout << "* >>>";
		cin >> Val;
		temp.SetType(Val);
		ClearInputBuffer();
		cout << "* ���������ʱ�䣨�� �� �� �ÿո��������" << endl;
		cout << "* >>>";
		cin >> tempTime;//��������������������Կ���ֱ�����룬�����������ݼ��
		ClearInputBuffer();
		temp.SetPublicTime(tempTime);//Ĭ�ϸ�ֵ��������
		cout << "* �����뱾����ӵ�������" << endl;
		cout << "* >>>";
		cin >> StockNum;
		if (StockNum < 0)
		{
			cout << "******************************" << endl;
			cout << "* �벻Ҫ���븺�������Զ�����Ϊ0" << endl;
			cout << "******************************" << endl;
			StockNum = 0;
		}
		ClearInputBuffer();
		temp.SetNum(StockNum);
		cout << "******************************" << endl;
		cout << "* ��ȷ��������Ϣ��Ӱ�����ᱻ����������Ϣ�����" << endl;
		cout << "* Ӱ������" << temp.GetName() << endl;
		cout << "* ���ң�" << temp.GetNation() << endl;
		cout << "* ���ͣ�" << temp.GetType() << endl;
		cout << "* ����ʱ�䣺" << temp.GetPublicTime() << endl;
		cout << "* ������" << temp.GetNum() << endl;
		cout << "******************************" << endl;
		cout << "* ȷ����ӣ�" << endl;
		cout << "* 1.ȷ��" << endl;
		cout << "* 2.ȡ��" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//ȷ�����
			_DVDStock.AddToHead(_DVDStock.OriHead, temp);//�����Զ���������
			//����ҳ�룬���¹����ҳ����
			_DVDStock.SetPage(1);
			_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
			cout << "******************************" << endl;
			cout << "* ��ӳɹ�" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		case 2:
			//ȡ��
			return 0;
		default:
			cout << "������ô�����ģ�" << endl;
			system("pause");
			return 0;
		}
	}
}

//����Ӱ�������棬Ӱ����ϸ��Ϣ
int ArrangeDVDMenu()
{
	WindowsClear();
	int CMD;
	int CMDRange = 6;		//���������κ�ҳ����תʱ��ֻ��6��ѡ��
	int TypeKey = 0;
	int SortMode = 0;
	string Key;
	cout << "******************************" << endl;
	cout << "*        Ӱ��" << endl;
	cout << "******************************" << endl;
	cout << "* ��ǰӰ��������" << _DVDStock.GetLength() << " ��ǰҳ�룺" << _DVDStock.GetCurrentPage() << endl;
	cout << "* ����" << _DVDStock.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_DVDStock.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	cout << "* 2.��ȷ������Ӱ������+ �ؼ���" << endl;
	cout << "* 3.ģ��������Ӱ������+ �ؼ���" << endl;
	cout << "* 4.���� + �������� + 1/0 ��/����" << endl;
	cout << "* 5.���Ӱ��" << endl;
	cout << "* 6.ɾ��Ӱ�� + Ӱ��ǰ������ (����ÿո����)" << endl;

	//if�����ж�����Ϊ�п���ѹ���������һҳ�����Բ���Ҫ��ҳ
	if (_DVDStock.GetCurrentPage() == 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		//������һҳ��
		cout << "* 7.��һҳ" << endl;
		CMDRange = 7;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() == _DVDStock.GetMaxPage())
	{
		//������һҳ
		cout << "* 7.��һҳ" << endl;
		CMDRange = 7;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		cout << "* 7.��һҳ" << endl;
		cout << "* 8.��һҳ" << endl;
		CMDRange = 8;
	}
	cout << "******************************" << endl;
	cout << "* �������ţ�" << endl;
	cout << "* 1.Ӱ���� 2.���� 3.���� 4.������� 5.�������" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);

	switch (CMD)
	{
	case 1:
		//����
		ClearInputBuffer();
		return 0;
	case 2:
		//��ȷ����
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(1, Key, 1);
		return 1;
	case 3:
		//ģ������
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(2, Key, 1);
		return 1;
	case 4:
		//����
		cin >> TypeKey >> SortMode;
		ClearInputBuffer();
		_DVDStock.SortDVDList(_DVDStock.OriHead, (STOCKDVDKEY)TypeKey, SortMode);
		return 1;
	case 5:
		//���Ӱ��
		ClearInputBuffer();
		AddDVDMenu();
		return 1;
	case 6:
		//ɾ��Ӱ��
		DeleteDVDMenu(_DVDStock.PageHead);
		return 1;
	case 7:
		//��һҳ
		if (_DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
		{
			_DVDStock.PageNext(_DVDStock.OriHead);
			return 1;
		}
		//����û��break����return�����Ե�������if��ʱ�����һҳ
	case 8:
		//��һҳ
		_DVDStock.PageLast(_DVDStock.OriHead);
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//�鿴����Ա������Ϣ����
int AdminInfomation()
{
	WindowsClear();

	int CMD;
	cout << "******************************" << endl;
	cout << "*        ������Ϣ" << endl;
	cout << "******************************" << endl;
	cout << "* �û�����" << _Users.Position->Data.GetName() << endl;
	cout << "* ��  �룺" << _Users.Position->Data.GetPassword() << endl;
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	cout << "* 2.�޸�����" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, 2);
	ClearInputBuffer();
	switch (CMD)
	{
	case 1:
		//����
		return 0;
	case 2:
		//�޸�����
		ChangeUserPassword();
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}
//����Ա��ӭ����
int AdminMenu()
{
	WindowsClear();

	int CMD;

	cout << "******************************" << endl;
	cout << "* ��ӭ����Ա:" << _Users.Position->Data.GetName() << endl;
	cout << "* ��ǰʱ�䣺" << NowTime << endl;
	cout << "******************************" << endl;
	cout << "* ��ǰӰ��������" << _DVDStock.GetLength() << endl;
	cout << "* ��ǰ�û�������" << _Users.GetLength() << endl;
	cout << "******************************" << endl;
	cout << "* ��ܰ��ʾ���밴����ʾ��������" << endl;
	cout << "******************************" << endl;
	cout << "* 1.�鿴������Ϣ" << endl;
	cout << "* 2.�鿴�û���Ϣ" << endl;
	cout << "* 3.�鿴Ӱ����Ϣ" << endl;
	cout << "* 4.�˳���¼" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";

	CMD = CinCMD(1, 4);
	ClearInputBuffer();
	switch (CMD)
	{
	case 1:
		RetryWindows(AdminInfomation);
		return 1;
		break;
	case 2:
		RetryWindows(ArrangeUserMenu);
		return 1;
		break;
	case 3:
		//�鿴��ϸӰ����Ϣ
		RetryWindows(ArrangeDVDMenu);
		return 1;
		break;
	case 4:
		return 0;
		break;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//��¼���������ص�¼�û���Ϣ
int Login()
{
	//������Ȼ�����û����������ж��û��Ƿ���ڼ���Ϣ
	WindowsClear();
	//�����û����벢����Ҫ�ж����ݺϷ��ԣ���Ϊ�û����������ں����ж�
	string Name, Password;

	cout << "******************************" << endl;
	cout << "* ������������ϵ����Ա��������" << endl;
	cout << "******************************" << endl;
	cout << "* �������û�����(����0�˳�)" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Name;
	if (Name == "0")
	{
		return 0;
	}
	cout << "******************************" << endl;
	cout << "* ���������룺" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Password;
	//�����û�
	_Users.SingleSearchUsers(_Users.OriHead,Name);

	if (_Users.Position && _Users.Position->Data.GetPassword() == Password)
	{
		//�ж��û����
		if (_Users.Position->Data.GetState() == ADMIN)
		{
			//����Ա
			ClearInputBuffer();
			//�����ǹ���Ա�����û��������˳�ϵͳ��ʱ��ű�������û���Ϣ
			RetryWindows(AdminMenu);
			return 1;
		}
		else
		{
			//��ͨ�û�
			ClearInputBuffer();
			//��ʼ���û���������
			_DVDBorrow.Init(_Users.Position->Data.GetName());
			//�����û���ӭ����
			RetryWindows(UserWelcomeMenu);
			_DVDBorrow.BorrowDVDSave();
			return 1;
		}
	}
	else
	{
		cout << "******************************" << endl;
		cout << "*    �����������û�������  *" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 1;
	}
}

//��ҳ��
int OptionBegin()
{
	WindowsClear();
	cout << "******************************" << endl;
	cout << "*        Ӱ������ϵͳ        *" << endl;
	cout << "******************************" << endl;
	cout << "* 1.��¼ " << endl;
	cout << "* 2.�˳�ϵͳ" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	int CMD = CinCMD(1, 2);
	ClearInputBuffer();
	switch (CMD)
	{
	case 1:
		//��¼
		RetryWindows(Login);
		return 1;
	case 2:
		//exit���Զ������������ֶ���������
		_Users.UsersSave();
		_DVDStock.DVDStockSave();
		WindowsClear();
		cout << "��лʹ�ã�" << endl;
		exit(0);
		return 0;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//�黹Ӱ������
int ReternBorrowedDVDMenu()
{
	//���ĵ����
	int Order = 0;
	cout << "******************************" << endl;
	cin >> Order;
	ClearInputBuffer();
	if(Order > _DVDBorrow.GetLength() || Order <= 0)
	{
		cout << "******************************" << endl;
		cout << "* �벻Ҫ�������" << endl;
		cout << "* ������ѡ��" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	_DVDBorrow.SingleSearchDVDBorrow(Order);
	if (_DVDBorrow.Position)
	{
		cout << "* ��Ҫ�黹��Ӱ��Ϊ��" << endl;
		cout << "* " << _DVDBorrow.Position->Data.GetName() << endl;
		//�黹������
		_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, _DVDBorrow.Position->Data.GetName());
		if (!_DVDStock.Position)
		{
			cout << "******************************" << endl;
			cout << "* ��Ӱ���ѱ�����Աɾ������Ҫ�Ƴ���Ӱ�����ļ�¼��" << endl;
			cout << "******************************" << endl;
			cout << "* 1.��" << endl;
			cout << "* 2.��" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			int CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//ɾ��
				_DVDBorrow.DeleteNode(_DVDBorrow.OriHead, _DVDBorrow.Position);
				cout << "******************************" << endl;
				cout << "* �Ƴ���¼�ɹ�!" << endl;
				cout << "******************************" << endl;
				system("pause");
			case 2:
				return 0;
			default:
				cout << "������ô�����ģ�" << endl;
				system("pause");
				return 0;
			}
		}
		_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() + 1);
		_DVDBorrow.DeleteNode(_DVDBorrow.OriHead, _DVDBorrow.Position);
		cout << "* �黹�ɹ�!" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout <<"******************************" << endl;
		cout << "* ϵͳ�ڹ黹Ӱ��������δ֪����" << endl;
		cout << "******************************" << endl;
		system("pause");
		exit(0);
	}
}

//δ��ʱ�黹Ӱ������
void OverTimeRemind()
{
	Node<BorrowDVD>* p = _DVDBorrow.OriHead;
	while (p)
	{
		if(p->Data.GetReturnTime() < NowTime)
		{
			int day = NowTime - p->Data.GetReturnTime();
			cout << "******************************" << endl;
			cout << "* Ӱ����" << p->Data.GetName() << "�Ѿ���ʱ�黹" << day << "��" << endl;
			cout << "* �뾡��黹" << endl;
			cout << "* ��Ϊ��ʱ�黹������Ҫ֧��" << RentOverMoney(day) << "Ԫ" << endl;
		}
		p = p->Next;
	}
}
//�û��޸��������
int ChangeUserPassword()
{
	WindowsClear();
	string NewPassword;
	cout << "******************************" << endl;
	cout << "* �����������룺" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> NewPassword;
	_Users.Position->Data.SetPassword(NewPassword);
	cout << "******************************" << endl;
	cout << "* �����޸ĳɹ�" << endl;
	cout << "******************************" << endl;
	system("pause");
	return 0;
}

//�鿴������Ϣ����
int UserInfomation()
{
	WindowsClear();
	int CMD;
	cout << "******************************" << endl;
	cout << "*        ������Ϣ" << endl;
	cout << "******************************" << endl;
	cout << "* �û�����" << _Users.Position->Data.GetName() << endl;
	cout << "* ��  �룺" << _Users.Position->Data.GetPassword() << endl;
	cout << "******************************" << endl;
	cout << "* Ӱ��������Ϣ��" << _DVDBorrow.GetLength() << endl;
	cout << "******************************" << endl;
	_DVDBorrow.DisplayList(_DVDBorrow.OriHead);
	OverTimeRemind();
	cout << "******************************" << endl;

	if (_DVDBorrow.OriHead)
	{
		cout << "* 1.����" << endl;
		cout << "* 2.�޸����루����ϵ����Ա�޸��û�����" << endl;
		cout << "* 3.�黹Ӱ����+ ����Ӱ��ǰ�����ţ�" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 3);
	}
	else
	{
		cout << "* 1.����" << endl;
		cout << "* 2.�޸����루����ϵ����Ա�޸��û�����" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
	}
	switch (CMD)
	{
	case 1:
		//����
		ClearInputBuffer();
		return 0;
	case 2:
		//�޸�����
		ClearInputBuffer();
		ChangeUserPassword();
		return 1;
	case 3:
		//�黹Ӱ��
		ReternBorrowedDVDMenu();
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//���ģ�ListHeadfromDVDWhere��һ���Ƿ�ҳ����Ҳ�����������������
int BorrowedDVD(Node<DVDStock>*& ListHeadfromDVDWhere)
{
	WindowsClear();
	int Order = 0;
	int Day = 0;
	BorrowDVD temp;
	while (getchar() != '\n')
	{
		cout << "******************************" << endl;
		cin >> Order;
		cin >> Day;
		if (Day <= 0)
		{
			cout << "* �벻Ҫ���벻���ʵ�����Ŷ~" << endl;
			cout << "* ���ν���ʧ��" << endl;
			cout << "* �����³���" << endl;
			system("pause");
			return 0;
		}
		//����������pagehead��˵���Ǵ�BorrowedDVDMenu���ģ���ôֻ����ҳ�����ģ�
		//����������SearchResultList��˵���������������ģ���ô��Ҫ��GetNumofSerchResult�Ƚ�
		if ((ListHeadfromDVDWhere == _DVDStock.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListHeadfromDVDWhere == _DVDStock.SearchResultList && (Order > _DVDStock.GetNumofSerchResult() || Order <= 0)))
		{
			cout << "******************************" << endl;
			cout << "* �벻Ҫ�������" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		//�������IDNum����
		_DVDStock.SingleSearchDVDStock(ListHeadfromDVDWhere,Order);
		//���أ����Ƿ��Ѿ�����
		if(_DVDBorrow.GetLength() > 0)
			_DVDBorrow.SingleSearchDVDBorrow(_DVDStock.Position->Data.GetName());
		if(_DVDBorrow.Position)
		{
			cout << "******************************" << endl;
			cout << "* ���Ѿ������˸�Ӱ��" << endl;
			cout << "* �벻Ҫ�ظ�����" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if(_DVDStock.Position->Data.GetNum() <= 0)
		{
			cout << "******************************" << endl;
			cout << "* Ӱ����治��" << endl;
			cout << "* ������ѡ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		else
		{
			//������
			_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() - 1);
			// ����һ��������������Ҫ�ҵ�ԭ���������ʵֵ������Ϊ����ʾһ�£���Ҫ������
			if(_DVDStock.SearchResultList == ListHeadfromDVDWhere)
			{
				_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead,_DVDStock.Position->Data.GetName());
				_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() - 1);
			}
		}

		cout << "��Ҫ���ĵ�Ӱ��Ϊ��" << endl;
		cout << _DVDStock.Position->Data.GetName() << endl;
		temp = _DVDStock.Position->Data;
		cout << "******************************" << endl;
		cout << "* ���ĵ�������";
		cout << Day << endl;
		cout << "******************************" << endl;

		temp.SetBorrowTime(NowTime);
		temp.SetReturnTime(NowTime + Day);
		temp.SetRentMoney(RentMoney(Day));
		_DVDBorrow.AddToHead(_DVDBorrow.OriHead, temp);
		cout << "* ���ĳɹ�" << endl;
		cout << "* �黹��Ҫ����:" << temp.GetRentMoney() << "Ԫ" << endl;
	}

	if (Order)
		cout << "* �����˼�ʱ�黹Ŷ��" << endl;
	else
		cout << "* ��û�н����κ�Ӱ��" << endl;
	cout << "******************************" << endl;
	system("pause");
	return 0;
}
//����DVD����˵�
//mode = 1��ȷ������mode = 2ģ������,state=1��ʾ����Ա��state=0��ʾ��ͨ�û�
int A_FSearchDVDResultMenu(int mode,string Key,int state)
{
	WindowsClear();
	int CMD;
	if (mode == 1)
		_DVDStock.ContinueSearchDVDStock(Key);
	else
		_DVDStock.ContinueSearchDVDStockFuzzy(Key);
	cout << "******************************" << endl;
	cout << "* ���������" << _DVDStock.GetNumofSerchResult() << endl;
	_DVDStock.DisplayList(_DVDStock.SearchResultList);
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	if (state == 0)
		cout << "* 2.���� + Ӱ��ǰ������ + ����(��������ÿո����)" << endl;
	else//state==1
		cout << "* 2.ɾ�� + Ӱ��ǰ������" << endl;
	cout << "******************************" << endl;
		cout << "*>>>";
	CMD = CinCMD(1, 2);
	switch (CMD)
	{
	case 1:
		//����
		_DVDStock.DestoryList(_DVDStock.SearchResultList);
		return 0;
	case 2:
		if (state == 0)
			//����
			BorrowedDVD(_DVDStock.SearchResultList);
		else
		{
			//ɾ��
			DeleteDVDMenu(_DVDStock.SearchResultList);//�����ﴫ�������������������PageHead
		}
		
		return 1;
	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

//����Ӱ��������
int BorrowDVDMenu()
{
	WindowsClear();
	int CMD;
	int CMDRange = 5;//���������κ�ҳ����תʱ��ֻ��5��ѡ��
	int TypeKey = 0;
	int SortMode = 0;
	string Key;
	cout << "******************************" << endl;
	cout << "*        ����Ӱ��" << endl;
	cout << "******************************" << endl;
	cout << "* ��ǰӰ��������" << _DVDStock.GetLength() << " ��ǰҳ�룺" << _DVDStock.GetCurrentPage() << endl;
	cout << "* ����" << _DVDStock.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_DVDStock.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.����" << endl;
	cout << "* 2.���� + Ӱ��ǰ������ + ����(��������ÿո����)" << endl;
	cout << "* 3.��ȷ������Ӱ������+ �ؼ���" << endl;
	cout << "* 4.ģ��������Ӱ������+ �ؼ���" << endl;
	cout << "* 5.���� + �������� + 1/0 ��/����" << endl;

	//if�����ж�����Ϊ�п���ѹ���������һҳ�����Բ���Ҫ��ҳ
	if (_DVDStock.GetCurrentPage() == 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		//������һҳ��
		cout << "* 6.��һҳ" << endl;
		CMDRange = 6;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() == _DVDStock.GetMaxPage())
	{
		//������һҳ
		cout << "* 6.��һҳ" << endl;
		CMDRange = 6;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		cout << "* 6.��һҳ" << endl;
		cout << "* 7.��һҳ" << endl;
		CMDRange = 7;
	}
	cout << "******************************" << endl;
	cout << "* �������ţ�" << endl;
	cout << "* 1.Ӱ���� 2.���� 3.���� 4.������� 5.�������" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);

	switch (CMD)
	{
	case 1:
		//����
		ClearInputBuffer();
		return 0;
	case 2:
		//addfunction()
		BorrowedDVD(_DVDStock.PageHead);
		return 1;
	case 3:
		//��ȷ����
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(1, Key, 0);
		return 1;
	case 4:
		//ģ������
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(2, Key, 0);
		return 1;
	case 5:
		//����
		cin >> TypeKey >> SortMode;
		ClearInputBuffer();
		_DVDStock.SortDVDList(_DVDStock.OriHead, (STOCKDVDKEY)TypeKey, SortMode);
		return 1;
	case 6:
		//��һҳ
		if (_DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
		{
			_DVDStock.PageNext(_DVDStock.OriHead);
			return 1;
		}
		//����û��break����return�����Ե�������if��ʱ�����һҳ
	case 7:
		//��һҳ
		_DVDStock.PageLast(_DVDStock.OriHead);
		return 1;

	default:
		cout << "������ô�����ģ�" << endl;
		system("pause");
		return 0;
	}
}

#pragma endregion