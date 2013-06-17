
// On récupère le canvas sur la page
var cv = document.getElementById('shape');
var ctx = cv.getContext("2d");

// Stock des coordonnées
var coordX;
var coordY;

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

});

// Et on dessine !
ctx.stroke();
