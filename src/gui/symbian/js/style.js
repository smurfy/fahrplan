// This JS is stateless - declare it as a library so only one copy is loaded
.pragma library

// Normal
var listBackgroundOdd = "#222"
var listBackgroundEven = "#444"
var listBackgroundHighlight = "#888"

// Inverted
var listBackgroundOddInverted = "#DDD"
var listBackgroundEvenInverted = "#BBB"
var listBackgroundHighlightInverted = "#888"

function getIconFromQrc(inverted, icon, type)
{
    var t = (typeof type == "undefined") ? ".png" : type;
    return "qrc:/src/gui/symbian/icon/" + icon + (inverted ? "-inverse" : "") + t;
}

function getIconFromTheme(inverted, icon)
{
    return "image://theme/" + icon + (inverted ? "_inverse" : "");
}
