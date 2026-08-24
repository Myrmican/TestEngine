#include <vector>
#include <string>
#include <any>

using namespace std;

namespace Engine {

	class DatabaseConnection {
	public:
		DatabaseConnection();

		bool query(string_view query, vector<any> values); //Return type will be replaced with actual result

		void set(string key, any value);
		any get(string key);
		void remove(string key);
		int listKeys();
	};
}