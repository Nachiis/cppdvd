#include "Time.hpp"
ostream& operator<<(ostream& out, const Time& time)
{
	string timestr = to_string(time.year) + "-" + to_string(time.month) + "-" + to_string(time.day);
	out << timestr;
	return out;
}
istream& operator>>(istream& in, Time& time)
{
	in >> time.year;
	in >> time.month; 
	in>> time.day;
	//检查输入是否合法，大小月
	if (time.month == 2)
	{
		if (time.year % 4 == 0 && time.year % 100 != 0 || time.year % 400 == 0)
		{
			if (time.day > 29)
			{
#if DEBUG
				cout << "日期输入错误，已自动修正" << endl;
				system("pause");
#endif
				time.day = 29;
			}
			if(time.day < 1)
			{
#if DEBUG
				cout << "日期输入错误，已自动修正" << endl;
				system("pause");
#endif
				time.day = 1;
			}
		}
		else
		{
			if (time.day > 28)
			{
#if DEBUG
				cout << "日期输入错误，已自动修正" << endl;
				system("pause");
#endif
				time.day = 28;
			}
			if(time.day < 1)
			{
#if DEBUG
				cout << "日期输入错误，已自动修正" << endl;
				system("pause");
#endif
				time.day = 1;
			}
		}
	}
	else if (time.month == 4 || time.month == 6 || time.month == 9 || time.month == 11)
	{
		if (time.day > 30)
		{
#if DEBUG
			cout << "日期输入错误，已自动修正" << endl;
			system("pause");
#endif
			time.day = 30;
		}
		if(time.day < 1)
		{
#if DEBUG
			cout << "日期输入错误，已自动修正" << endl;
			system("pause");
#endif
			time.day = 1;
		}
	}
	else
	{
		if (time.day > 31)
		{
#if DEBUG
			cout << "日期输入错误，已自动修正" << endl;
			system("pause");
#endif
			time.day = 31;
		}
		if(time.day < 1)
		{
#if DEBUG
			cout << "日期输入错误，已自动修正" << endl;
			system("pause");
#endif
			time.day = 1;
		}
	}
	return in;
}
fstream& operator<<(fstream& fileout, const Time& time)
{
	using std::operator<<;
	fileout << time.year << " " << time.month << " " << time.day << " ";
	return fileout;
}
Time operator+(Time t1, int t2)
{
	Time t = t1;
	t.day += t2;
	//大小月判断
	if (t.month == 2)
	{
		if (t.year % 4 == 0 && t.year % 100 != 0 || t.year % 400 == 0)
		{
			if (t.day > 29)
			{
				t.day -= 29;
				t.month++;
			}
		}
		else
		{
			if (t.day > 28)
			{
				t.day -= 28;
				t.month++;
			}
		}
	}
	else if (t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11)
	{
		if (t.day > 30)
		{
			t.day -= 30;
			t.month++;
		}
	}
	else
	{
		if (t.day > 31)
		{
			t.day -= 31;
			t.month++;
		}
	}
	//年份进位
	if (t.month > 12)
	{
		t.month -= 12;
		t.year++;
	}
	return t;
}

int RentMoney(int Day)
{
	if (Day % 3)//余数为1或2，天数不足3天
	{
		return Day / 3 + 1;
	}
	else
	{
		return Day / 3;
	}
}
double RentOverMoney(int Day)
{
	return Day * 0.8;
}
bool operator<(Time t1, Time t2)
{
	if (t1.year < t2.year)
		return true;
	else if (t1.year == t2.year)
	{
		if (t1.month < t2.month)
			return true;
		else if (t1.month == t2.month)
		{
			if (t1.day < t2.day)
				return true;
		}
	}
	return false;
}
//t1 > t2
int operator-(Time t1, Time t2)
{
	int day = 0;
	if (t1 < t2)
	{
		Time temp = t1;
		t1 = t2;
		t2 = temp;
	}
	while (t2 < t1)
	{
		t2 = t2 + 1;
		day++;
	}
	return day;
}