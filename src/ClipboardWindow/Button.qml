
import QtQuick 2.0

Rectangle {
    id: button
    width: 100; height: 50
        radius: 15
        border.color: borderColor

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: label
    }

    property string label: "button label"
    property color buttonColor: "#835da5"
    property color onHoverColor: "gold"
    property color borderColor: "white"

    signal buttonClick()

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent

        onClicked: buttonClick()

        hoverEnabled: true
        onEntered: parent.border.color = onHoverColor
        onExited:  parent.border.color = borderColor
    }

    color: buttonMouseArea.pressed ? Qt.lighter(buttonColor, 1.5) : buttonColor
}
