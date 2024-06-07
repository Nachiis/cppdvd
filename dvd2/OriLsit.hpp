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
//单向模板链表
template<typename T>
class List
{
public:
	fstream file;//链表所属文件
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
	Node<T>* OriHead;//数据源头指针
	Node<T>* OriTail;//数据源尾指针
	Node<T>* PageHead;//分页头指针
	Node<T>* PageTail;//分页尾指针
	Node<T>* Position;//当前位置指针，此指针可以指向Orihead也可以是Pagehead
	Node<T>* SearchResultList;//搜索结果链表

	int GetCurrentPage() const { return Page; }
	int GetLength() const { return Lenth; }
	int GetMaxPage() const { return MaxPage; }

	void OriHeadCheck()
	{
		if (!OriHead)
		{
			cout << "******************************" << endl;
			cout << "* 系统错误：传入空链表，系统终止！" << endl;
			cout << "******************************" << endl;
			system("pause");
			exit(0);
		}
	}
	//删除特定链表
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
	//根据position删除特定链表的节点
	virtual void DeleteNode(Node<T>*& head, Node<T>*& position)
	{
		if (!head)
		{
			cout << "******************************" << endl;
			cout << "* 系统错误：传入空链表，系统终止！" << endl;
			cout << "******************************" << endl;
			system("pause");
			exit(0);
		}
		if(!position)
		{
			cout << "******************************" << endl;
			cout << "* 系统错误：传入空节点，系统终止！" << endl;
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
		//不管怎么样，都要释放position
	}
	//添加到特定链表表头
	virtual void AddToHead(Node<T>*& head, T val)
	{
		head = new Node<T>(val, head);
		//链表长度+1
		Lenth++;
	}
	//添加到特定链表表尾
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
		//链表长度+1
		Lenth++;
	}
	//文件中构造链表，lenth自动加，返回构造链表节点数量
	virtual void BuildListFromFile(Node<T>*& head, fstream& file)
	{

		T temp;
		while (file >> temp)
			AddToHead(head, temp);
	}
	//构造分页链表
	virtual void BuildPageListFrom(Node<T>*& head)
	{
#if DEBUG
		if (!head)
		{
			cout << "构建分页传入空链表" << endl;
		}
		system("pause");
#endif
		//直接标记分页起始位置和终止位置
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
			cout << "分页链表头指针为空" << endl;
			system("pause");
		}
#endif // DEBUG

		
		PageTail = temp_p;
	}
	//跳转页码
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
	//根据链表和打开的文件写入
	virtual void WriteListToFile(Node<T>* head, fstream& file)
	{

		file.clear();	//清除文件流状态
		file.seekg(0, ios::beg);	//文件指针移动到文件头
		Node<T>* p = head;
		while (p)
		{
			file << p->Data;
			p = p->Next;
		}
		file.clear();	//清除文件流状态
		file.seekg(0, ios::beg);	//文件指针移动到文件头
	}
	//计算最大页数
	virtual void CulcMaxPage()
	{
		if (Lenth % ShowNum)
			MaxPage = Lenth / ShowNum + 1;
		else
			MaxPage = Lenth / ShowNum;
	}
	//设置页数
	virtual void SetPage(int PageNum){Page = PageNum;}
protected:
	int NumofSearchResult;
	int ShowNum;//每页显示数量
	int Page;//当前页数
	int MaxPage;//最大页数
	int Lenth;//数据源长度
};
//DVD库存链表
class DVDStockList :public List<DVDStock>
{
public:
	

	DVDStockList(int ShowNum) :
		List(ShowNum)
	{
		//打开文件
		file.open("DVD.txt", ios::in);
#if DEBUG
		if(file.is_open())
			cout << "DVDStockList打开成功" << endl;
		else
			cout << "DVDStockList打开失败" << endl;
		system("pause");
#endif
		//构造数据源链表，设置链表长度
		BuildListFromFile(OriHead, file);
		Page = 1;
		CulcMaxPage();
		//构造分页链表
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
	//获取搜索结果数量
	int GetNumofSerchResult(){return NumofSearchResult;}
	//根据影碟名连续查找，返回查找到的链表
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
	//根据影碟名连续模糊搜索
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
	//根据影碟名单次查找，返回到position
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
	//根据序号查找，position会跳到Orihead，Pagehead，SearchResult链表
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
	//对给定链表和关键词进行排序
	void SortDVDList(Node<DVDStock>* head, STOCKDVDKEY key, int ascending)
	{
		if (!head)
		{
#if DEBUG
			cout << "Sort传入空链表！" << endl;
			system("pause");
#endif // DEBUG
			return;
		}
		if (!(head->Next))
		{
#if DEBUG
			cout << "Sort链表只有一个元素！" << endl;
#endif // DEBUG
			return;
		}
		//冒泡！
		Node<DVDStock>* temp_p = nullptr;
		bool isChange = true;
		//排序
		switch (key)
		{
		case Name:
			//冒泡
			if (ascending)
				//升序
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
				//降序
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
			//排序
			if (ascending)
				//升序
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
				//降序
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
			//排序
			if (ascending)
				//升序
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
				//降序
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
			//排序
			if (ascending)
				//升序
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
				//降序
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
			//排序
			if (ascending)
				//升序
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
				//降序
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
			cout << "* 系统错误：未知关键字" << endl;
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
			cout << setw(16) << left << "影碟名";
			cout << setw(16) << left << "国家";
			cout << setw(16) << left << "类型";
			cout << setw(16) << left << "出版时间";
			cout << setw(16) << left << "库存数量";
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
			cout << "无" << endl;
		}
	}

	void PageListDisplay()
	{
		int i = 0;
		Node<DVDStock>* temp_p = PageHead;
		if (temp_p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "影碟名";
			cout << setw(16) << left << "国家";
			cout << setw(16) << left << "类型";
			cout << setw(16) << left << "出版时间";
			cout << setw(16) << left << "库存数量";
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
			cout << "无" << endl;
		}
	}
};

//用户借阅的链表，这个比较特殊，由于只有知道了用户名才能构建链表，所以构造函数中不传入用户名
class DVDBorrowList :public List<BorrowDVD>
{
public:
	DVDBorrowList(int ShowNum):
		List(ShowNum){}
	void Init(string Name)
	{
		Usersname = Name;
		//打开文件
		file.open(PSERSONALFILEROAD + Usersname + ".txt", ios::in);
#if DEBUG
		if (file.is_open())
		{
			cout << "打开成功" << endl;
		}
			
		else
			cout << "打开失败" << endl;
		system("pause");
#endif
		BuildListFromFile(OriHead, file);
#if DEBUG
		if(OriHead)
			cout << "构建成功" << endl;
		else
			cout << "构建失败" << endl;
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
	//根据string Name，用于查重
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
	//根据序号查找
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
	//借阅DVD链表输出
	void DisplayList(Node<BorrowDVD>* head)
	{
		Node<BorrowDVD>* p = head;
		int i = 0;
		if (p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "影碟名";
			cout << setw(16) << left << "国家";
			cout << setw(16) << left << "类型";
			cout << setw(16) << left << "出版时间";
			cout << setw(16) << left << "借阅时间";
			cout << setw(16) << left << "归还时间";
			cout << setw(16) << left << "归还租金";
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
			cout << "无" << endl;
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
		//打开文件
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
	//根据name查找，用于登录，管理员创建用户的时候查重
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
			cout << setw(16) << left << "用户名";
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
			cout << "无" << endl;
		}
	}
	void DisplayList(Node<Users>* head)
	{
		Node<Users>* p = head;
		int i = 0;
		if (p)
		{
			cout << "* " << setw(4) << left << ">>>  ";
			cout << setw(16) << left << "用户名";
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
			cout << "无" << endl;
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
			cout << "删除失败" << endl;
			system("pause");
		}
		else
		{
			cout << "删除成功" << endl;
			system("pause");
		}
#else
		if (remove((PSERSONALFILEROAD + Name + ".txt").c_str()))
		{
			cout << "******************************" << endl;
			cout << "* 系统错误：删除失败" << endl;
			cout << "******************************" << endl;
			system("pause");

		}
#endif 
	}
	//按用户名排序
	void SortUsersList(Node<Users>* head, int ascending)
	{
		if (!head)
		{
			//cout << "Sort传入空链表！" << endl;
			//system("pause");
			return;
		}
		if (head->Next == nullptr)
		{
			//cout << "Sort链表只有一个元素！" << endl;
			//system("pause");
			return;
		}
		//冒泡！
		Node<Users>* temp_p = nullptr;
		bool isChange = true;

		if (ascending)
			//升序
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
			//降序
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
	//重命名文件
	void RenameFile(string OldName, string NewName)
	{
#if DEBUG
		if (rename((PSERSONALFILEROAD + OldName + ".txt").c_str(), (PSERSONALFILEROAD + NewName + ".txt").c_str()))
		{
			cout << "重命名失败" << endl;
			system("pause");
		}
		else
		{
			cout << "重命名成功" << endl;
			system("pause");
		}
#else
		if (-1 == rename((PSERSONALFILEROAD + OldName + ".txt").c_str(), (PSERSONALFILEROAD + NewName + ".txt").c_str()))
		{
			cout << "******************************" << endl;
			cout << "* 系统错误：重命名失败" << endl;
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
			cout << "* 系统错误：创建用户失败" << endl;
			cout << "******************************" << endl;
			system("pause");
			return;
		}
		sfile.close();
	}
};