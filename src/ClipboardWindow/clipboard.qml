
import QtQuick 1.0

Rectangle {
	id: rect
	width: 500
	height: 250
	anchors.fill: parent
	color: "gray"
	
	signal getClipboard()
	
	Button {
		id: getClipboardButton
		anchors.centerIn: parent
		label: "Get clipboard"
		
		onButtonClick: {
			rect.getClipboard()
		}
	}
}