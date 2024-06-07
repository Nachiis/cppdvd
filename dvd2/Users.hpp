#pragma once
#include "Inc.h"
#include "Node.hpp"
#include "DVD.hpp"

using namespace std;
class Users
{
public:
	Users(string N = " ", string P = " ", int s = 0) :
		Name(N),
		Password(P),
		State(s),
		IDNum(0) {}
	string GetName() const { return Name; }
	string GetPassword() const { return Password; }
	int GetState() const { return State; }
	int GetIDNum() const { return IDNum; }
	void SetName(string N) { Name = N; }
	void SetPassword(string P) { Password = P; }
	void SetState(int s) { State = s; }
	void SetIDNum(int s) { IDNum = s; }

	friend bool operator==(const Users l, const Users r);
	friend bool operator!=(const Users l, const Users r);
	friend istream& operator>>(istream& in, Users& u);
	friend ostream& operator<<(ostream& out, const Users& u);
	friend fstream& operator<<(fstream& out, Users u);

protected:
	string Name;					//用户名
	string Password;				//密码
	int State;						//身份
	int IDNum;						//编号

};
bool operator==(const Users l, const Users r);
bool operator!=(const Users l, const Users r);
istream& operator>>(istream& in, Users& u);
ostream& operator<<(ostream& out, const Users& u);
fstream& operator<<(fstream& out, Users u);