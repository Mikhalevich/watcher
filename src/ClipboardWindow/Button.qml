
import QtQuick 1.0

Rectangle {
	id: button
	width: 150; height: 75

	Text {
		id: buttonLabel
		anchors.centerIn: parent
		text: label
	}

	property string label: "button label"
	property color buttonColor: "lightblue"
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
	
	color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}