let BMM_IconClose = $('.BMM-BurgerMenuClose');
let BMH_IconClose = $('.BMH-BurgerMenuClose');
let BM_Icon = $('.Header-BurgerMenuIcon');
let APP_SwipeStart = 0;
let APP_SwipeEnd = 0;



$('body').on('touchstart', function (event) {
    APP_SwipeStart = event.originalEvent.touches[0].pageX;
})
$('body').on('touchend', function (event) {
    APP_SwipeEnd = event.originalEvent.changedTouches[0].pageX;
    if (APP_SwipeEnd - APP_SwipeStart >= 100) {
        BM_Show();
    } else if (APP_SwipeStart - APP_SwipeEnd >= 100) {
        BM_Hide();
    }
})
BMM_IconClose.click(() => {
    BM_Hide();
});
BMH_IconClose.click(() => {
    BM_Hide();
});
BM_Icon.click(() => {
    BM_Show();
});



function HTTP_Request() {
    document.cookie = `inform=${encodeURIComponent('{"type":"devicecheck","systemid":"000000002","password":"123456"}')}; max-age=300`;

    let APP_Cookie = document.cookie;
    if (APP_Cookie) {
        let APP_CookieJson = JSON.parse(APP_Cookie.replace("inform=", ""));
        APP_CookieJson["type"] = "getinfo";

        let xhr = new XMLHttpRequest();
        xhr.open('GET', `http://192.168.0.138:1337/${encodeURIComponent(JSON.stringify(APP_CookieJson))}`, true);
        xhr.withCredentials = false;
        xhr.send();

        xhr.onload = () => {
            HTTP_Working(xhr.responseText);
        };

        xhr.onerror = () => {
            window.location.href = './server-error.html';
        };
    } else {
        window.location.href = './registration.html';
    }
}

function HTTP_Working(HTTP_Response) {
    if (HTTP_Response === "0" && HTTP_Response === "-1") {
        alert("Smart Env server error");
    } else {
        document.querySelector(".Main-Information").innerHTML = HTTP_Response;
    }
}

function BM_Show() {
    anime({
        targets: '.Header-BurgerMenuIcon',
        opacity: ['1', '0'],
        duration: 500,
        loop: false
    });

    anime({
        targets: ['.BM-Content', '.BM-BlackScreen'],
        translateX: ['-150%', '0'],
        easing: 'easeInOutQuad',
        direction: 'alternate',
        duration: 1000,
        loop: false
    });
}

function BM_Hide() {
    anime({
        targets: ['.BM-Content', '.BM-BlackScreen'],
        translateX: ['0', '-150%'],
        easing: 'easeInOutQuad',
        direction: 'alternate',
        duration: 1000,
        loop: false
    });

    setTimeout(() => {
        anime({
            targets: '.Header-BurgerMenuIcon',
            opacity: ['0', '1'],
            duration: 500,
            loop: false
        })
    }, 1000);
}
