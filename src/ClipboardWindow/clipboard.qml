
import QtQuick 1.0

Rectangle {
	id: rect
	width: 500
	height: 250
	anchors.fill: parent
        color: "white"
	
	// signals
	signal getClipboard()
        signal setClipboard(string text)
	
	// connections
	Connections {
		target: clipboardWindow
		onTextData: {
			clipboardText.text = text
		}
	}
	
        // header buttons
        Row {
            id: header
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            // get clipboard button
            Button {
                id: getClipboardButton
                label: "Get clipboard"

                onButtonClick: {
                        rect.getClipboard()
                }
            }

            // set clipboard button
            Button {
                id: setClipboardButton
                label: "Set clipboard"

                onButtonClick:
                {
                    setClipboard(clipboardText.text)
                }
            }
        }

        // text area
        Flickable {
		id: flickArea
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: header.bottom
			
                contentWidth: clipboardText.width; contentHeight: clipboardText.height
		flickableDirection: Flickable.VerticalFlick
                clip: true

                TextEdit {
                        id: clipboardText
                        readOnly: false

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom*2
                        anchors.top: parent.top
                }
        }
}
