// Update LED color upon button press
function changeColor(button) {
    var xhttp = new XMLHttpRequest();
    var x = button.id;

    switch (x) {
        case 'red':
            xhttp.open("GET", "/red");
            xhttp.send();
            break;
        case 'green':
            xhttp.open("GET", "/green");
            xhttp.send();
            break;
        case 'blue':
            xhttp.open("GET", "/blue");
            xhttp.send();
            break;
        case 'purple':
            xhttp.open("GET", "/purple");
            xhttp.send();
            break;
        case 'yellow':
            xhttp.open("GET", "/yellow");
            xhttp.send();
            break;
    }
}

// Update LED brightness upon button press
function changeBrightness(button) {
    var xhttp = new XMLHttpRequest();
    var x = button.id;

    switch (x) {
        case 'low':
            xhttp.open("/GET", "/low");
            xhttp.send();
            break;
        case 'mid':
            xhttp.open("/GET", "/mid");
            xhttp.send();
            break;
        case 'high':
            xhttp.open("/GET", "/high");
            xhttp.send();
            break;
    }
}