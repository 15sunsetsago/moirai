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
#include <memory>

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

		std::string get_user_name()
		{
			return user_name;
		}

		std::string get_password()
		{
			return password;
		}

		bool operator==(user& other)
		{
			// Assuming usernames are unique
			return user_name == other.get_user_name();
		}

};

namespace std
{
	template<>
	struct hash<user>
	{
		size_t operator()(user& key)
		{
			return hash<std::string>()(key.get_password());
		}
	};
}

class citadel
{
	private:
		std::unordered_map<std::string, user> user_map;
	public:

		void add_user(user user) { user_map.insert({ user.get_user_name(), user}); }

		bool is_user_in_map(std::string user_name) { return user_map.contains(user_name); } 

		// Use AFTER user_name validation (is_user_in_map())
		user retrieve_user(std::string username)
		{
			return user_map.at(username);
		}
};


user create_user_from_request(std::shared_ptr<citadel> hash_table, std::string request_body);
user create_user_for_login(std::string request_body);

void login_validation(std::shared_ptr<citadel> hash_table, user person);

/*
	Exceptions
*/

class username_taken_exception : public std::exception
{
	public:
		virtual const char* what() const noexcept
		{
			return "Username taken";
		}
};

class login_incorrect_exception : public std::exception
{
	public:
		virtual const char* what() const noexcept
		{
			return "Information incorrect";
		}
};