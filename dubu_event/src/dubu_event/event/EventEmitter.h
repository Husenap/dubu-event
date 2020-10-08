#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <dubu_util/dubu_util.h>

namespace dubu::event {

namespace internal {
struct _Token {};
}  // namespace internal

using Token = std::shared_ptr<internal::_Token>;

class EventEmitter {
private:
	struct Listener {
		void*                           callback;
		std::weak_ptr<internal::_Token> token;
	};

public:
	template <typename EventType>
	Token Subscribe(std::function<void(const EventType&)> callback) {
		const dubu::util::IdType eventId = dubu::util::TypeId::Get<EventType>();

		const auto cb = new std::function<void(const EventType&)>{
		    [=](const EventType& event) { callback(event); }};

		const Token token = std::make_shared<internal::_Token>();

		const Listener listener{.callback = reinterpret_cast<void*>(cb),
		                        .token    = token};

		Subscribe(eventId, listener);

		return token;
	}

protected:
	template <typename EventType>
	void Emit(const EventType& event) {
		const dubu::util::IdType eventId = dubu::util::TypeId::Get<EventType>();

		auto it = mListeners.find(eventId);
		if (it == mListeners.end()) {
			return;
		}

		for (std::size_t i = 0; i < it->second.size(); ++i) {
			const auto& entry = it->second[i];

			auto cb = reinterpret_cast<std::function<void(const EventType&)>*>(
			    entry.callback);

			if (entry.token.expired()) {
				delete cb;
				it->second.erase(it->second.begin() + i);
				--i;
				continue;
			}

			(*cb)(event);
		}
	}

private:
	void Subscribe(dubu::util::IdType eventId, Listener listener);

	std::map<dubu::util::IdType, std::vector<Listener>> mListeners;
};

}  // namespace dubu::event