#include "vedas.h"
using json = nlohmann::json;

//checks if users.json exists
static bool fileExists(const std::string& users) {
	struct stat buf;

	if (stat(users.c_str(), &buf) != -1) { return true; }

	return false;
}

//checks if username is already taken
bool infoCheckSignUp(const std::string& userData) {
	if (!fileExists("users.json")) {
		return false;
	}
	std::ifstream users_file("users.json");
	json usersData = json::parse(users_file);
	users_file.close();

	for (const json& item : usersData["users"]) {
		if (item.contains("username") && item["username"] == userData) {
			return true;
		}
	}
	return false;
}

//if username isn't taken then allows user to register an account
bool registerUser(const std::string& username, const std::string& password) {

	//to hash the password
	std::hash<std::string> hasher;
	size_t hashedPassword = hasher(password);

	json newUser;

	if (!fileExists("users.json")) {
		return false;
	}
	std::ifstream users_file("users.json");
	json usersData = json::parse(users_file);
	users_file.close();

	newUser["username"] = username;
	newUser["password"] = std::to_string(hashedPassword); //gets hashed password as a string

	usersData["users"].push_back(newUser);
	std::ofstream users_file_out("users.json");
	users_file_out << usersData.dump(4);
	users_file_out.close();

	return true;
}