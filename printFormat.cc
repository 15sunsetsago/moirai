#include "vedas.h"

using json = nlohmann::json;

std::string fetchAndFormatMediaResults(const std::string& query)
{
    std::ifstream key_file("key.json");
    json key = json::parse(key_file);

    std::string api_key = key["TMDB_KEY"];
    std::string url = "https://api.themoviedb.org/3/search/multi?api_key=" + api_key + "&query=" + cpr::util::urlEncode(query);

    cpr::Response cprMediaResponse = cpr::Get(cpr::Url{ url });

    if (cprMediaResponse.status_code != 200)
    {
        crow::mustache::context error_ctx;
        error_ctx["title"] = "tmdb api error";
        error_ctx["message"] = "api failed to fetch result. status code: " + std::to_string(cprMediaResponse.status_code);
        return crow::mustache::load("error.html").render_string(error_ctx);
        //return "<p>API ERROR FAILED TO FETCH TMDB STATUS CODE: " + std::to_string(cprMediaResponse.status_code) + "</p>";
    }

    json data = json::parse(cprMediaResponse.text);

    if (!data.contains("results") || !data["results"].is_array() || data["results"].empty())
    {
        crow::mustache::context error_ctx;
        error_ctx["title"] = "no result found";
        error_ctx["message"] = "no results found for \"" + query + "\".";
        return crow::mustache::load("error.html").render_string(error_ctx);
        //return "<p>NOTHING FOUND FOR \"" + query + "\". TRY AGAIN</p>";
    }

    crow::mustache::context ctx;
    std::vector<crow::mustache::context> results;

    for (std::size_t i = 0; i < data["results"].size(); ++i)
    {
        json item = data["results"][i];
        crow::mustache::context item_ctx;

        if (item.contains("title"))
        {
            item_ctx["title"] = item["title"].get<std::string>();
        }
        else if (item.contains("name"))
        {
            item_ctx["title"] = item["name"].get<std::string>();
        }
        else
        {
            item_ctx["title"] = "Unknown Title";
        }

        if (item.contains("overview"))
        {
            item_ctx["description"] = item["overview"].get<std::string>();
        }
        else
        {
            item_ctx["description"] = "No description available.";
        }

        if (item.contains("release_date"))
        {
            item_ctx["release"] = item["release_date"].get<std::string>();
        }
        else
        {
            item_ctx["release"] = "no release date available";
        }

        if (item.contains("media_type"))
        {
            item_ctx["media_type"] = item["media_type"].get<std::string>();
        }
        else
        {
            item_ctx["media_type"] = "Unknown";
        }

        results.push_back(std::move(item_ctx));
    }

    ctx["results"] = std::move(results);

    return crow::mustache::load("results.html").render_string(ctx);
}