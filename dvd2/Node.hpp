#pragma once
#include "Inc.h"
//��������
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