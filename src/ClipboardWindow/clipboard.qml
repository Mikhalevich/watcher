
import QtQuick 1.0

Rectangle {
	id: rect
	width: 500
	height: 250
	anchors.fill: parent
	color: "gray"
	
	// signals
	signal getClipboard()
	
	// connections
	Connections {
		target: clipboardWindow
		onTextData: {
			clipboardText.text = text
		}
	}
	
	Button {
		id: getClipboardButton
		
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		
		label: "Get clipboard"
		
		onButtonClick: {
			rect.getClipboard()
		}
	}
	
	Flickable {
		id: flickArea
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.top: getClipboardButton.bottom
			
		contentWidth: clipboardText.width; contentHeight: clipboardText.height
		flickableDirection: Flickable.VerticalFlick
		clip: true
		
		TextEdit {
			id: clipboardText
			readOnly: true
			
			anchors.fill: parent
		}
	}
}