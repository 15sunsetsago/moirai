#include "sight.h"

struct sessionHandler {
    struct context {
        std::string username;
        bool logged_in = false;
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);
};

void sessionHandler::before_handle(crow::request& req, crow::response& res, sessionHandler::context& ctx) {
    std::string cookie = req.get_header_value("Cookie");

    if (!cookie.empty()) {
        std::unordered_map<std::string, std::string> cookies = parseCookies(cookie);
        if (cookies.count("username")) {
            ctx.username = cookies["username"];
            ctx.logged_in = true;
        }
    }
}

void sessionHandler::after_handle(crow::request&, crow::response&, sessionHandler::context&) {
    // No-op or you can add response header modifications here
}
