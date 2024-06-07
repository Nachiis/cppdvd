#pragma once
#include "Inc.h"
#include "Time.hpp"
#include "Node.hpp"
#include "DVD.hpp"
#include "Users.hpp"

#define USERSFILEROAD		"Users.txt"
#define PSERSONALFILEROAD	"./Users/"
#define DVDFILEROAD			"DVD.txt"
#define DEFAULTPASSWORD		"123456"
#define ADMIN 1
#define VIP 2
#define DEBUG 0

bool zh_CN_less_than(const string& s1, const string& s2);
//����ģ������
template<typename T>
class List
{
public:
	fstream file;//���������ļ�
	List(int ShowNum) :
		OriHead(nullptr), 
		OriTail(nullptr),
		PageHead(nullptr),
		PageTail(nullptr),
		Position(nullptr),
		SearchResultList(nullptr),
		ShowNum(ShowNum),
		Page(0),
		Lenth(0),
		MaxPage(0),
		NumofSearchResult(0){}
	Node<T>* OriHead;//����Դͷָ��
	Node<T>* OriTail;//����Դβָ��
	Node<T>* PageHead;//��ҳͷָ��
	Node<T>* PageTail;//��ҳβָ��
	Node<T>* Position;//��ǰλ��ָ�룬��ָ�����ָ��OriheadҲ������Pagehead
	Node<T>* SearchResultList;//�����������

	int GetCurrentPage() const { return Page; }
	int GetLength() const { return Lenth; }
	int GetMaxPage() const { return MaxPage; }

	void OriHeadCheck()
	{
		if (!OriHead)
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ���󣺴��������ϵͳ��ֹ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			exit(0);
		}
	}
	//ɾ���ض�����
	virtual void DestoryList(Node<T>*& head)
	{
		Node<T>* p;
		while (head)
		{
			p = head;
			head = head->Next;
			delete p;
		}
	}
	//����positionɾ���ض�����Ľڵ�
	virtual void DeleteNode(Node<T>*& head, Node<T>*& position)
	{
		if (!head)
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ���󣺴��������ϵͳ��ֹ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			exit(0);
		}
		if(!position)
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ���󣺴���սڵ㣬ϵͳ��ֹ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			exit(0);
		}
		if (head == position)
		{
			head = head->Next;
			if (head == PageHead)
				OriHead = OriHead->Next;
			delete position;
			position = nullptr;
			Lenth--;
			return;
		}
		Node<T>* temp_p = head;
		while (temp_p->Next)
		{
			if (temp_p->Next == position)
			{
				if(position == PageTail)
					PageTail = temp_p;
				temp_p->Next = temp_p->Next->Next;
				delete position;
				position = nullptr;
				Lenth--;
				return;
			}
			temp_p = temp_p->Next;
		}
		//������ô������Ҫ�ͷ�position
	}
	//��ӵ��ض������ͷ
	virtual void AddToHead(Node<T>*& head, T val)
	{
		head = new Node<T>(val, head);
		//������+1
		Lenth++;
	}
	//��ӵ��ض������β
	virtual void AddToTail(Node<T>*& head, T val)
	{
		if (!head)
		{
			head = new Node<T>(val);
			return;
		}
		Node<T>* temp_p = head;
		while (temp_p->Next)
		{
			temp_p = temp_p->Next;
		}
		temp_p->Next = new Node<T>(val);
		//������+1
		Lenth++;
	}
	//�ļ��й�������lenth�Զ��ӣ����ع�������ڵ�����
	virtual void BuildListFromFile(Node<T>*& head, fstream& file)
	{

		T temp;
		while (file >> temp)
			AddToHead(head, temp);
	}
	//�����ҳ����
	virtual void BuildPageListFrom(Node<T>*& head)
	{
#if DEBUG
		if (!head)
		{
			cout << "������ҳ���������" << endl;
		}
		system("pause");
#endif
		//ֱ�ӱ�Ƿ�ҳ��ʼλ�ú���ֹλ��
		Node<T>* temp_p = head;
		int Counter = 0;
		while (Counter < (Page - 1) * ShowNum && temp_p)
		{
			temp_p = temp_p->Next;
			Counter++;
		}
		Counter++;
		PageHead = temp_p;
		if (temp_p)
		{
			while (Counter < Page * ShowNum && temp_p->Next)
			{
				temp_p = temp_p->Next;
				Counter++;
			}
		}
#if DEBUG
		else
		{
			cout << "��ҳ����ͷָ��Ϊ��" << endl;
			system("pause");
		}
#endif // DEBUG

		
		PageTail = temp_p;
	}
	//��תҳ��
	virtual void PageNext(Node<T>*& head)
	{
		Page++;
#if DEBUG
		cout << "Page:" << this->GetCurrentPage() << endl;
		system("pause");
#endif // DEBUG

		BuildPageListFrom(head);
	}
	virtual void PageLast(Node<T>*& head)
	{
		Page--;
#if DEBUG
		cout << "Page:" << this->GetCurrentPage() << endl;
		system("pause");
#endif // DEBUG
		BuildPageListFrom(head);
	}
	//��������ʹ򿪵��ļ�д��
	virtual void WriteListToFile(Node<T>* head, fstream& file)
	{

		file.clear();	//����ļ���״̬
		file.seekg(0, ios::beg);	//�ļ�ָ���ƶ����ļ�ͷ
		Node<T>* p = head;
		while (p)
		{
			file << p->Data;
			p = p->Next;
		}
		file.clear();	//����ļ���״̬
		file.seekg(0, ios::beg);	//�ļ�ָ���ƶ����ļ�ͷ
	}
	//�������ҳ��
	virtual void CulcMaxPage()
	{
		if (Lenth % ShowNum)
			MaxPage = Lenth / ShowNum + 1;
		else
			MaxPage = Lenth / ShowNum;
	}
	//����ҳ��
	virtual void SetPage(int PageNum){Page = PageNum;}
protected:
	int NumofSearchResult;
	int ShowNum;//ÿҳ��ʾ����
	int Page;//��ǰҳ��
	int MaxPage;//���ҳ��
	int Lenth;//����Դ����
};
//DVD�������
class DVDStockList :public List<DVDStock>
{
public:
	

	DVDStockList(int ShowNum) :
		List(ShowNum)
	{
		//���ļ�
		file.open("DVD.txt", ios::in);
#if DEBUG
		if(file.is_open())
			cout << "DVDStockList�򿪳ɹ�" << endl;
		else
			cout << "DVDStockList��ʧ��" << endl;
		system("pause");
#endif
		//��������Դ��������������
		BuildListFromFile(OriHead, file);
		Page = 1;
		CulcMaxPage();
		//�����ҳ����
		BuildPageListFrom(OriHead);
	}
	~DVDStockList(){}
	void DVDStockSave()
	{
		if (file.is_open())
			file.close();
		file.open("DVD.txt", ios::out);
		WriteListToFile(OriHead, file);
		file.close();
		DestoryList(OriHead);
	}
	//��ȡ�����������
	int GetNumofSerchResult(){return NumofSearchResult;}
	//����Ӱ�����������ң����ز��ҵ�������
	void ContinueSearchDVDStock(string Name)
	{
		OriHeadCheck();
		DestoryList(SearchResultList);
		NumofSearchResult = 0;
		Node<DVDStock>* temp_p = OriHead;
		while(temp_p)
		{
			if (temp_p->Data.GetName() == Name)
			{
				AddToTail(SearchResultList, temp_p->Data);
				NumofSearchResult++;
			}
			temp_p = temp_p->Next;
		}
	}
	//����Ӱ��������ģ������
	void ContinueSearchDVDStockFuzzy(string Name)
	{
		OriHeadCheck();
		DestoryList(SearchResultList);
		NumofSearchResult = 0;
		Node<DVDStock>* temp_p = OriHead;
		while (temp_p)
		{
			if (temp_p->Data.GetName().find(Name) != string::npos)
			{
				AddToTail(SearchResultList, temp_p->Data);
				NumofSearchResult++;
			}
			temp_p = temp_p->Next;
		}
	}
	//����Ӱ�������β��ң����ص�position
	void SingleSearchDVDStock(Node<DVDStock>*& DataBaseList,string Name)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<DVDStock>* temp_p = DataBaseList;
		while (temp_p)
		{
			if (temp_p->Data.GetName() == Name)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	//������Ų��ң�position������Orihead��Pagehead��SearchResult����
	void SingleSearchDVDStock(Node<DVDStock>*&DataBaseList,int IDNum)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<DVDStock>* temp_p = DataBaseList;
		while (temp_p)
		{
			if (temp_p->Data.GetIDNum() == IDNum)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	//�Ը�������͹ؼ��ʽ�������
	void SortDVDList(Node<DVDStock>* head, STOCKDVDKEY key, int ascending)
	{
		if (!head)
		{
#if DEBUG
			cout << "Sort���������" << endl;
			system("pause");
#endif // DEBUG
			return;
		}
		if (!(head->Next))
		{
#if DEBUG
			cout << "Sort����ֻ��һ��Ԫ�أ�" << endl;
#endif // DEBUG
			return;
		}
		//ð�ݣ�
		Node<DVDStock>* temp_p = nullptr;
		bool isChange = true;
		//����
		switch (key)
		{
		case Name:
			//ð��
			if (ascending)
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Next->Data.GetName(), temp_q->Data.GetName()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			else
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Data.GetName(), temp_q->Next->Data.GetName()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}

			break;
		case Nation:
			//����
			if (ascending)
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Next->Data.GetNation(), temp_q->Data.GetNation()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			else
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Data.GetNation(), temp_q->Next->Data.GetNation()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}

			break;
		case Type:
			//����
			if (ascending)
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Next->Data.GetType(), temp_q->Data.GetType()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			else
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (zh_CN_less_than(temp_q->Data.GetType(), temp_q->Next->Data.GetType()))
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			break;
		case PublicTimeYear:
			//����
			if (ascending)
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (temp_q->Next->Data.GetPublicTime().GetYear() < temp_q->Data.GetPublicTime().GetYear())
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			else
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (temp_q->Data.GetPublicTime().GetYear() < temp_q->Next->Data.GetPublicTime().GetYear())
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			break;
		case StockNum:
			//����
			if (ascending)
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (temp_q->Next->Data.GetNum() < temp_q->Data.GetNum())
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			else
				//����
				while (temp_p != head->Next && isChange)
				{
					Node<DVDStock>* temp_q = head;
					isChange = false;
					for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
					{
						if (temp_q->Data.GetNum() < temp_q->Next->Data.GetNum())
						{
							DVDStock temp = temp_q->Data;
							temp_q->Data = temp_q->Next->Data;
							temp_q->Next->Data = temp;
							isChange = true;
						}
					}
				}
			break;
		default:
			cout << "******************************" << endl;
			cout << "* ϵͳ����δ֪�ؼ���" << endl;
			cout << "******************************" << endl;
			break;
		}

	}

	void DisplayList(Node<DVDStock>* head)
	{
		Node<DVDStock>* p = head;
		int i = 0;
		if (p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "Ӱ����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����ʱ��";
			cout << setw(16) << left << "�������";
			cout << endl;
			while (p)
			{
				p->Data.SetIDNum(++i);
				cout << p->Data;
				p = p->Next;
			}
		}
		else
		{
			cout << "��" << endl;
		}
	}

	void PageListDisplay()
	{
		int i = 0;
		Node<DVDStock>* temp_p = PageHead;
		if (temp_p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "Ӱ����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����ʱ��";
			cout << setw(16) << left << "�������";
			cout << endl;
			while (temp_p != PageTail)
			{
				temp_p->Data.SetIDNum(++i);
				cout << temp_p->Data;
				temp_p = temp_p->Next;
			}
			temp_p->Data.SetIDNum(++i);
			cout << temp_p->Data;
			temp_p = temp_p->Next;
		}
		else
		{
			cout << "��" << endl;
		}
	}
};

//�û����ĵ���������Ƚ����⣬����ֻ��֪�����û������ܹ����������Թ��캯���в������û���
class DVDBorrowList :public List<BorrowDVD>
{
public:
	DVDBorrowList(int ShowNum):
		List(ShowNum){}
	void Init(string Name)
	{
		Usersname = Name;
		//���ļ�
		file.open(PSERSONALFILEROAD + Usersname + ".txt", ios::in);
#if DEBUG
		if (file.is_open())
		{
			cout << "�򿪳ɹ�" << endl;
		}
			
		else
			cout << "��ʧ��" << endl;
		system("pause");
#endif
		BuildListFromFile(OriHead, file);
#if DEBUG
		if(OriHead)
			cout << "�����ɹ�" << endl;
		else
			cout << "����ʧ��" << endl;
		system("pause");
#endif // DEBUG

		Page = 1;
		CulcMaxPage();
		BuildPageListFrom(OriHead);
	}
	~DVDBorrowList(){}
	void BorrowDVDSave()
	{
		if(file.is_open())
			file.close();
		file.open(PSERSONALFILEROAD + Usersname + ".txt", ios::out);
		WriteListToFile(OriHead, file);
		file.close();
		DestoryList(OriHead);
	}
	//����string Name�����ڲ���
	void SingleSearchDVDBorrow(string Name)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<BorrowDVD>* temp_p = OriHead;
		while (temp_p)
		{
			if (temp_p->Data.GetName() == Name)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	//������Ų���
	void SingleSearchDVDBorrow(int IDNum)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<BorrowDVD>* temp_p = OriHead;
		while (temp_p)
		{
			if (temp_p->Data.GetIDNum() == IDNum)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	//����DVD�������
	void DisplayList(Node<BorrowDVD>* head)
	{
		Node<BorrowDVD>* p = head;
		int i = 0;
		if (p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "Ӱ����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����";
			cout << setw(16) << left << "����ʱ��";
			cout << setw(16) << left << "����ʱ��";
			cout << setw(16) << left << "�黹ʱ��";
			cout << setw(16) << left << "�黹���";
			cout << endl;
			while (p)
			{
				p->Data.SetIDNum(++i);
				cout << p->Data;
				p = p->Next;
			}
		}
		else
		{
			cout << "��" << endl;
		}
	}
private:
	string Usersname;
};

class UsersList :public List<Users>
{
public:
	int GetNumofSerchResult() { return NumofSearchResult; }

	UsersList(int ShowNum) :
		List(ShowNum)
	{
		//���ļ�
		file.open(USERSFILEROAD, ios::in | ios::out);
		BuildListFromFile(OriHead, file);
		Page = 1;
		CulcMaxPage();
		BuildPageListFrom(OriHead);
	}
	~UsersList() {}
	void UsersSave()
	{
		if (file.is_open())
			file.close();
		file.open(USERSFILEROAD, ios::out);
		WriteListToFile(OriHead, file);
		file.close();
		DestoryList(OriHead);
	}
	void UsersOpen()
	{
		if (!file.is_open())
			file.open(USERSFILEROAD, ios::in);
		BuildListFromFile(OriHead, file);
		Page = 1;
		CulcMaxPage();
		BuildPageListFrom(OriHead);
	}
	//����name���ң����ڵ�¼������Ա�����û���ʱ�����
	void SingleSearchUsers(Node<Users>* ListFromWhere, string Name)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<Users>* temp_p = ListFromWhere;
		while (temp_p)
		{
			if (temp_p->Data.GetName() == Name)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	void SingleSearchUsers(Node<Users>* ListFromWhere, int IDNum)
	{
		OriHeadCheck();

		Position = nullptr;
		Node<Users>* temp_p = ListFromWhere;
		while (temp_p)
		{
			if (temp_p->Data.GetIDNum() == IDNum)
			{
				Position = temp_p;
				return;
			}
			temp_p = temp_p->Next;
		}
	}
	void PageListDisplay()
	{
		int i = 0;
		Node<Users>* temp_p = PageHead;
		if (temp_p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "�û���";
			cout << endl;
			while (temp_p != PageTail)
			{
				temp_p->Data.SetIDNum(++i);
				cout << temp_p->Data;
				temp_p = temp_p->Next;
			}
			temp_p->Data.SetIDNum(++i);
			cout << temp_p->Data;
			temp_p = temp_p->Next;
		}
		else
		{
			cout << "��" << endl;
		}
	}
	void DisplayList(Node<Users>* head)
	{
		Node<Users>* p = head;
		int i = 0;
		if (p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "�û���";
			cout << endl;
			while (p)
			{
				p->Data.SetIDNum(++i);
				cout << p->Data;
				p = p->Next;
			}
		}
		else
		{
			cout << "��" << endl;
		}
	}
	void ContinueSearchUsers(string Name)
	{
		OriHeadCheck();
		DestoryList(SearchResultList);
		NumofSearchResult = 0;
		Node<Users>* temp_p = OriHead;
		while (temp_p)
		{
			if (temp_p->Data.GetName() == Name)
			{
				AddToTail(SearchResultList, temp_p->Data);
				NumofSearchResult++;
			}
			temp_p = temp_p->Next;
		}
	}
	void ContinueSearchUsersFuzzy(string Name)
	{
		OriHeadCheck();
		DestoryList(SearchResultList);
		NumofSearchResult = 0;
		Node<Users>* temp_p = OriHead;
		while (temp_p)
		{
			if (temp_p->Data.GetName().find(Name) != string::npos)
			{
				AddToTail(SearchResultList, temp_p->Data);
				NumofSearchResult++;
			}
			temp_p = temp_p->Next;
		}
	}
	void DeleteFile(string Name)
	{
#if DEBUG	
		if (remove((PSERSONALFILEROAD + Name + ".txt").c_str()))
		{
			cout << "ɾ��ʧ��" << endl;
			system("pause");
		}
		else
		{
			cout << "ɾ���ɹ�" << endl;
			system("pause");
		}
#else
		if (remove((PSERSONALFILEROAD + Name + ".txt").c_str()))
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ����ɾ��ʧ��" << endl;
			cout << "******************************" << endl;
			system("pause");

		}
#endif 
	}
	//���û�������
	void SortUsersList(Node<Users>* head, int ascending)
	{
		if (!head)
		{
			//cout << "Sort���������" << endl;
			//system("pause");
			return;
		}
		if (head->Next == nullptr)
		{
			//cout << "Sort����ֻ��һ��Ԫ�أ�" << endl;
			//system("pause");
			return;
		}
		//ð�ݣ�
		Node<Users>* temp_p = nullptr;
		bool isChange = true;

		if (ascending)
			//����
			while (temp_p != head->Next && isChange)
			{
				Node<Users>* temp_q = head;
				isChange = false;
				for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
				{
					if (zh_CN_less_than(temp_q->Next->Data.GetName(), temp_q->Data.GetName()))
					{
						Users temp = temp_q->Data;
						temp_q->Data = temp_q->Next->Data;
						temp_q->Next->Data = temp;
						isChange = true;
					}
				}
			}
		else
			//����
			while (temp_p != head->Next && isChange)
			{
				Node<Users>* temp_q = head;
				isChange = false;
				for (; temp_q->Next != temp_p; temp_q = temp_q->Next)
				{
					if (zh_CN_less_than(temp_q->Data.GetName(), temp_q->Next->Data.GetName()))
					{
						Users temp = temp_q->Data;
						temp_q->Data = temp_q->Next->Data;
						temp_q->Next->Data = temp;
						isChange = true;
					}
				}
			}
	}
	//�������ļ�
	void RenameFile(string OldName, string NewName)
	{
#if DEBUG
		if (rename((PSERSONALFILEROAD + OldName + ".txt").c_str(), (PSERSONALFILEROAD + NewName + ".txt").c_str()))
		{
			cout << "������ʧ��" << endl;
			system("pause");
		}
		else
		{
			cout << "�������ɹ�" << endl;
			system("pause");
		}
#else
		if (-1 == rename((PSERSONALFILEROAD + OldName + ".txt").c_str(), (PSERSONALFILEROAD + NewName + ".txt").c_str()))
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ����������ʧ��" << endl;
			cout << "******************************" << endl;
			system("pause");
		}
#endif
	}
	void CreateUser(string name)
	{
		ofstream sfile;
		sfile.open(PSERSONALFILEROAD + name + ".txt", ios::out);
		if (!sfile.is_open())
		{
			cout << "******************************" << endl;
			cout << "* ϵͳ���󣺴����û�ʧ��" << endl;
			cout << "******************************" << endl;
			system("pause");
			return;
		}
		sfile.close();
	}
};