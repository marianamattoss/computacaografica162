#ifndef LINKEDLIST_ELEMENT_HPP_
#define LINKEDLIST_ELEMENT_HPP_

template<typename T>
class Element {
private:
	T _info;
	Element<T> *_next;

public:
	Element(const T &info) : _info(info), _next(0) {}
	Element(const T &info, Element<T> *next) : _info(info), _next(next) {}
	~Element() {}

	Element<T>* next() const {
		return _next;
	}

	T info() const {
		return _info;
	}

	void next(Element<T>* next) {
		_next = next;
	}
};


#endif /* LINKEDLIST_ELEMENT_HPP_ */
