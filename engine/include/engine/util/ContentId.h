#include <string>

namespace Engine {
	class ContentId {
	public:
		ContentId(const std::string& protocol, int64_t id);

		const std::string& getProtocol() const;
		int64_t getId() const;

	private:
		std::string protocol;
		int64_t id;
	};
}