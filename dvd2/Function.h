#pragma once
#include "Inc.h"
#include "Users.hpp"
#include "Node.hpp"
bool Init();
int RetryWindows(int(*WindowsFunction)(void));
int OptionBegin();
int UserInfomation();
int BorrowDVDMenu();
int A_FSearchDVDResultMenu(int mode, string Key, int state);
int ChangeUserPassword();
int ChangeUsersName(Node<Users>* ListFromWhere);