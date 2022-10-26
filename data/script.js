//Fonction de commande Javascript avec Jquery

$(document).ready(function(){
    $("#saveButton").click(function(){
        var valeur = $("#inputhost").val();
        if (confirm("Vous confirmer le redémarrage ?")) {
            $.post("receivedData",{
            hostname: valeur,
            nomssid: valeur,
            wifipassword: valeur,
            portrtsp: valeur}
            );
            }});
    });

    $(document).ready(function(){
        $("#appliquer").click(function(){
            var valeur = $("#choixDelayLed").val();
            $.post("delayLed",{
                valeurDelayLed: valeur
            });
        });
    });
    

function saveButton() {
    var xhttp = new XMLHttpRequest();
    if (confirm("Confirmer la sauvegarde ?")) {
        xhttp.open("GET", "save", true);
        xhttp.send();
        };
}

function reinitButton() {
    var xhttp = new XMLHttpRequest();
    if (confirm("Confirmer la reinitialisation ?\n (Attention les données de connection seront perdues)")) {
        xhttp.open("GET", "reinit", true);
        xhttp.send();
        };
}

function restartButton() {
    var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "restart", true);
        xhttp.send();
}

//setInterval(function getData2()
//{
//    var xhttp = new XMLHttpRequest();
//    xhttp.onreadystatechange = function()
//    {                                                                               
//        if(this.readyState == 4 && this.status == 200)
//        {
//            document.getElementById("inputssid").innerHTML = this.responseText;
//        }
//    };
//    xhttp.open("GET", "envoid1", true);
//    xhttp.send();
//}, 2000);

function getData()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
        document.getElementById("displayversion").innerHTML = this.responseXML.getElementsByTagName('version')[0].childNodes[0].nodeValue; 
        document.getElementById("displayipadresse").innerHTML = this.responseXML.getElementsByTagName('ipadresse')[0].childNodes[0].nodeValue; 
        document.getElementById("displayssid").innerHTML = this.responseXML.getElementsByTagName('namessid')[0].childNodes[0].nodeValue; 
        document.getElementById("displayhostname").innerHTML = this.responseXML.getElementsByTagName('hostname')[0].childNodes[0].nodeValue;     
        document.getElementById("displayportrtsp").innerHTML = this.responseXML.getElementsByTagName('portrtsp')[0].childNodes[0].nodeValue;     
        var element=document.getElementById("inputhostname").innerHTML;
        }
    };
    xhttp.open("GET", "sendData", true);
    xhttp.send();
};

