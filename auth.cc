#include "vedas.h"

/* Parse Form to get user data*/

// Make sure passwords are alphanumeric only

// RENAME THIS FUNCTION
// make this function void and add_user in function()
user create_user_from_request(std::shared_ptr<citadel> hash_table, std::string request_body)
{
    int pos_and = request_body.find_first_of('&');
    std::string first = request_body.substr(0, pos_and);
    std::string second = request_body.substr(pos_and + 1);

    int equal_pos_one = first.find_first_of('=');
    int equal_pos_two = second.find_first_of('=');

    std::string user_name = first.substr(equal_pos_one + 1);
    std::string password = second.substr(equal_pos_two + 1);

    bool check = hash_table->is_user_in_map(user_name);
    if (check) throw username_taken_exception();

    user temp(user_name, password);
    return temp;
}

user create_user_for_login(std::string request_body)
{
    int pos_and = request_body.find_first_of('&');
    std::string first = request_body.substr(0, pos_and);
    std::string second = request_body.substr(pos_and + 1);

    int equal_pos_one = first.find_first_of('=');
    int equal_pos_two = second.find_first_of('=');

    std::string user_name = first.substr(equal_pos_one + 1);
    std::string password = second.substr(equal_pos_two + 1);

    user temp(user_name, password);
    return temp;
}

void login_validation(std::shared_ptr<citadel> hash_table, user person)
{
    std::string user_name = person.get_user_name();

    // First check if username is in hash_table
    bool check = hash_table->is_user_in_map(user_name);
    if (!check) throw login_incorrect_exception();

    // Compare password hashes
    user db = hash_table->retrieve_user(user_name);
    if (person.get_password() != db.get_password()) throw login_incorrect_exception();
}