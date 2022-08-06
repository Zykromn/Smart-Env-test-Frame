LNCH_Preapare();

function LNCH_Working(LNCH_Flag) {
    setTimeout(() => {
        if (LNCH_Flag === 0) {
            window.location.href = './registration.html';
        } else if (LNCH_Flag === 1) {
            window.location.href = '../system.html';
        } else if (LNCH_Flag === 2) {
            window.location.href = './success-access.html';
        } else {
            window.location.href = './server-error.html';
        }
    }, 2000)
}


function LNCH_Preapare() {
    // document.cookie = `inform=${encodeURIComponent('{"type":"devicecheck","systemid":"000000002","password":"123456"}')}; max-age=300`;

    let APP_Cookie = document.cookie;
    if (APP_Cookie) {
        let xhr = new XMLHttpRequest();
        xhr.open('GET', `http://192.168.0.138:1337/${APP_Cookie.replace("inform=", "")}`, true);
        xhr.withCredentials = false;
        xhr.send();

        xhr.onload = () => {
            LNCH_Working(Number(xhr.responseText));
        };

        xhr.onerror = () => {
            window.location.href = './server-error.html';
        };
    } else {
        window.location.href = './registration.html';
    }
}
