#pragma once
template<class Type>
class DblLinkedList 
{
private:
	typedef struct Node
	{
		Type value;
		struct Node* next;
		struct Node* prev;
	};
	Node* head;
	Node* curr;
public:
	int length;

	DblLinkedList()
	{
		head = nullptr;
		curr = nullptr;
		length = 0;
	}

	DblLinkedList(Type value)
	{
		head = nullptr;
		curr = nullptr;
		length = 0;
		push(value);
	}

	DblLinkedList(const DblLinkedList& list)
	{
		this->head = nullptr;
		this->curr = nullptr;
		this->length = 0;
		Node* p = list.curr;
		for (int i = 0; i < list.length; i++)
		{
			push(p->value);
			p = p->next;
		}
	}

	~DblLinkedList()
	{
		clear();
	}

	void init()
	{
		curr = head;
	}

	void next()
	{
		if (!empty())
			curr = curr->next;
	}

	void prev()
	{
		if (!empty())
			curr = curr->prev;
	}

	Type getValue()
	{
		if (!empty())
			return curr->value;
		else
			return 0;
	}

	bool empty()
	{
		if (curr == nullptr)
			return true;
		else
			return false;
	}

	void push(Type value)
	{
		Node* inserted;
		inserted = new Node;
		inserted->value = value;
		if (empty()) // если список пуст
		{
			head = inserted;
			curr = inserted;
			curr->next = inserted;
			curr->prev = inserted;
		}
		else // если список не пуст
		{
			inserted->next = curr->next; 
			inserted->next->prev = inserted;
			curr->next = inserted;
			inserted->prev = curr;
		}
		length++;
		curr = inserted;
	}

	void pop()
	{
		if (empty()) return;
		Node* temp = curr;

		if (length == 1)
		{
			head = nullptr;
			curr = nullptr;
		}
		else
		{
			curr->next->prev = curr->prev;
			curr->prev->next = curr->next;
			curr = curr->next;
		}
		if (temp == head)
			head = head->next;
		length--;
		delete temp;
	}

	void clear()
	{
		for (int i = 0; i < length;)
			pop();
	}
};
