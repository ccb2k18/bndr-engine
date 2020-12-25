/*MIT License

Copyright (c) 2020 Caleb Christopher Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <pch.h>
#pragma once

namespace bndr {

	// forward declare
	template <class T>
	class Queue;

	// bndr::Queue Iterator class that when used automatically empties the queue
	// when using a range-based for loop, you will get all of the contents from the queue until it is empty
	template <class T>
	class QueueIterator {

	public:
		// make code more readable with these using declarations
		using ElementType = typename Queue<T>::ElementType;
		using NodeType = typename Queue<T>::NodeType;
		using Iterator = typename QueueIterator<T>;

	private:

		ElementType* element;
		NodeType* node;
		Queue<T>* queue;

	public:

		// constructor takes in an element pointer, node pointer, and pointer to the queue instance
		QueueIterator(ElementType* e, NodeType* n, Queue<T>* q) : element(e), node(n), queue(q) {}
		// left increment (++i)
		Iterator& operator++() {

			// remove element from front of queue
			queue->dequeue();
			// update the element pointer
			// if the node isn't null then assign the element to the new front element in the Queue
			if (queue->node != nullptr) {

				element = &(queue->node->getElement());
			}
			// otherwise set it to null as well
			else {

				element = nullptr;
			}
			// update the node pointer
			node = queue->node;
			// return the modified instance
			return *this;
		}
		// right increment (i++)
		Iterator operator++(int) {

			Iterator iterator = Iterator(element, node, queue);
			++(*this);
			return iterator;
		}
		// dereference the iterator
		ElementType& operator*() {

			return *(element);
		}
		// check if two iterators are equal
		bool operator==(const Iterator& iterator) {

			return (iterator.node == node);
		}
		// check if two iterators are not equal
		bool operator!=(const Iterator& iterator) {

			return !((*this) == iterator);
		}
	};

	// This Queue class can be used for a variety of purposes
	// It is preferred as a better alternative than std::queue because its custom iterator automatically empties the queue
	template <class T>
	class Queue {

	protected:

		// node class to chain objects in a queue
		template <class T>
		class Node {

			T element;
			Node<T>* next;
		public:

			// constructor with element of type T
			Node(const T& newElement) : element(newElement), next(nullptr) {}
			// move constructor with element of type T (useful if T is quite large)
			Node(T&& newElement) : element((T&&)newElement), next(nullptr) {}
			// getter and setter for element
			inline T& getElement() { return element; }
			inline void setElement(T newElement) { element = newElement; }
			// getter and setter for next pointer
			inline Node<T>* getNext() { return next; }
			inline void setNext(Node<T>* newNext) { next = newNext; }

		};

		// first node of queue
		Node<T>* node;
		// the size of the queue
		int size;
		// pointer to the back of the queue for O(1) enqueue calls
		Node<T>* back;

	public:

		// using-declarations for making code more readable
		using Iterator = typename QueueIterator<T>;
		using ElementType = typename T;
		using NodeType = typename Node<T>;

		// default constructor is the only constructor
		Queue() : node(nullptr), size(0), back(nullptr) {}
		// construct the queue with an initializer list
		Queue(std::initializer_list<T>&& list);
		// copy constructor is not aloud
		Queue(const Queue<T>& queue) = delete;
		// move constructor
		Queue(Queue<T>&& queue);
		// reassignment is not aloud for queues
		Queue<T> operator=(const Queue<T>& queue) = delete;
		// peek at the front of the queue
		T peek();
		// add to back of queue
		void enqueue(const T& element);
		// move enqueue overload
		void enqueue(T&& element);
		// remove element from the front of queue
		T dequeue();
		// output to cout
		void print();
		// get the size of the queue
		inline int getSize() { return size; }
		// make Iterator a friend class
		friend class Iterator;
		// get iterator to front of queue
		inline Iterator begin() { return Iterator(&(node->getElement()), node, this); }
		// get iterator to back of queue
		inline Iterator end() { return Iterator(nullptr, nullptr, this); }
		// deallocate all nodes when our queue is destroyed
		~Queue();
	};

	template <class T>
	Queue<T>::Queue(std::initializer_list<T>&& list) {

		// add each value in the initializer list to the Queue
		for (const T& value : list) {

			enqueue(value);
		}
	}

	template <class T>
	Queue<T>::Queue(Queue<T>&& queue) {

		// move the data from the other queue
		node = queue.node;
		size = queue.size;
		back = queue.back;

		// now to put the other queue in a hollow state simply assign its node to null, its size to 0, and its back to null
		queue.node = nullptr;
		queue.size = 0;
		queue.back = nullptr;

	}

	template <class T>
	T Queue<T>::peek() {

		if (node != nullptr) {

			return node->getElement();
		}
		else {

			BNDR_EXCEPTION("Cannot peek at front of empty bndr::Queue instance");
		}
	}

	template <class T>
	void Queue<T>::enqueue(const T& element) {

		// increase size by 1
		size++;
		// if the queue doesn't have any nodes add the element to the first node
		if (node == nullptr) {

			node = new Node<T>(element);
			// now the back of the queue is the first node
			back = node;
			return;
		}
		// otherwise get the end node, add the new node to its next pointer, and update the back pointer
		back->setNext(new Node<T>(element));
		back = back->getNext();
	}

	template <class T>
	void Queue<T>::enqueue(T&& element) {

		// increase size by 1
		size++;
		// if the queue doesn't have any nodes add the element to the first node
		if (node == nullptr) {

			node = new Node<T>((T&&)element);
			// now the back of the queue is the first node
			back = node;
			return;
		}
		// otherwise get the end node, add the new node to its next pointer, and update the back pointer
		back->setNext(new Node<T>((T&&)element));
		back = back->getNext();
	}

	template <class T>
	T Queue<T>::dequeue() {

		if (node == nullptr) {

			// throw an exception since the queue is empty
			BNDR_EXCEPTION("Cannot call method dequeue on an empty bndr::Queue instance");
		}
		// decrement size
		size--;
		// if the queue has at least one element then copy the value of the first node to return
		T popValue = node->getElement();
		// if the queue only has one element then delete it
		if (node->getNext() == nullptr) {
			
			// deallocate the node and set the pointer to null
			delete node;
			node = nullptr;
			back = nullptr;
			return popValue;
		}
		// now if we have two nodes or more this is what we do

		Node<T>* nodeToDelete = node;
		node = node->getNext();
		nodeToDelete->setNext(nullptr);
		delete nodeToDelete;

		// return the popValue
		return popValue;

	}

	template <class T>
	void Queue<T>::print() {

		std::cout << "{ ";
		// if the queue is empty print nothing
		if (node == nullptr) {

			std::cout << '}';
			return;
		}
		// print the first node
		std::cout << node->getElement() << ' ';
		Node<T>* localPtr = node->getNext();
		// and print all nodes that follow
		while (localPtr != nullptr) {

			std::cout << localPtr->getElement() << ' ';
			localPtr = localPtr->getNext();
		}
		std::cout << '}';
	}

	template <class T>
	Queue<T>::~Queue() {

		// if our node exists then delete it and any other nodes
		if (node != nullptr) {

			// get the next pointer from the first node
			Node<T>* localPtr = node->getNext();
			// now delete the first node
			delete node;
			// traverse the queue deleting all the other nodes
			while (localPtr != nullptr) {

				Node<T>* nextPtr = localPtr->getNext();
				delete localPtr;
				localPtr = nextPtr;
			}
		}
	}

}

