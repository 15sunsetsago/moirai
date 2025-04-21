#define CROW_MAIN
#include "vedas.h"
using json = nlohmann::json;

/*
    TODO:
    - Username restrictions
    - Password restrictions
    i.e length, character types....
*/

int main() {
    // Initiate map
    std::shared_ptr<citadel> hash_table = std::make_shared<citadel>();

    user debug_user("john", "pork");
    hash_table->add_user(debug_user);

    crow::mustache::set_base("templates");
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        ([](const crow::request& req) {
        std::string rawCookie = req.get_header_value("Cookie");
        std::unordered_map<std::string, std::string> cookies = parseCookies(rawCookie);

        crow::mustache::context ctx;

        if (cookies.find("username") != cookies.end()) {
            ctx["username"] = cookies["username"];
        }

        return crow::mustache::load("home.html").render(ctx);
            });


    CROW_ROUTE(app, "/static/<string>")
        ([](const std::string& filename) {
        std::ifstream in("static/" + filename, std::ifstream::in);
        if (!in) return crow::response(404);
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();

        crow::response res(contents.str());
        res.add_header("Content-Type", "text/css");
        return res;
            });


    CROW_ROUTE(app, "/search")
        ([](const crow::request& req) {
        const char* q = req.url_params.get("q");

        if (!q) {
            return crow::response(400, "Missing query");
        }

        std::string query = q;
        std::string html = fetchAndFormatMediaResults(query);

        return crow::response(html);
            });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)
        ([hash_table](const crow::request& req) 
        {
            try
            {
                user possible_user = create_user_for_login(req.body);
                login_validation(hash_table, possible_user);
                return crow::response(200, "Login Successful!");
            }
            catch (login_incorrect_exception& e)
            {
                // HANDLE EXCEPTION BY TELLING END-USER
                CROW_LOG_INFO << "Login Error: " << e.what();
                return crow::response(409, "Error logging in");
            }
        });

    CROW_ROUTE(app, "/signup").methods(crow::HTTPMethod::Post)
        ([hash_table](const crow::request& req) 
        {
            try
            {
                user temp_user = create_user_from_request(hash_table, req.body);
                hash_table->add_user(temp_user);
                return crow::response(200, "Works");
            }
            catch (username_taken_exception& e)
            {
                // HANDLE EXCEPTION BY TELLING END-USER
                CROW_LOG_INFO << e.what();
                return crow::response(409, "username already taken");
            }
        });

    app.port(8080).multithreaded().run();
    return 0;
}