#pragma once
#include "Inc.h"

class Time
{
public:
	Time(int y=1,int m=1,int d=1):year(y),month(m),day(d){}
	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }
	void SetYear(int y) { year = y; }
	void SetMonth(int m) { month = m; }
	void SetDay(int d) { day = d; }
	void SetCurrentTime() {
		//获取当前时间
		SYSTEMTIME _time;
		GetLocalTime(&_time);
		year = _time.wYear;
		month = _time.wMonth;
		day = _time.wDay;
	}
	friend istream& operator>>(istream& in, Time& time);
	friend ostream& operator<<(ostream& out, const Time& time);
	friend fstream& operator<<(fstream& fileout, const Time& time);
	/*friend fstream& operator>>(fstream& filein, Time& time);*/
	
	friend Time operator+(Time t1, int t2);
	friend bool operator<(Time t1, Time t2);
	friend int operator-(Time t1, Time t2);
	//时间租金
	friend int RentMoney(int Day);
	//时间租金超出部分
	friend double RentOverMoney(int Day);
private:
	
	int year;
	int month;
	int day;
};
int RentMoney(int Day);
double RentOverMoney(int Day);

istream& operator>>(istream& in, Time& time);
ostream& operator<<(ostream& out, const Time& time);
fstream& operator<<(fstream& fileout, const Time& time);

//time类+int天数
Time operator+(Time t1, int t2);
//时间比较
bool operator<(Time t1, Time t2);
//时间差值，返回天数
int operator-(Time t1, Time t2);
