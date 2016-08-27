#include <vector>

#include "element.hpp"

using namespace std;

template<typename T>
class LinkedList {

private:
	Element<T> *_head;
	int _size;

public:
	LinkedList() : _head(0), _size(0) {}
	//~LinkedList();

	void pushFront(const T& element);
	T popFront();

	void pushOn(const T& element, int pos);
	int getIndex(const T& element) const;
	bool contains(const T& element);
	T popFrom(int pos);

	void push(const T& newElement) {
		Element<T> *element = new Element<T>(newElement);

		if (_size == 0) {
			_head = element;
		} else {
			Element<T> *last = _head;

			while (last->next() != 0) {
				last = last->next();
			}

			last->next(element);
		}

		++_size;
	}

	T pop() {
		Element<T> *newLast = _head;

		if (_size < 2) {
			T returned = _head->info();
			delete _head;
			_head = 0;
			--_size;
			return returned;
		} else {
			while (newLast->next()->next() != 0) {
				newLast = newLast->next();
			}

			T returned = newLast->next()->info();
			delete newLast->next();
			newLast->next(0);
			--_size;
			return returned;
		}
	}

	T popElement(const T& element);
	
	bool isEmpty() {
		return _size == 0;
	}

	bool isEquals(T element1, T element2);
	bool isGreaterThan(T element1, T element2);
	bool isLessThan(T element1, T element2);
	void destroy();

	void buildFromVector(vector<T> elements) {
		for(T element : elements) {
			push(element);
		}
	}
};
