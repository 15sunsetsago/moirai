#include "vedas.h"

/* Parse Form to get user data*/

// Make sure passwords are alphanumeric only


user handle_signup_request(std::string request_body)
{
    int pos_and = request_body.find_first_of('&');
    std::string first = request_body.substr(pos_and + 1);
    std::string second = request_body.substr(0, pos_and);

    int equal_pos_one = first.find('=');
    int equal_pos_two = second.find('=');

    std::string user_name = first.substr(equal_pos_one + 1);
    std::string password = second.substr(equal_pos_two + 1);

    user temp(user_name, password);
    std::cout << temp.get_user_name() << " " << temp.get_password(); 
    return temp;
}

// Adds to User_map
void sign_up(std::string request_body)
{
    user myself = handle_signup_request(request_body);
}
