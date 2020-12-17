#include <pch.h>
#pragma once

namespace bndr {

	// base class for all event objects in the engine
	// every event instance will have some kind of state associated with it
	class Event {

	protected:

		int state;
	public:

		Event(int newState) : state(newState) {}
		Event() : state(0) {}
		inline int getState() { return state; }
	};

	// base class for event queues in the engine
	// the template class 'E' reminds the programmer that it only should accept an instance of Event
	template <class E>
	class EventQueue {

		Queue<E> queue;
	public:

		// initialize the event queue with an event
		EventQueue(E newEvent);
		EventQueue() : {}
	};

	template <class E>
	EventQueue<E>::EventQueue(E newEvent) {

		queue.enqueue(newEvent);
	}
}


