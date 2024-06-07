#pragma once
#include "Inc.h"
//µ•œÚ¡¥±Ì
template <typename T>
class Node {
public:
	Node(T data, Node* next = nullptr):
		Data(data),
		Next(next) {}
public:
	T Data;
	Node* Next;

};