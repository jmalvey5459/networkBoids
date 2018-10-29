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

#include "LinkedList.h"

LinkedList::LinkedList()
{
	head.count = 0;
	head.headNode = NULL;
	return;
}

LinkedList::~LinkedList()
{
	deleteAllNodes();
	return;
}

void LinkedList::addToHead(EVENT_TYPE type, Event* e)
{
	Node *pNew = new Node;
	pNew->type = type;
	pNew->event = e;
	pNew->next = head.headNode;
	head.headNode = pNew;
	head.count++;
}

bool LinkedList::removeFromHead()
{
	bool exit;
	Node *temp;

	if (head.headNode)
	{
		temp = head.headNode;
		head.headNode = head.headNode->next;
		delete temp;
		head.count--;
		exit = true; // returns true if successful
	}
	else
		exit = false; // returns false if unsuccessful

	return exit;
}

void LinkedList::addNode(EVENT_TYPE type, Event* event)
{
	Node *pNew = new Node,
		*pPre = NULL,
		*pCur = head.headNode;
	pNew->type = type;
	pNew->event = event;

	while (pCur && dataCmp(pNew->type, pCur->type) >= 0)
	{
		pPre = pCur;
		pCur = pCur->next;
	}

	if (pPre)
	{
		pNew->next = pPre->next;
		pPre->next = pNew;
		head.count++;
	}
	else
	{
		pNew->next = head.headNode;
		head.headNode = pNew;
		head.count++;
	}

}

bool LinkedList::deleteNode(EVENT_TYPE type)
{
	bool exit;
	Node *pPre = NULL,
		*pCur = head.headNode;

	while (pCur && dataCmp(pCur->type, type) < 0)
	{
		pPre = pCur;
		pCur = pCur->next;
	}

	if (pCur && dataCmp(pCur->type, type) == 0)
	{
		if (pPre)
		{
			pPre->next = pCur->next;
			delete pCur;
			head.count--;
			exit = true; // return true if successful
		}
		else
		{
			head.headNode = pCur->next;
			delete pCur;
			head.count--;
			exit = true; // return true if successful
		}
	}
	else
		exit = false; // return false if unsuccessful

	return exit;
}

void LinkedList::deleteAllNodes()
{
	Node *temp;

	while (head.headNode)
	{
		temp = head.headNode;
		head.headNode = head.headNode->next;
		delete temp;
		head.count--;
	}

	return;
}

bool LinkedList::isEmpty()
{
	return head.count == 0;
}

int LinkedList::getNoOfNodes()
{
	return head.count;
}

void LinkedList::displayAllNodes()
{
	Node *pCur = head.headNode;
	int nodeCount = 1;

	while (pCur)
	{
		std::cout << "Node " << nodeCount << ": ";
		displayNode(pCur);
		std::cout << std::endl;

		nodeCount++;
		pCur = pCur->next;
	}

	return;
}

int LinkedList::dataCmp(int value0, int value1)
{
	int exit = 0;

	if (value0 < value1)
		exit = -1;
	else if (value0 > value1)
		exit = 1;

	return exit;
}

void LinkedList::displayNode(Node *node)
{
	std::cout << node->type << "\t" << node->event->getName();
	return;
}

Node* LinkedList::getHead() {
	return head.headNode;
}

void LinkedList::deleteHead() {
	delete head.headNode;
}

void LinkedList::setHead(Node* n) {
	head.headNode = n;
}