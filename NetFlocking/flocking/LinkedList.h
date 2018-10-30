#pragma once

// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 1
// 8-27-2018
//
// Vedant Chaudhari, 1532077
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

// Inspired by https://github.com/skorks/c-linked-list/blob/master/linkedlist.h

#include <iostream>
#include <string>

#include "Event.h"

struct Node {
	EVENT_TYPE type;
	Event* event;

	Node* next;
};

struct Head{
	int count;

	Node* headNode;
};

class LinkedList {
private:
	int dataCmp(int, int);
	void displayNode(Node*);

	Head head;

public:
	LinkedList();
	~LinkedList();

	void addToHead(EVENT_TYPE, Event*);
	bool removeFromHead();

	void addNode(EVENT_TYPE, Event*);
	bool deleteNode(EVENT_TYPE);
	void deleteAllNodes();

	bool isEmpty();
	int getNoOfNodes();

	void displayAllNodes();

	Node* getHead();
	void deleteHead();
	void setHead(Node*);
};