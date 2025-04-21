#include "vedas.h"
using json = nlohmann::json;

//map to store and split user name pass from html req, could also use streamview for this btw
std::unordered_map<std::string, std::string> parseFormData(const std::string& body) {

    std::unordered_map<std::string, std::string> userPass;
    std::stringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        std::stringstream pairStream(pair);
        std::string user, pass;
        std::getline(pairStream, user, '=');
        std::getline(pairStream, pass);

        userPass[user] = pass;
    }

    return userPass;
}

//hashtable that's gonna store username from cookies and session id (also cookie spliter)
std::unordered_map<std::string, std::string> parseCookies(const std::string& cookieHeader) {

    std::unordered_map<std::string, std::string> cookies;
    std::stringstream ss(cookieHeader);
    std::string pair;

    while (std::getline(ss, pair, ';')) {
        std::stringstream pairStream(pair);
        std::string cookie, sessionID;
        std::getline(pairStream, cookie, '=');
        std::getline(pairStream, sessionID);

        // get rid of space
        cookie.erase(0, cookie.find_first_not_of(" "));

        cookies[cookie] = sessionID;
    }

    return cookies;
};

//checks if login info is correct
bool infoCheckLogin(const std::string& username, const std::string& password) {
    std::ifstream users_file("users.json");
    if (!users_file) return false;

    json usersData = json::parse(users_file);

    for (const json& item : usersData["users"]) {

        std::hash<std::string> hasher;
        size_t hashedPassword = hasher(password);
        std::string inputHash = std::to_string(hashedPassword);

        if (item.contains("username") && item.contains("password")) {
            std::string storedUser = item["username"];
            std::string storedPass = item["password"];

            if (storedUser == username && storedPass == inputHash) {
                return true;
            }
        }
    }

    return false;
}
