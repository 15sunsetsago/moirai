document.addEventListener("DOMContentLoaded", () => {
    const loginFunc = document.getElementById("login-func");
    const signupFunc = document.getElementById("signup-func");
    const loginBtn = document.getElementById("show-login");
    const signupBtn = document.getElementById("show-signup");

    if (loginBtn) {
        loginBtn.addEventListener("click", () => {
            loginFunc.classList.remove("hidden");
            signupFunc.classList.add("hidden");
        });
    }

    if (signupBtn) {
        signupBtn.addEventListener("click", () => {
            signupFunc.classList.remove("hidden");
            loginFunc.classList.add("hidden");
        });
    }
});

function hideFunc(id) {
    const modal = document.getElementById(id);
    if (modal) modal.classList.add("hidden");
}
