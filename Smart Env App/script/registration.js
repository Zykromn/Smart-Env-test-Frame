let FORM_Button = document.querySelector(".Form-Button");
FORM_Button.addEventListener("click", FORM_OnHandle);



function FORM_OnHandle() {
    let FORM_ValuesSYSTEMID = document.querySelector(".System-Id").value;
    let FORM_ValuesPASSWORD = document.querySelector(".System-Pas").value;

    let xhr = new XMLHttpRequest();
    xhr.open('GET', `http://192.168.0.138:1337/{"type":"getinfobj","systemid":"000000001"}`, true);
    xhr.withCredentials = false;
    xhr.send();

    xhr.onload = () => {
        FORM_Working(Number(xhr.responseText));
    };

    xhr.onerror = () => {
        window.location.href = './server-error.html';
    };
}

function FORM_Working(HTTP_ResponseFlag) {
    alert("НАЧАЛО")
    alert(HTTP_ResponseFlag)
    if (HTTP_ResponseFlag === 0) {
        alert("NOPE")
    } else if (HTTP_ResponseFlag === 1) {
        alert("OLD")
        window.location.href = "./../system.html"
    } else if (HTTP_ResponseFlag === 2) {
        alert("NEW")
        // window.location.href = "./success-access.html"
    } else {
        alert("ELSE")
    }
}
