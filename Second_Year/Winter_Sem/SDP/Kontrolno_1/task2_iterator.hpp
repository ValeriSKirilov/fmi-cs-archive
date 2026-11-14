#ifndef _LINKED_LIST_HPP
#define _LINKED_LIST_HPP

/*	����������� �� ������� �� ������� ������ � ���� ������
	 ___________
	|��/��| ��� |
	|_____|_____|
	 data   next
*/
template <typename T>
struct LinkedListElement
{
	T data;
	LinkedListElement<T> *next;
};

// ���������� �� ���� LinkedList
template <typename T>
class LinkedList;

// ���������� �� ���� ��������
template <typename T>
class LinkedListIterator
{
	// ��������� ������ �� ������ �� ������� �� ������� ������ � ��������
	using LLE = LinkedListElement<T>;
	using I = LinkedListIterator<T>;

	// ��������� ������ �� ���������� ����������� �� �������
	friend LinkedList<T>;

	// ��������� ���������� �� ������� ���� ptr ��� �������
	LLE *ptr;

public:
	// ����������� �� ����������� � ����������� � ���� ���������
	LinkedListIterator(LLE *pos = nullptr) : ptr(pos) {}

	// ����� �� ������� �� �������� ��� ������� �������
	I next() const
	{
		// �������� �� ��������� �� ������� ��������
		if (!valid())
		{
			// ������� � ����� ������� ��������
			return *this;
		}
		// ������� ��� ����� ��������, ����� �� ������� ��� ��������� ������� �� �������
		return I(ptr->next);
	}

	// �������� �� ��������� (������� � ������ � ����������� �� ���� �� ��������� ������)
	bool valid() const
	{
		return ptr != nullptr;
	}

	// ������ �� ��������� �� ���������� �� �������� ������� � ���������
	T const &getConst() const
	{
		return ptr->data;
	}

	T &get() const
	{
		return ptr->data;
	}

	// ����������� �����
	// it <=> it.valid();
	operator bool() const
	{
		return valid();
	}

	// ++it
	I &operator++()
	{
		return (*this = next());
	}

	// it++
	I operator++(int)
	{
		I saved = *this;
		++(*this);
		return saved;
	}

	// *it = 3;
	T &operator*()
	{
		return get();
	}

	// T data = *it;
	T const &operator*() const
	{
		return getConst();
	}
	// ��������� �� ���������
	// it1 == it2
	bool operator==(I const &it) const
	{
		return ptr == it.ptr;
	}

	// it1 != it2
	bool operator!=(I const &it) const
	{
		return !(*this == it);
	}
};

// ���������� �� ���� ������� ������
template <typename T>
class LinkedList
{
public:
	// �� ���������� I ���� ��� �� LinkedListIterator<T>
	using I = LinkedListIterator<T>;
	using Type = T;

private:
	using LLE = LinkedListElement<T>;

	// ����������� �� ������� ������ � ��� ���������
	LLE *front, *back;

public:
	// ����������� �� �����������, ����� ������� ������ ������
	LinkedList() : front(nullptr), back(nullptr) {}

	void InserAfterNInterval(I it, int n, T x);
};

#endif
