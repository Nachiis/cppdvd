#pragma once
#include "Inc.h"
#include "Time.hpp"
class DVD
{
public:
	DVD(string N = "hee", string Na = "hee", string T = "hee", Time P = Time(0, 0, 0)) 
		:Name(N), Nation(Na), Type(T), PublicTime(P),IDNum(0){}
	string GetName() const { return Name; }
	string GetNation() const { return Nation; }
	string GetType() const { return Type; }
	Time GetPublicTime() const { return PublicTime; }
	int GetIDNum() const { return IDNum; }
	void SetName(string N) { Name = N; }
	void SetNation(string Na) { Nation = Na; }
	void SetType(string T) { Type = T; }
	void SetPublicTime(Time P) { PublicTime = P; }//默认赋值运算足矣
	void SetIDNum(int ID) { IDNum = ID; }
protected:
	string Name;			//碟片名
	string Nation;			//出版国家
	string Type;			//类型
	Time PublicTime;		//出版时间
	int IDNum;				//序号
};

//用户借阅的DVD
class BorrowDVD :public DVD
{
private:
	Time BorrowTime;		//借阅时间
	Time ReturnTime;		//归还时间
	double RentMoney;			//租金
public:
	BorrowDVD(string N = "hee", string Na = "hee", string T = "hee", Time P = Time(0, 0, 0)) :
		DVD(N, Na, T, P), 
		BorrowTime(Time(0, 0, 0)),
		ReturnTime(Time(0, 0, 0)), 
		RentMoney(0){}
	Time GetBorrowTime() const { return BorrowTime; }
	Time GetReturnTime() const { return ReturnTime; }
	double GetRentMoney() const { return RentMoney; }
	void SetBorrowTime(Time B) { BorrowTime = B; }
	void SetReturnTime(Time R) { ReturnTime = R; }
	void SetRentMoney(int M) { RentMoney = M; }

	friend istream& operator>>(istream& in, BorrowDVD& d);
	friend ostream& operator<<(ostream& out, BorrowDVD d);
	friend fstream& operator<<(fstream& fileout, const BorrowDVD& d);

	void operator=(const DVD& d)
	{
		if (this == &d)
		{
			cout << "系统错误：不允许存在自我赋值" << endl;
			return;
		}
		Name = d.GetName();
		Nation = d.GetNation();
		Type = d.GetType();
		PublicTime = d.GetPublicTime();
	}
};

//DVD库存
class DVDStock :public DVD
{
private:
	int Num;				//库存数量，作为租借标记
public:
	DVDStock(string N = "hee", string Na = "hee", string T = "hee", Time P = Time(0, 0, 0), int Num = 0) :
		DVD(N, Na, T, P), 
		Num(Num){}
	int GetNum() const { return Num; }
	void SetNum(int N) { Num = N; }

	friend istream& operator>>(istream& in, DVDStock& d);
	friend ostream& operator<<(ostream& out, DVDStock d);
	friend fstream& operator<<(fstream& fileout, const DVDStock& d);
};

istream& operator>>(istream& in, DVDStock& d);
ostream& operator<<(ostream& out, DVDStock d);
fstream& operator<<(fstream& fileout, const DVDStock& d);

istream& operator>>(istream& in, BorrowDVD& d);
ostream& operator<<(ostream& out, BorrowDVD d);
fstream& operator<<(fstream& fileout, const BorrowDVD& d);

enum STOCKDVDKEY
{
	Name = 1,
	Nation,
	Type,
	PublicTimeYear,
	StockNum,
	IDNum,
};