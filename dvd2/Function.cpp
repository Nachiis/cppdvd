#include "Inc.h"
#include "Node.hpp"
#include "Users.hpp"
#include "DVD.hpp"
#include "OriLsit.hpp"
#include "Function.h"
int ListShowNum = 15;//每页显示的数量
DVDStockList _DVDStock(ListShowNum);
DVDBorrowList _DVDBorrow(ListShowNum);
UsersList _Users(ListShowNum);
Time NowTime;
//=============窗口功能函数=============
#pragma region Function
//清屏函数，参数表示需要删除的字符数，0表示清屏
void WindowsClear(int Num = 0)
{
	if (Num)
		while (Num--)
			cout << "\b \b";
	else
		system("cls");
}
//初始化获取当前时间
bool Init(){NowTime.SetCurrentTime();return true;}
/*输入整数，两个参数表示命令范围，输入字符会令整型为0，所以满足b >= a >= 1*/
int CinCMD(int a,int b)
{
	int cmd;
	cin >> cmd;
	while (cmd > b || cmd < a)
	{
		cout << "******************************" << endl;
		cout << "* 输入错误，请重试";
		Sleep(500);
		WindowsClear(9*2);
		// 清空输入流
		cin.clear();
		while (cin.get()!='\n');
		cout << "*>>>";
		cin >> cmd;
	}
	return cmd;
}
//重复输入
int RetryWindows(int(*WindowsFunction)(void))
{
	//当调用的窗口返回0时，说明当前窗口退出
	while(WindowsFunction());
	//临时返回值
	return 0;
}
void ClearInputBuffer()
{
	// 清除输入缓冲区中的残余数据
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
#pragma endregion
//=============窗口=============
#pragma region Windows
//用户欢迎界面
int UserWelcomeMenu()
{
	WindowsClear();
	
	int CMD;

	cout << "******************************" << endl;
	cout << "* 欢迎用户:" << _Users.Position->Data.GetName() << endl;
	cout << "* 当前时间：" << NowTime << endl;
	cout << "******************************" << endl;
	cout << "* 温馨提示：请按照提示输入命令" << endl;
	cout << "******************************" << endl;
	cout << "* 1.查看个人信息" << endl;
	cout << "* 2.借阅影碟 (归还请到个人信息中查看)" << endl;
	cout << "* 3.退出登录" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, 3);
	ClearInputBuffer();
	//临时返回值
	switch (CMD)
	{
	case 1:
		//查看个人信息
		RetryWindows(UserInfomation);
		return 1;
	case 2:
		//借阅影碟，既然是进去，那么默认页码是1
		_DVDStock.SetPage(1);
		_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
		RetryWindows(BorrowDVDMenu);
		return 1;
	case 3:
		//退出登录
		return 0;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}
//添加用户界面
int AddUsersMenu()
{
	WindowsClear();
	string Name;
	Users* nowlogin = &_Users.Position->Data;
	Users temp;
	int tempState;
	int CMD;
	cout << "******************************" << endl;
	cout << "* 请输入用户名：" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Name;
	if (Name == "0")
	{
		cout << "******************************" << endl;
		cout << "* 用户名不允许为0" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	temp.SetName(Name);
	ClearInputBuffer();
	//查找是否存在
	_Users.SingleSearchUsers(_Users.OriHead, Name);
	if (_Users.Position)
	{
		cout << "******************************" << endl;
		cout << "* 用户已存在" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		temp.SetName(Name);
		cout << "* 请输入用户类型：(1/2 管理员/用户)" << endl;
		cout << "* >>>";
		tempState = CinCMD(1, 2);
		temp.SetState(tempState);
		temp.SetPassword(DEFAULTPASSWORD);
		ClearInputBuffer();
		cout << "******************************" << endl;
		cout << "* 请确认以下信息，影碟将会被按照如下信息被添加" << endl;
		cout << "* 用户名：" << temp.GetName() << endl;
		cout << "* 用户类型：";
		if (temp.GetState() == 1)
			cout << "管理员" << endl;
		else
			cout << "用户" << endl;
		cout << "******************************" << endl;
		cout << "* 确认添加？" << endl;
		cout << "* 1.确认" << endl;
		cout << "* 2.取消" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//确认添加
			_Users.AddToHead(_Users.OriHead, temp);//链表自动长度增加
			//重置页码，重新构造分页链表
			_Users.SetPage(1);
			_Users.BuildPageListFrom(_Users.OriHead);
			//position重新定位
			_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
			if(tempState == 2)
				_Users.CreateUser(Name);
				
			cout << "******************************" << endl;
			cout << "* 添加成功" << endl;
			cout << "* 默认密码为：" << DEFAULTPASSWORD << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		case 2:
			//取消
			return 0;
		default:
			cout << "你是怎么进来的？" << endl;
			system("pause");
			return 0;
		}
	}
}
//要注意，当进入这个界面的时候，position就已经指向了要查看的用户，当删除这个用户的时候，如果position没有
//重新定位，那么position就会指空，导致this指针无法访问现在正在登录的用户，所以在删除用户的时候要重新定位position
//删除用户界面
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
		//给定序号IDNum查找
		_Users.SingleSearchUsers(ListFromWhere, Order);
		//对ListFromWhere进行判断，防止越界
		//传入pagehead，则只能是页码来的，传入searchresultlist，则只能是搜索结果来的
		if ((ListFromWhere == _Users.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListFromWhere == _Users.SearchResultList && (Order <= 0 || Order > _DVDStock.GetNumofSerchResult())))
		{
			cout << "******************************" << endl;
			cout << "* 请不要超出序号" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if (_Users.Position)
		{
			if (_Users.Position->Data.GetName() == nowlogin->GetName())
			{
				cout << "******************************" << endl;
				cout << "* 不能删除当前登录用户" << endl;
				cout << "******************************" << endl;
				system("pause");
				//position重新定位
				_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
				return 0;
			}
			cout << "******************************" << endl;
			cout << "* 您要删除的用户为：" << endl;
			cout << "* " << _Users.Position->Data.GetName() << endl;
			cout << "******************************" << endl;
			cout << "* 确认删除？" << endl;
			cout << "* 1.确认" << endl;
			cout << "* 2.取消" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//删除
				tempname = _Users.Position->Data.GetName();
				if (ListFromWhere == _Users.PageHead)
					//删除分页链表的节点，同时也删除了Orihead里的节点
					_Users.DeleteNode(_Users.PageHead, _Users.Position);
				else if (ListFromWhere == _Users.SearchResultList)//鬼知道会不会传了个什么链表进来
				{
					//得先删除对应的Orihead里的，不然把position指向的SearchResultList删了就找不到了
					//删除模糊搜索结果链表
					_Users.DeleteNode(_Users.SearchResultList, _Users.Position);
					//删除原始链表
					_Users.SingleSearchUsers(_Users.OriHead, tempname);
					_Users.DeleteNode(_Users.OriHead, _Users.Position);
				}
				//文件删
				_Users.DeleteFile(tempname);
				//将position重新定位
				_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
				//重置页码，重新构造分页链表
				_DVDStock.SetPage(1);
				_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
				cout << "******************************" << endl;
				cout << "* 删除成功" << endl;
				cout << "******************************" << endl;
				
				system("pause");
				break;
			case 2:
				//取消
				break;
			default:
				cout << "你是怎么进来的？" << endl;
				system("pause");
				return 0;
			}
		}
		else
		{
			//明明防止越界了，又是有搜索结果的，还是找不到？
			cout << "******************************" << endl;
			cout << "* 系统不知为何未找到该用户" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* 您没有删除任何用户" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* 最后一次删除的用户序号为：" << Order << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}

//mode = 1精确搜索，mode = 2模糊搜索，能进行搜索用户的一定是管理员
//模糊精确搜索用户
int A_FSearchUsersResultMenu(int mode, string Key)
{
	WindowsClear();
	int CMD;
	if (mode == 1)
		_Users.ContinueSearchUsers(Key);
	else
		_Users.ContinueSearchUsersFuzzy(Key);
	cout << "******************************" << endl;
	cout << "* 搜索结果：" << _Users.GetNumofSerchResult() << endl;
	_Users.DisplayList(_Users.SearchResultList);
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	cout << "* 2.删除 + 影碟前面的序号" << endl;
	cout << "* 3.更改用户名 + 用户前面的序号 + 新用户名" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";

	CMD = CinCMD(1, 3);
	switch (CMD)
	{
	case 1:
		//返回
		_Users.DestoryList(_Users.SearchResultList);
		return 0;
	case 2:
		//删除
		DeleteUsersMenu(_Users.SearchResultList);//看这里传的是搜索结果链表，不是PageHead
		return 1;
	case 3:
		//更改用户名
		ChangeUsersName(_Users.SearchResultList);//看这里传的是搜索结果链表，不是PageHead
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}
//更改用户名
int ChangeUsersName(Node<Users>* ListFromWhere)
{
	WindowsClear();
	int CMD;
	int Order = 0;
	string tempname;
	string oldname;
	Users* nowlogin = &_Users.Position->Data;
	cin >> Order;
	//给定序号IDNum查找
	_Users.SingleSearchUsers(ListFromWhere, Order);//这里position跳走了，后面要跳回来
	//对ListFromWhere进行判断，防止越界
	//传入pagehead，则只能是页码来的，传入searchresultlist，则只能是搜索结果来的
	if ((ListFromWhere == _Users.PageHead && (Order <= 0 || Order > ListShowNum)) ||
		(ListFromWhere == _Users.SearchResultList && (Order <= 0 || Order > _Users.GetNumofSerchResult())))
	{
		cout << "******************************" << endl;
		cout << "* 请不要超出序号" << endl;
		cout << "* 请重新选择" << endl;
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
			cout << "* 用户名不允许为0" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		oldname = _Users.Position->Data.GetName();
		cout << "******************************" << endl;
		cout << "* 您要修改的用户为：";
		cout << "* " << oldname << endl;
		cout << "* 即将修改为：";
		cout << "* " << tempname << endl;
		cout << "******************************" << endl;
		cout << "* 确认修改？" << endl;
		cout << "* 1.确认" << endl;
		cout << "* 2.取消" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//修改
			if (ListFromWhere == _Users.PageHead)
			{
				//修改分页链表的节点，同时也修改了Orihead里的节点
				_Users.Position->Data.SetName(tempname);
			}
			else if (ListFromWhere == _Users.SearchResultList)//鬼知道会不会传了个什么链表进来
			{
				//修改搜索结果链表
				_Users.Position->Data.SetName(tempname);
				//修改原始链表
				_Users.SingleSearchUsers(_Users.OriHead, oldname);
				_Users.Position->Data.SetName(tempname);
			}
			//文件改名，只有普通用户才有文件
			if (_Users.Position->Data.GetState() == VIP)
				_Users.RenameFile(oldname, tempname);
			cout << "******************************" << endl;
			cout << "* 修改成功" << endl;
			cout << "******************************" << endl;
			//重置页码，重新构造分页链表
			_DVDStock.SetPage(1);
			_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
			//将position重新定位
			_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
			system("pause");
			return 0;
		case 2:
			//取消
			return 0;
		default:
			cout << "你是怎么进来的？" << endl;
			system("pause");
			return 0;
		}
	}
	else
	{
		//明明防止越界了，又是有搜索结果的，还是找不到？
		cout << "******************************" << endl;
		cout << "* 系统不知为何未找到该用户" << endl;
		cout << "* 请重新选择" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* 您没有更改任何用户" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}
//重置密码
void ResetPassword()
{
	WindowsClear();
	int Order = 0;
	Users* nowlogin = &_Users.Position->Data;
	cin >> Order;
	ClearInputBuffer();
	//给定序号IDNum查找
	_Users.SingleSearchUsers(_Users.PageHead, Order);
	if (_Users.Position)
	{
		_Users.Position->Data.SetPassword(DEFAULTPASSWORD);
		cout << "******************************" << endl;
		cout << "* 重置成功" << endl;
		cout << "* 密码已重置为：" << DEFAULTPASSWORD << endl;
		cout << "******************************" << endl;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* 未知序号" << endl;
		cout << "******************************" << endl;
	}
	system("pause");
	//position重新定位
	_Users.SingleSearchUsers(_Users.OriHead, nowlogin->GetName());
}

//管理用户主界面
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
	cout << "*        管理用户" << endl;
	cout << "******************************" << endl;
	cout << "* 当前用户数量：" << _Users.GetLength() << " 当前页码：" << _Users.GetCurrentPage() << endl;
	cout << "* 共：" << _Users.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_Users.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	cout << "* 2.精确搜索（用户名）+ 关键词" << endl;
	cout << "* 3.模糊搜索（用户名）+ 关键词" << endl;
	cout << "* 4.排序    （用户名） + 1/0 升/降序" << endl;
	cout << "* 5.添加用户" << endl;
	cout << "* 6.删除用户 + 用户前面的序号 (多个用空格隔开)" << endl;
	cout << "* 7.重置密码 + 用户前面的序号" << endl;
	cout << "* 8.更改用户名 + 用户前面的序号 + 新用户名" << endl;
	//判断是否有下一页或者上一页
	if (_Users.GetCurrentPage() == 1 && _Users.GetCurrentPage() != _Users.GetMaxPage())
	{
		//存在下一页面
		cout << "* 9.下一页" << endl;
		CMDRange = 9;
	}
	else if (_Users.GetCurrentPage() != 1 && _Users.GetCurrentPage() == _Users.GetMaxPage())
	{
		//存在上一页
		cout << "* 9.上一页" << endl;
		CMDRange = 9;
	}
	else if (_Users.GetCurrentPage() != 1 && _Users.GetCurrentPage() != _Users.GetMaxPage())
	{
		cout << "* 9.下一页" << endl;
		cout << "* 10.上一页" << endl;
		CMDRange = 10;
	}
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);
	switch (CMD)
	{
	case 1:
		//返回
		ClearInputBuffer();
		return 0;
	case 2:
		//精确搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchUsersResultMenu(1, Key);
		return 1;
	case 3:
		//模糊搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchUsersResultMenu(2, Key);
		return 1;
	case 4:
		//排序，排完之后找回position
		cin >> SortMode;
		ClearInputBuffer();
		_Users.SortUsersList(_Users.OriHead, SortMode);
		_Users.SingleSearchUsers(_Users.OriHead, Nowname);
		return 1;
	case 5:
		//添加用户
		AddUsersMenu();
		return 1;
	case 6:
		//删除用户，删完找回position
		DeleteUsersMenu(_Users.PageHead);
		return 1;
	case 7:
		//重置密码
		ResetPassword();
		return 1;
	case 8:
		//更改用户名
		ChangeUsersName(_Users.PageHead);
		return 1;
	case 9:
		//下一页
		if (_Users.GetCurrentPage() != _Users.GetMaxPage())
		{
			_Users.PageNext(_Users.OriHead);
			return 1;
		}
		//这里没有break或者return。所以当不满足if的时候就下一页
	case 10:
		//上一页
		_Users.PageLast(_Users.OriHead);
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//删除影碟界面,ListHeadfromDVDWhere不一定是分页链表，也可能是搜索结果链表
int DeleteDVDMenu(Node<DVDStock>*ListFromWhere)
{
	WindowsClear();
	int CMD;
	int Order = 0;
	while (getchar() != '\n')
	{
		cin >> Order;
		//给定序号IDNum查找
		_DVDStock.SingleSearchDVDStock(ListFromWhere, Order);
		//对ListFromWhere进行判断，防止越界
		//传入pagehead，则只能是页码来的，传入searchresultlist，则只能是搜索结果来的
		if ((ListFromWhere == _DVDStock.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListFromWhere == _DVDStock.SearchResultList && (Order <= 0 || Order > _DVDStock.GetNumofSerchResult())))
		{
			cout << "******************************" << endl;
			cout << "* 请不要超出序号" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if (_DVDStock.Position)
		{
			cout << "******************************" << endl;
			cout << "* 您要删除的影碟为：" << endl;
			cout << "* " << _DVDStock.Position->Data.GetName() << endl;
			cout << "******************************" << endl;
			cout << "* 确认删除？" << endl;
			cout << "* 1.确认" << endl;
			cout << "* 2.取消" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//删除
				if (ListFromWhere == _DVDStock.PageHead)
					//删除分页链表的节点，同时也删除了Orihead里的节点
					_DVDStock.DeleteNode(_DVDStock.PageHead, _DVDStock.Position);
				else if (ListFromWhere == _DVDStock.SearchResultList)//鬼知道会不会传了个什么链表进来
				{
					string tempname = _DVDStock.Position->Data.GetName();
					//得先删除对应的Orihead里的，不然把position指向的SearchResultList删了就找不到了
					//删除模糊搜索结果链表
					_DVDStock.DeleteNode(_DVDStock.SearchResultList, _DVDStock.Position);
					//删除原始链表
					_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, tempname);
					_DVDStock.DeleteNode(_DVDStock.OriHead, _DVDStock.Position);
				}
				cout << "******************************" << endl;
				cout << "* 删除成功" << endl;
				cout << "******************************" << endl;
				//重置页码，重新构造分页链表
				_DVDStock.SetPage(1);
				_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
				system("pause");
				break;
			case 2:
				//取消
				break;
			default:
				cout << "你是怎么进来的？" << endl;
				system("pause");
				return 0;
			}	
		}
		else
		{
			//明明防止越界了，又是有搜索结果的，还是找不到？
			cout << "******************************" << endl;
			cout << "* 系统不知为何未找到该影碟" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
	}
	if (Order == 0)
	{
		cout << "******************************" << endl;
		cout << "* 您没有删除任何影碟" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* 最后一次删除的影碟序号为：" << Order << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
}
//添加影碟菜单，这得来一个新菜单，因为添加影碟需要输入很多信息
int AddDVDMenu()
{
	WindowsClear();
	string Val;
	DVDStock temp;
	Time tempTime;
	int StockNum;
	int CMD;
	cout << "******************************" << endl;
	cout << "* 若库存中有，则视作进货，本系统暂不支持同名影碟" << endl;
	cout << "* 请想输入影碟名：" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Val;
	temp.SetName(Val);
	ClearInputBuffer();
	//查找是否存在
	_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, Val);
	if(_DVDStock.Position)
	{
		cout << "******************************" << endl;
		cout << "* 影碟已存在，请直接输入添加的数量：" << endl;
		cout << "******************************" << endl;
		cin >> StockNum;
		ClearInputBuffer();
		_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() + StockNum);
		cout << "******************************" << endl;
		cout << "* 设置成功" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout << "******************************" << endl;
		cout << "* 即将添加新影碟：" << Val << endl;
		cout << "* 请输入国家：" << endl;
		cout << "* >>>";
		cin >> Val;
		temp.SetNation(Val);
		ClearInputBuffer();
		cout << "* 请输入类型：" << endl;
		cout << "* >>>";
		cin >> Val;
		temp.SetType(Val);
		ClearInputBuffer();
		cout << "* 请输入出版时间（年 月 日 用空格隔开）：" << endl;
		cout << "* >>>";
		cin >> tempTime;//重载了输入运算符，所以可以直接输入，且有输入数据检查
		ClearInputBuffer();
		temp.SetPublicTime(tempTime);//默认赋值运算足矣
		cout << "* 请输入本次添加的数量：" << endl;
		cout << "* >>>";
		cin >> StockNum;
		if (StockNum < 0)
		{
			cout << "******************************" << endl;
			cout << "* 请不要输入负数，已自动修正为0" << endl;
			cout << "******************************" << endl;
			StockNum = 0;
		}
		ClearInputBuffer();
		temp.SetNum(StockNum);
		cout << "******************************" << endl;
		cout << "* 请确认以下信息，影碟将会被按照如下信息被添加" << endl;
		cout << "* 影碟名：" << temp.GetName() << endl;
		cout << "* 国家：" << temp.GetNation() << endl;
		cout << "* 类型：" << temp.GetType() << endl;
		cout << "* 出版时间：" << temp.GetPublicTime() << endl;
		cout << "* 数量：" << temp.GetNum() << endl;
		cout << "******************************" << endl;
		cout << "* 确认添加？" << endl;
		cout << "* 1.确认" << endl;
		cout << "* 2.取消" << endl;
		cout << "******************************" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
		switch (CMD)
		{
		case 1:
			//确认添加
			_DVDStock.AddToHead(_DVDStock.OriHead, temp);//链表自动长度增加
			//重置页码，重新构造分页链表
			_DVDStock.SetPage(1);
			_DVDStock.BuildPageListFrom(_DVDStock.OriHead);
			cout << "******************************" << endl;
			cout << "* 添加成功" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		case 2:
			//取消
			return 0;
		default:
			cout << "你是怎么进来的？" << endl;
			system("pause");
			return 0;
		}
	}
}

//管理影碟主界面，影碟详细信息
int ArrangeDVDMenu()
{
	WindowsClear();
	int CMD;
	int CMDRange = 6;		//当不存在任何页面跳转时，只有6个选项
	int TypeKey = 0;
	int SortMode = 0;
	string Key;
	cout << "******************************" << endl;
	cout << "*        影碟" << endl;
	cout << "******************************" << endl;
	cout << "* 当前影碟数量：" << _DVDStock.GetLength() << " 当前页码：" << _DVDStock.GetCurrentPage() << endl;
	cout << "* 共：" << _DVDStock.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_DVDStock.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	cout << "* 2.精确搜索（影碟名）+ 关键词" << endl;
	cout << "* 3.模糊搜索（影碟名）+ 关键词" << endl;
	cout << "* 4.排序 + 分类词序号 + 1/0 升/降序" << endl;
	cout << "* 5.添加影碟" << endl;
	cout << "* 6.删除影碟 + 影碟前面的序号 (多个用空格隔开)" << endl;

	//if这样判断是因为有可能压根结果不满一页，所以不需要换页
	if (_DVDStock.GetCurrentPage() == 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		//存在下一页面
		cout << "* 7.下一页" << endl;
		CMDRange = 7;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() == _DVDStock.GetMaxPage())
	{
		//存在上一页
		cout << "* 7.上一页" << endl;
		CMDRange = 7;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		cout << "* 7.下一页" << endl;
		cout << "* 8.上一页" << endl;
		CMDRange = 8;
	}
	cout << "******************************" << endl;
	cout << "* 分类词序号：" << endl;
	cout << "* 1.影碟名 2.国家 3.类型 4.出版年份 5.库存数量" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);

	switch (CMD)
	{
	case 1:
		//返回
		ClearInputBuffer();
		return 0;
	case 2:
		//精确搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(1, Key, 1);
		return 1;
	case 3:
		//模糊搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(2, Key, 1);
		return 1;
	case 4:
		//排序
		cin >> TypeKey >> SortMode;
		ClearInputBuffer();
		_DVDStock.SortDVDList(_DVDStock.OriHead, (STOCKDVDKEY)TypeKey, SortMode);
		return 1;
	case 5:
		//添加影碟
		ClearInputBuffer();
		AddDVDMenu();
		return 1;
	case 6:
		//删除影碟
		DeleteDVDMenu(_DVDStock.PageHead);
		return 1;
	case 7:
		//下一页
		if (_DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
		{
			_DVDStock.PageNext(_DVDStock.OriHead);
			return 1;
		}
		//这里没有break或者return。所以当不满足if的时候就下一页
	case 8:
		//上一页
		_DVDStock.PageLast(_DVDStock.OriHead);
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//查看管理员个人信息界面
int AdminInfomation()
{
	WindowsClear();

	int CMD;
	cout << "******************************" << endl;
	cout << "*        个人信息" << endl;
	cout << "******************************" << endl;
	cout << "* 用户名：" << _Users.Position->Data.GetName() << endl;
	cout << "* 密  码：" << _Users.Position->Data.GetPassword() << endl;
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	cout << "* 2.修改密码" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, 2);
	ClearInputBuffer();
	switch (CMD)
	{
	case 1:
		//返回
		return 0;
	case 2:
		//修改密码
		ChangeUserPassword();
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}
//管理员欢迎界面
int AdminMenu()
{
	WindowsClear();

	int CMD;

	cout << "******************************" << endl;
	cout << "* 欢迎管理员:" << _Users.Position->Data.GetName() << endl;
	cout << "* 当前时间：" << NowTime << endl;
	cout << "******************************" << endl;
	cout << "* 当前影碟数量：" << _DVDStock.GetLength() << endl;
	cout << "* 当前用户数量：" << _Users.GetLength() << endl;
	cout << "******************************" << endl;
	cout << "* 温馨提示：请按照提示输入命令" << endl;
	cout << "******************************" << endl;
	cout << "* 1.查看个人信息" << endl;
	cout << "* 2.查看用户信息" << endl;
	cout << "* 3.查看影碟信息" << endl;
	cout << "* 4.退出登录" << endl;
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
		//查看详细影碟信息
		RetryWindows(ArrangeDVDMenu);
		return 1;
		break;
	case 4:
		return 0;
		break;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//登录函数，返回登录用户信息
int Login()
{
	//清屏，然后构造用户链表用于判断用户是否存在及信息
	WindowsClear();
	//输入用户密码并不需要判断数据合法性，因为用户输入错误会在后面判断
	string Name, Password;

	cout << "******************************" << endl;
	cout << "* 忘记密码请联系管理员重置密码" << endl;
	cout << "******************************" << endl;
	cout << "* 请输入用户名：(输入0退出)" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Name;
	if (Name == "0")
	{
		return 0;
	}
	cout << "******************************" << endl;
	cout << "* 请输入密码：" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> Password;
	//搜索用户
	_Users.SingleSearchUsers(_Users.OriHead,Name);

	if (_Users.Position && _Users.Position->Data.GetPassword() == Password)
	{
		//判断用户身份
		if (_Users.Position->Data.GetState() == ADMIN)
		{
			//管理员
			ClearInputBuffer();
			//不管是管理员还是用户都是在退出系统的时候才保存库存和用户信息
			RetryWindows(AdminMenu);
			return 1;
		}
		else
		{
			//普通用户
			ClearInputBuffer();
			//初始化用户借阅链表
			_DVDBorrow.Init(_Users.Position->Data.GetName());
			//进入用户欢迎界面
			RetryWindows(UserWelcomeMenu);
			_DVDBorrow.BorrowDVDSave();
			return 1;
		}
	}
	else
	{
		cout << "******************************" << endl;
		cout << "*    输入的密码或用户名错误  *" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 1;
	}
}

//主页面
int OptionBegin()
{
	WindowsClear();
	cout << "******************************" << endl;
	cout << "*        影碟管理系统        *" << endl;
	cout << "******************************" << endl;
	cout << "* 1.登录 " << endl;
	cout << "* 2.退出系统" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	int CMD = CinCMD(1, 2);
	ClearInputBuffer();
	switch (CMD)
	{
	case 1:
		//登录
		RetryWindows(Login);
		return 1;
	case 2:
		//exit后自动析构，但是手动保存数据
		_Users.UsersSave();
		_DVDStock.DVDStockSave();
		WindowsClear();
		cout << "感谢使用！" << endl;
		exit(0);
		return 0;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//归还影碟界面
int ReternBorrowedDVDMenu()
{
	//借阅的序号
	int Order = 0;
	cout << "******************************" << endl;
	cin >> Order;
	ClearInputBuffer();
	if(Order > _DVDBorrow.GetLength() || Order <= 0)
	{
		cout << "******************************" << endl;
		cout << "* 请不要超出序号" << endl;
		cout << "* 请重新选择" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	_DVDBorrow.SingleSearchDVDBorrow(Order);
	if (_DVDBorrow.Position)
	{
		cout << "* 您要归还的影碟为：" << endl;
		cout << "* " << _DVDBorrow.Position->Data.GetName() << endl;
		//归还加入库存
		_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead, _DVDBorrow.Position->Data.GetName());
		if (!_DVDStock.Position)
		{
			cout << "******************************" << endl;
			cout << "* 该影碟已被管理员删除，需要移除该影碟借阅记录吗？" << endl;
			cout << "******************************" << endl;
			cout << "* 1.是" << endl;
			cout << "* 2.否" << endl;
			cout << "******************************" << endl;
			cout << "*>>>";
			int CMD = CinCMD(1, 2);
			switch (CMD)
			{
			case 1:
				//删除
				_DVDBorrow.DeleteNode(_DVDBorrow.OriHead, _DVDBorrow.Position);
				cout << "******************************" << endl;
				cout << "* 移除记录成功!" << endl;
				cout << "******************************" << endl;
				system("pause");
			case 2:
				return 0;
			default:
				cout << "你是怎么进来的？" << endl;
				system("pause");
				return 0;
			}
		}
		_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() + 1);
		_DVDBorrow.DeleteNode(_DVDBorrow.OriHead, _DVDBorrow.Position);
		cout << "* 归还成功!" << endl;
		cout << "******************************" << endl;
		system("pause");
		return 0;
	}
	else
	{
		cout <<"******************************" << endl;
		cout << "* 系统在归还影碟处发生未知错误" << endl;
		cout << "******************************" << endl;
		system("pause");
		exit(0);
	}
}

//未及时归还影碟提醒
void OverTimeRemind()
{
	Node<BorrowDVD>* p = _DVDBorrow.OriHead;
	while (p)
	{
		if(p->Data.GetReturnTime() < NowTime)
		{
			int day = NowTime - p->Data.GetReturnTime();
			cout << "******************************" << endl;
			cout << "* 影碟：" << p->Data.GetName() << "已经超时归还" << day << "天" << endl;
			cout << "* 请尽快归还" << endl;
			cout << "* 因为超时归还，您需要支付" << RentOverMoney(day) << "元" << endl;
		}
		p = p->Next;
	}
}
//用户修改密码界面
int ChangeUserPassword()
{
	WindowsClear();
	string NewPassword;
	cout << "******************************" << endl;
	cout << "* 请输入新密码：" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	cin >> NewPassword;
	_Users.Position->Data.SetPassword(NewPassword);
	cout << "******************************" << endl;
	cout << "* 密码修改成功" << endl;
	cout << "******************************" << endl;
	system("pause");
	return 0;
}

//查看个人信息界面
int UserInfomation()
{
	WindowsClear();
	int CMD;
	cout << "******************************" << endl;
	cout << "*        个人信息" << endl;
	cout << "******************************" << endl;
	cout << "* 用户名：" << _Users.Position->Data.GetName() << endl;
	cout << "* 密  码：" << _Users.Position->Data.GetPassword() << endl;
	cout << "******************************" << endl;
	cout << "* 影碟借阅信息：" << _DVDBorrow.GetLength() << endl;
	cout << "******************************" << endl;
	_DVDBorrow.DisplayList(_DVDBorrow.OriHead);
	OverTimeRemind();
	cout << "******************************" << endl;

	if (_DVDBorrow.OriHead)
	{
		cout << "* 1.返回" << endl;
		cout << "* 2.修改密码（请联系管理员修改用户名）" << endl;
		cout << "* 3.归还影碟（+ 输入影碟前面的序号）" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 3);
	}
	else
	{
		cout << "* 1.返回" << endl;
		cout << "* 2.修改密码（请联系管理员修改用户名）" << endl;
		cout << "*>>>";
		CMD = CinCMD(1, 2);
	}
	switch (CMD)
	{
	case 1:
		//返回
		ClearInputBuffer();
		return 0;
	case 2:
		//修改密码
		ClearInputBuffer();
		ChangeUserPassword();
		return 1;
	case 3:
		//归还影碟
		ReternBorrowedDVDMenu();
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//借阅！ListHeadfromDVDWhere不一定是分页链表，也可能是搜索结果链表
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
			cout << "* 请不要输入不合适的天数哦~" << endl;
			cout << "* 本次借阅失败" << endl;
			cout << "* 请重新尝试" << endl;
			system("pause");
			return 0;
		}
		//如果传入的是pagehead，说明是从BorrowedDVDMenu来的，那么只能是页码来的，
		//如果传入的是SearchResultList，说明是搜索过后来的，那么就要和GetNumofSerchResult比较
		if ((ListHeadfromDVDWhere == _DVDStock.PageHead && (Order <= 0 || Order > ListShowNum)) ||
			(ListHeadfromDVDWhere == _DVDStock.SearchResultList && (Order > _DVDStock.GetNumofSerchResult() || Order <= 0)))
		{
			cout << "******************************" << endl;
			cout << "* 请不要超出序号" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		//给定序号IDNum查找
		_DVDStock.SingleSearchDVDStock(ListHeadfromDVDWhere,Order);
		//查重，看是否已经借阅
		if(_DVDBorrow.GetLength() > 0)
			_DVDBorrow.SingleSearchDVDBorrow(_DVDStock.Position->Data.GetName());
		if(_DVDBorrow.Position)
		{
			cout << "******************************" << endl;
			cout << "* 您已经借阅了该影碟" << endl;
			cout << "* 请不要重复借阅" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		if(_DVDStock.Position->Data.GetNum() <= 0)
		{
			cout << "******************************" << endl;
			cout << "* 影碟库存不足" << endl;
			cout << "* 请重新选择" << endl;
			cout << "******************************" << endl;
			system("pause");
			return 0;
		}
		else
		{
			//库存减少
			_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() - 1);
			// 套了一层搜索的链表，需要找到原来链表的真实值，而且为了显示一致，需要都减掉
			if(_DVDStock.SearchResultList == ListHeadfromDVDWhere)
			{
				_DVDStock.SingleSearchDVDStock(_DVDStock.OriHead,_DVDStock.Position->Data.GetName());
				_DVDStock.Position->Data.SetNum(_DVDStock.Position->Data.GetNum() - 1);
			}
		}

		cout << "您要借阅的影碟为：" << endl;
		cout << _DVDStock.Position->Data.GetName() << endl;
		temp = _DVDStock.Position->Data;
		cout << "******************************" << endl;
		cout << "* 借阅的天数：";
		cout << Day << endl;
		cout << "******************************" << endl;

		temp.SetBorrowTime(NowTime);
		temp.SetReturnTime(NowTime + Day);
		temp.SetRentMoney(RentMoney(Day));
		_DVDBorrow.AddToHead(_DVDBorrow.OriHead, temp);
		cout << "* 借阅成功" << endl;
		cout << "* 归还需要交纳:" << temp.GetRentMoney() << "元" << endl;
	}

	if (Order)
		cout << "* 别忘了及时归还哦！" << endl;
	else
		cout << "* 您没有借阅任何影碟" << endl;
	cout << "******************************" << endl;
	system("pause");
	return 0;
}
//搜索DVD结果菜单
//mode = 1精确搜索，mode = 2模糊搜索,state=1表示管理员，state=0表示普通用户
int A_FSearchDVDResultMenu(int mode,string Key,int state)
{
	WindowsClear();
	int CMD;
	if (mode == 1)
		_DVDStock.ContinueSearchDVDStock(Key);
	else
		_DVDStock.ContinueSearchDVDStockFuzzy(Key);
	cout << "******************************" << endl;
	cout << "* 搜索结果：" << _DVDStock.GetNumofSerchResult() << endl;
	_DVDStock.DisplayList(_DVDStock.SearchResultList);
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	if (state == 0)
		cout << "* 2.借阅 + 影碟前面的序号 + 天数(多个借阅用空格隔开)" << endl;
	else//state==1
		cout << "* 2.删除 + 影碟前面的序号" << endl;
	cout << "******************************" << endl;
		cout << "*>>>";
	CMD = CinCMD(1, 2);
	switch (CMD)
	{
	case 1:
		//返回
		_DVDStock.DestoryList(_DVDStock.SearchResultList);
		return 0;
	case 2:
		if (state == 0)
			//借阅
			BorrowedDVD(_DVDStock.SearchResultList);
		else
		{
			//删除
			DeleteDVDMenu(_DVDStock.SearchResultList);//看这里传的是搜索结果链表，不是PageHead
		}
		
		return 1;
	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

//借阅影碟主界面
int BorrowDVDMenu()
{
	WindowsClear();
	int CMD;
	int CMDRange = 5;//当不存在任何页面跳转时，只有5个选项
	int TypeKey = 0;
	int SortMode = 0;
	string Key;
	cout << "******************************" << endl;
	cout << "*        借阅影碟" << endl;
	cout << "******************************" << endl;
	cout << "* 当前影碟数量：" << _DVDStock.GetLength() << " 当前页码：" << _DVDStock.GetCurrentPage() << endl;
	cout << "* 共：" << _DVDStock.GetMaxPage() << endl;
	cout << "******************************" << endl;
	_DVDStock.PageListDisplay();
	cout << "******************************" << endl;
	cout << "* 1.返回" << endl;
	cout << "* 2.借阅 + 影碟前面的序号 + 天数(多个借阅用空格隔开)" << endl;
	cout << "* 3.精确搜索（影碟名）+ 关键词" << endl;
	cout << "* 4.模糊搜索（影碟名）+ 关键词" << endl;
	cout << "* 5.排序 + 分类词序号 + 1/0 升/降序" << endl;

	//if这样判断是因为有可能压根结果不满一页，所以不需要换页
	if (_DVDStock.GetCurrentPage() == 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		//存在下一页面
		cout << "* 6.下一页" << endl;
		CMDRange = 6;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() == _DVDStock.GetMaxPage())
	{
		//存在上一页
		cout << "* 6.上一页" << endl;
		CMDRange = 6;
	}
	else if (_DVDStock.GetCurrentPage() != 1 && _DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
	{
		cout << "* 6.下一页" << endl;
		cout << "* 7.上一页" << endl;
		CMDRange = 7;
	}
	cout << "******************************" << endl;
	cout << "* 分类词序号：" << endl;
	cout << "* 1.影碟名 2.国家 3.类型 4.出版年份 5.库存数量" << endl;
	cout << "******************************" << endl;
	cout << "*>>>";
	CMD = CinCMD(1, CMDRange);

	switch (CMD)
	{
	case 1:
		//返回
		ClearInputBuffer();
		return 0;
	case 2:
		//addfunction()
		BorrowedDVD(_DVDStock.PageHead);
		return 1;
	case 3:
		//精确搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(1, Key, 0);
		return 1;
	case 4:
		//模糊搜索
		cin >> Key;
		ClearInputBuffer();
		A_FSearchDVDResultMenu(2, Key, 0);
		return 1;
	case 5:
		//排序
		cin >> TypeKey >> SortMode;
		ClearInputBuffer();
		_DVDStock.SortDVDList(_DVDStock.OriHead, (STOCKDVDKEY)TypeKey, SortMode);
		return 1;
	case 6:
		//下一页
		if (_DVDStock.GetCurrentPage() != _DVDStock.GetMaxPage())
		{
			_DVDStock.PageNext(_DVDStock.OriHead);
			return 1;
		}
		//这里没有break或者return。所以当不满足if的时候就下一页
	case 7:
		//上一页
		_DVDStock.PageLast(_DVDStock.OriHead);
		return 1;

	default:
		cout << "你是怎么进来的？" << endl;
		system("pause");
		return 0;
	}
}

#pragma endregion