var dynamicIPButton = document.getElementById('dynamic');
dynamicIPButton.addEventListener('click', function() {
    document.getElementById('ipaddr').disabled = true;
    document.getElementById('gateway').disabled = true;
    document.getElementById('mask').disabled = true;
});
var staticIPButton = document.getElementById('static');
staticIPButton.addEventListener('click', function() {
    document.getElementById('ipaddr').disabled = false;
    document.getElementById('gateway').disabled = false;
    document.getElementById('mask').disabled = false;
});

var submitButton = document.getElementById('submitButton');
submitButton.addEventListener('click', function() {
    var config = "1,";
    config += document.getElementById('ssid').value + ",";
    config += document.getElementById('password').value + ",";
    config += document.getElementById('address').value + ",";
    if (document.getElementById('static').checked) {
        config += "static,";
    } else {
        config += "dynamic,";
    }
    config += document.getElementById('ipaddr').value + ",";
    config += document.getElementById('gateway').value + ",";
    config += document.getElementById('mask').value + ",";

    console.log(config);
    // send string as body in POST request to "/"
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/", true);
    xhr.setRequestHeader('Content-Type', 'plain/text');
    xhr.send(config);
    xhr.onreadystatechange = function() {
        if (xhr.readyState == XMLHttpRequest.DONE) {
            console.log(xhr.responseText);
        }
    }

});
