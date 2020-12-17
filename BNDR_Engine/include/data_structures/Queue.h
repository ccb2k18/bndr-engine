#include <pch.h>
#pragma once

namespace bndr {

	// This Queue class has a custom iterator built for it so
	// a range-based for-loop is possible
	template <class T>
	class Queue {

		// node class to chain objects in a queue
		template <class T>
		class Node {

			T element;
			Node<T>* next;
		public:

			// constructor with element of type T
			Node(T newElement) : element(newElement), next(nullptr) {}
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
		// pointer to the back of the queue for quick pushes
		Node<T>* back;

	public:

		// default constructor is the only constructor
		Queue() : node(nullptr), size(0), back(nullptr) {}
		// copy constructor is not aloud
		Queue(const Queue<T>& queue) = delete;
		// move constructor
		Queue(Queue<T>&& queue);
		// reassignment is not aloud for queues
		Queue<T> operator=(const Queue<T>& queue) = delete;
		// peek at the front of the queue
		T peek();
		// add to front of queue
		void enqueue(T element);
		// pop an element from the queue
		T dequeue();
		// output to cout
		void print();
		inline int getSize() { return size; }
		// deallocate all nodes when our queue is destroyed
		~Queue();
	};

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
	void Queue<T>::enqueue(T element) {

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

		// store the pointer to the second node
		Node<T>* secondNode = node->getNext();
		// assign the first node element to be the second node's element
		node->SetElement(secondNode->getElement());
		// assign the first node next to point to the third node or null
		node->SetNext(secondNode->getNext());
		// finally delete the hollow-state second node so that the copied second node becomes the first node
		// and the third node becomes the second node
		delete secondNode;
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

