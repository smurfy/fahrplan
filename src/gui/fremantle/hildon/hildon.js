
function themeFilename(name) {
    return "file://etc/hildon/theme/images/" + name + ".png";
}

function iconFilename(name) {
    return "file://usr/share/icons/hicolor/48x48/hildon/" + name + ".png";
}

function themedButton(basename, sensitive, pressed) {
    if (sensitive) {
        if (pressed) {
            return themeFilename(basename + "Pressed");
        } else {
            return themeFilename(basename + "Normal");
        }
    } else {
        return themeFilename(basename + "Disabled");
    }
}

