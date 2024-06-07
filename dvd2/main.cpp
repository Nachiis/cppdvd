#include "Function.h"

//摘要，fstream同时读写需要在读写之间清空标志位，且写文件之前要把光标放到开头；
//文件读入确实有读入int的函数，但是本项目有time这个类，其中有根据int构造time类，
//且time本身又有文件读入的>>运算符，导致二义性，所以全部转化成string读入，再转化成int；
//DVDstock类在赋值的时候调用默认赋值运算符，然而并不需要重载运算符，因为string类已经重载了；
//而且time类也不用重载，因为成员函数只有3个int

int main()
{
	if (Init())
	{
		while (true)
		{
			RetryWindows(OptionBegin);
		}
	}
	else
	{
		cout << "文件打开失败！" << endl;
		exit(0);
	}
	return 0;
}