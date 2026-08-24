#include <services/database/DatabaseConnection.h>

namespace Engine {

	namespace Database {
		DatabaseConnection* connect();
		void onQuery();
	}
}