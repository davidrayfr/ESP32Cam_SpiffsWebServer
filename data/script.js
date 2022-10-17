function saveButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "SAVE", true);
    xhttp.send();
}

function restartButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "RESTART", true);
    xhttp.send();
}

setInterval(function getData()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("valeurLuminosite").innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", "lireLuminosite", true);
    xhttp.send();
}, 2000);
