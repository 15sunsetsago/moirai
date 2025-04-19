#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <sys/stat.h>
#include <unordered_set>
#include <iomanip>
#include <functional>
#include <cstddef>
#include <string_view>
#include <ranges>
#include <math.h>
#include <stdint.h>
#include <regex>

#define CROW_DISABLE_JSON
#include <crow.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

//search api format
std::string fetchAndFormatMediaResults(const std::string& query);

//map for user names and passwords
std::unordered_map<std::string, std::string> parseFormData(const std::string& body);

//map for splitting cookie string
std::unordered_map<std::string, std::string> parseCookies(const std::string& cookieHeader);

//checks for user and pass
bool infoCheckLogin(const std::string& username, const std::string& password);

//sign up functions, username checker and register user
bool infoCheckSignUp(const std::string& username);
bool registerUser(const std::string& username, const std::string& password);

/*
	TODO:
	-	Implement watchlist
	-	Implement favourites
	-	map vs vector idk.
*/

void sign_up(std::string request_body);

class user
{
	private:
		std::string user_name;
		std::string password;
	public:
		user(std::string t_user_name, std::string t_password)
		{
			user_name = t_user_name;
			std::hash<std::string> hash;
			password = hash(t_password);
		}

		const std::string get_user_name() const
		{
			return user_name;
		}

		const std::string get_password() const
		{
			return password;
		}

		bool operator==(const user& other) const
		{
			// Assuming usernames are unique
			return user_name == other.get_user_name();
		}

};

namespace std
{
	// template specialization to take user as parameter and uses user_name as 
	template<>
	struct hash<user>
	{
		size_t operator()(const user& key) const
		{
			return hash<std::string>()(key.get_password());
		}
	};
}

class user_map 
{
	private:
		std::unordered_map<user, user> user_map;

	public:
		// Return pointer to map since we have ONE map for entire codebase
		// Switch to smart pointer if necessary
		std::unordered_map<user, user>* get_map()
		{
			std::unordered_map<user, user>* ptr = &user_map;
			return ptr;
		}

		void add_user(user new_user)
		{
			user_map.insert({ new_user, new_user });
		}
};