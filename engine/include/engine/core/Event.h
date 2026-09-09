#pragma once

#include <iostream>
#include <functional>

namespace Engine {

	template <typename... Args>
	class Event {
	public:
		using Callback = std::function<void(Args...)>;

		

		struct Connection {
			Event* event = nullptr;
			size_t id = 0;

			void disconnect() {
				if (!event) return;

				event->disconnect(id);
				event = nullptr;
			}
		};

		Connection connect(Callback callback) {
			size_t id = m_nextId++;
			m_callbacks[id] = callback;
			return Connection{this};
		}

		void call(Args... args) const {
			auto callbacksCopy = m_callbacks;
			for (const auto& [id, callback] : callbacksCopy) {
				if (callback) callback(args...);
			}
		}

	private:
		void disconnect() {

		}

		size_t m_nextId = 0;
		std::unordered_map<size_t, Callback> m_callbacks;
	};
}