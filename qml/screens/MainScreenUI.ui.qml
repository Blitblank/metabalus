

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import MyApp 1.0

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height
    color: "#181818"

    Slider {
        id: slider
        x: 300
        y: 282
        value: 0.5
        rotation: -90
        live: true
    }

    TextInput {
        id: sliderMax
        x: 360
        y: 179
        width: 80
        height: 20
        color: "#b9eaff"
        text: qsTr("100")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
    }

    TextInput {
        id: sliderMin
        x: 360
        y: 401
        width: 80
        height: 20
        color: "#b9eaff"
        text: qsTr("100")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
    }

    TextInput {
        id: sliderValue
        x: 360
        y: 153
        width: 80
        height: 20
        color: "#63b9de"
        text: qsTr("100")
        font.pixelSize: 30
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
    }

    Button {
        id: buttonReset
        x: 359
        y: 504
        width: 81
        height: 32
        text: qsTr("Reset")
    }

    Button {
        id: buttonIncrement
        x: 359
        y: 542
        text: qsTr("Increment")
    }

    Text {
        id: text1
        x: 340
        y: 471
        width: 120
        height: 27
        color: "#e8cece"
        text: qsTr("Text")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
    }
    states: [
        State {
            name: "clicked"
        }
    ]
}
