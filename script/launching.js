document.addEventListener("DOMContentLoaded", () => {
    setTimeout(LNCH_Ready, 2000);
})


function LNCH_Ready() {
    window.location.href = './registration.html'
}

function LNCH_Preapare() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'phones.json', false);
    xhr.send();

    if (xhr.status != 200) {
        alert( xhr.status + ': ' + xhr.statusText );
    } else {
        alert( xhr.responseText );
    }
}
