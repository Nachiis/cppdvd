#include "DVD.hpp"
#include "Node.hpp"
#include "Function.h"
istream& operator>>(istream& in, BorrowDVD& d)
{
	string rentmoneystr = "0";
	in >> d.Name >> d.Nation >> d.Type >> d.PublicTime >> d.BorrowTime >> d.ReturnTime >> rentmoneystr;
	d.RentMoney = stoi(rentmoneystr);
	return in;
}
ostream& operator<<(ostream& out, BorrowDVD d)
{
	using std::operator<<;
	out << "* " << setw(4) << left << d.IDNum;
	out << setw(16) << left << d.Name;
	out << setw(16) << left << d.Nation;
	out << setw(16) << left << d.Type;
	out << setw(16) << left << d.PublicTime;
	out << setw(16) << left << d.BorrowTime;
	out << setw(16) << left << d.ReturnTime;
	out << setw(16) << left << d.RentMoney;
	out << endl;
	return out;
}
fstream& operator<<(fstream& fileout, const BorrowDVD& d)
{
	using std::operator<<;
	fileout << d.Name << " ";
	fileout << d.Nation << " ";
	fileout << d.Type << " ";
	fileout << d.PublicTime << " ";
	fileout << d.BorrowTime << " ";
	fileout << d.ReturnTime << " ";
	fileout << to_string(d.RentMoney) << " ";
	fileout << endl;
	return fileout;
}
istream& operator>>(istream& in, DVDStock& d)
{
	in >> d.Name >> d.Nation >> d.Type >> d.PublicTime >> d.Num;
	return in;
}
ostream& operator<<(ostream& out, DVDStock d)
{
	using std::operator<<;
	out << "* " << setw(4) << left << d.IDNum;
	out << setw(16) << left << d.Name;
	out << setw(16) << left << d.Nation;
	out << setw(16) << left << d.Type;
	out << setw(16) << left << d.PublicTime;
	out << setw(16) << left << d.Num;
	out << endl;
	return out;
}
fstream& operator<<(fstream& fileout, const DVDStock& d)
{
	using std::operator<<;
	fileout << d.Name << " ";
	fileout << d.Nation << " ";
	fileout << d.Type << " ";
	fileout << d.PublicTime << " ";
	fileout << d.Num << " ";
	fileout << endl;
	return fileout;
}