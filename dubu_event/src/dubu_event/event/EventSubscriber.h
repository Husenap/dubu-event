#pragma once

#include <functional>
#include <vector>

#include "Token.h"

namespace dubu::event {

class EventSubscriber {
public:
	EventSubscriber() = default;
	virtual ~EventSubscriber() {}
	EventSubscriber(const EventSubscriber&) = delete;

protected:
	template <typename EventType, typename EmitterType>
	void Subscribe(std::function<void(const EventType&)> cb,
	               EmitterType&                          emitter) {
		tokens.push_back(emitter.Subscribe<EventType>(cb));
	}

private:
	std::vector<dubu::event::Token> tokens;
};

}  // namespace dubu::event
