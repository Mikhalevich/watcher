
import QtQuick 1.0

Rectangle {
	id: rect
	width: 500
	height: 250
	anchors.fill: parent
        color: activePalette.window
	
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

        // palette
        SystemPalette {
            id: activePalette
        }

        Column {
            anchors.fill: parent
            spacing: 10
	
            // header buttons
            Row {
                id: header
                spacing: 5

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

                Button {
                    id: clearButton
                    label: "Clear"

                    onButtonClick: {
                        clipboardText.text = ""
                    }
                }
            }

            // text border
            Rectangle {
                border.color: "black"

                width: parent.width - 1
                height: parent.height / 2

                // text area
                Flickable {
                        id: flickArea
                        anchors.fill: parent

                        contentWidth: clipboardText.paintedWidth; contentHeight: clipboardText.paintedHeight
                        flickableDirection: Flickable.VerticalFlick
                        clip: true

                        function ensureVisible(r)
                        {
                            if (contentX >= r.x)
                                contentX = r.x;
                            else if (contentX+width <= r.x+r.width)
                                contentX = r.x+r.width-width;

                            if (contentY >= r.y)
                                contentY = r.y;
                            else if (contentY+height <= r.y+r.height)
                                contentY = r.y+r.height-height;
                        }

                        TextEdit {
                                id: clipboardText
                                readOnly: false

                                anchors.fill: parent

                                onCursorRectangleChanged: flickArea.ensureVisible(cursorRectangle)
                        }
                }
            }
        }
}
