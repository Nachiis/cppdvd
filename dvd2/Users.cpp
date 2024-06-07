#include "Users.hpp"
//UsersÀàÅĞ¶ÏÓÃ»§==
bool operator==(const Users l, const Users r)
{
	if (l.Name == r.Name && l.Password == r.Password)return true;
	else return false;
}
bool operator!=(const Users l, const Users r)
{
	return !(l == r);
}
istream& operator>>(istream& in, Users& u)
{
	in >> u.Name >> u.Password >> u.State;
	return in;
}
ostream& operator<<(ostream& out, const Users& u)
{
	using std::operator<<;
	out << "* " << setw(4) << left << u.IDNum;
	out << setw(16) << left << u.Name;
	out << endl;
	return out;
}
fstream& operator<<(fstream& out, Users u)
{
	using std::operator<<;
	out << u.Name << " " << u.Password << " " << u.State << endl;
	return out;
}