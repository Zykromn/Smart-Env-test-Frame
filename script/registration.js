let FORM_Button = document.querySelector(".formButton");
FORM_Button.addEventListener("click", FORM_OnHandle);

function FORM_OnHandle() {
    let FORM_Div = document.querySelector(".registration")
    FORM_Div.style.color = "#ffffff"
    FORM_Div.style.fontSize = "2em"
    FORM_Div.innerHTML = "Form sended!"
}
