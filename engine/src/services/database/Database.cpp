#include <services/database/Database.h>

namespace Engine {
	namespace Database {
		DatabaseConnection* connect() {
			DatabaseConnection* dbConnection = new DatabaseConnection();
			return dbConnection;
		}
	}
}