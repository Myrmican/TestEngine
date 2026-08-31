namespace Engine {

	template <typename... Args>
	class Event {
	public:
		struct Connection {
			Event* event = nullptr;

			void disconnect() {
				if (!event) return;

				event->disconnect();
				event = nullptr;
			}
		};

		Connection connect() {
			return Connection{this};
		}

		void call(Args... args) const {

		}

	private:
		void disconnect() {

		}
	};
}