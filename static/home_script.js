document.addEventListener("DOMContentLoaded", () =>
{
    const login_function = document.getElementById("login_form");
    const signup_function = document.getElementById("signup_form");
    const login_button = document.getElementById("show_login");
    const signup_button = document.getElementById("show_signup");

    if (login_button)
    {
        login_button.addEventListener("click", () =>
        {
            login_function.class_list.remove("hidden");
            signup_function.class_list.add("hidden");
        });
    }

    if (signup_button)
    {
        signup_button.addEventListener("click", () =>
        {
            signup_function.class_list.remove("hidden");
            login_function.class_list.add("hidden");
        }
        );
    }
});

function hide_form(id) {
    const hidden_function = document.getElementById(id);
    if (hidden_function) hidden_function.class_list.add("hidden");
}
