var configString = "";
var pos = 1;
var config = [];

function generateControls(number) {
    for(var i = 0; i < number; i++) {
        var link = "/action?id="+i+"&state=";
        addElement(i, config[pos++], link, config[pos++], config[pos++], config[pos++]);
    }
}

function addElement(index, text, link, pulseEnable, onEnable, offEnable) {
    console.log(index);
    var controlContainer = document.getElementById("controlContainer");
    var controlElement = document.createElement("div");
    var controlBox = document.createElement("div");
    controlElement.className = "controlElement";
    controlElement.id = index + "element";
    controlBox.id = index + "box";
    controlBox.className = "controlBox";
    controlElement.innerHTML += text;
    controlContainer.appendChild(controlElement);
    
    var pulseButton = document.createElement("a");
    var pulseButtonImg = document.createElement("img");
    pulseButton.className = "controlButton";
    pulseButton.href = link + "0";
    pulseButton.id = index + "pulse";
    pulseButtonImg.src = "pulse.svg";
    pulseButtonImg.className = "buttonImg";
    pulseButton.appendChild(pulseButtonImg);
    var onButton = document.createElement("a");
    var onButtonImg = document.createElement("img");
    onButton.className = "controlButton";
    onButton.href = link + "1";
    onButton.id = index + "on";
    onButtonImg.src = "on.svg";
    onButtonImg.className = "buttonImg";
    onButton.appendChild(onButtonImg);
    var offButton = document.createElement("a");
    var offButtonImg = document.createElement("img");
    offButton.className = "controlButton";
    offButton.href = link + "2";
    offButton.id = index + "off";
    offButtonImg.src = "off.svg";
    offButtonImg.className = "buttonImg";
    offButton.appendChild(offButtonImg);
    

    if (pulseEnable == 1) {
        controlBox.appendChild(pulseButton);
    }
    if (onEnable == 1) {
        controlBox.appendChild(onButton);
    }
    if (offEnable == 1) {
        controlBox.appendChild(offButton);
    }

    controlElement.appendChild(controlBox);

}

var done = false;
var url = "/relays.conf";
var configFile = new XMLHttpRequest();
    configFile.open("GET",url,true);
    configFile.onreadystatechange = function() {
        if (configFile.readyState== 4 || configFile.status == 200 || configFile.status == 0) {
            if (done == false) {
                configString = configFile.responseText;
                if (configString.length > 1) {
                    done = true;
                }
                console.log(configString);
                config = configString.split(",");
                var n = config[0];
                console.log(n);
                generateControls(n);
                console.log(config);
            }
        }
    }
configFile.send(null);
