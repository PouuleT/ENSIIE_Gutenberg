// On récupère le canvas sur la page
var cv = document.getElementById('shape');
var ctx = cv.getContext("2d");

// Stock des coordonnées
var coordX;
var coordY;

var distancePU;
var distancePD;

var materiel = "defaut";

$("#tempsDecoupe").html("---");

// Pour chaque valeur du tableau de résultats $result aka res
$.each(res, function(cle, val){
    
    // Initialisation de la forme
    if (val.indexOf("LTPU") != -1){
        coordX = Number(val.substring(val.indexOf('[')+1,val.indexOf('-')))/6;
        coordY = Number(val.substring(val.indexOf('-')+1,val.indexOf(']')))/6;
        
        if (coordX && coordY) ctx.moveTo(coordX,coordY);
    } 
    
    // Découpe
    if (val.indexOf("PD") != -1){
        coordX = Number(val.substring(val.indexOf('B[')+2,val.lastIndexOf(',')))/6;
        coordY = Number(val.substring(val.lastIndexOf(',')+1,val.lastIndexOf(']')))/6;
        
        if (coordX && coordY) ctx.lineTo(coordX,coordY);

    }
    
    // Déplacement de la tête
    if (val.indexOf("PU") != -1){
        coordX = Number(val.substring(val.indexOf('B[')+2,val.lastIndexOf(',')))/6;
        coordY = Number(val.substring(val.lastIndexOf(',')+1,val.lastIndexOf(']')))/6;

        if (coordX && coordY) ctx.moveTo(coordX,coordY);

    }
    
    if (val.indexOf("Distance totale PD") != -1){
        distancePD = Number(val.substring(val.indexOf('Distance totale PD')+21, val.indexOf(", PU : ")));
    }
    
    if (val.indexOf(", PU : ") != -1){
        distancePU = Number(val.substr(val.indexOf(", PU : ")+7));
    }

});

// Et on dessine !
ctx.stroke();

// Selection du matériel
$("#materiel").on('click', function(){

    materiel = $("#materiel")[0].value;
    
    // Calculs de la durée
    if (materiel == "plexi") $("#tempsDecoupe").html(Math.round(0.007*distancePD*2.27+0.007*distancePU*0.1));
    else if (materiel == "carton") $("#tempsDecoupe").html(Math.round(0.007*distancePD*0.16+0.007*distancePU*0.1));
    else $("#tempsDecoupe").html("---");

});




