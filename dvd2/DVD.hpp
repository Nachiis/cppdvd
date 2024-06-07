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
	void SetPublicTime(Time P) { PublicTime = P; }//Ĭ�ϸ�ֵ��������
	void SetIDNum(int ID) { IDNum = ID; }
protected:
	string Name;			//��Ƭ��
	string Nation;			//�������
	string Type;			//����
	Time PublicTime;		//����ʱ��
	int IDNum;				//���
};

//�û����ĵ�DVD
class BorrowDVD :public DVD
{
private:
	Time BorrowTime;		//����ʱ��
	Time ReturnTime;		//�黹ʱ��
	double RentMoney;			//���
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
			cout << "ϵͳ���󣺲�����������Ҹ�ֵ" << endl;
			return;
		}
		Name = d.GetName();
		Nation = d.GetNation();
		Type = d.GetType();
		PublicTime = d.GetPublicTime();
	}
};

//DVD���
class DVDStock :public DVD
{
private:
	int Num;				//�����������Ϊ�����
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