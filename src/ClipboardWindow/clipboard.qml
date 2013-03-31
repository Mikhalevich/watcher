
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
        signal getLastClipboard()
	
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

                // get last clipboard
                Button {
                    id: getLastClipboard
                    label: "Get Last Clipboard"

                    onButtonClick: {
                        rect.getLastClipboard();
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
                height: parent.height / 3

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

            ListView {
                id: clipboardList
                width: parent.width - 1
                height: parent.height * 2 / 3
                clip: true

                model: clipboardModel
                delegate: itemDelegate
                highlight: itemHighlight
            }

        } // Column

        // list view delegat
        Component {
            id: itemDelegate

            Item {
                width: parent.width - 1;
                height: 40
                clip: true

                Text {
                    id: itemDelegateText
                    text: display
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        clipboardList.currentIndex = index
                        clipboardText.text = itemDelegateText.text
                    }
                }
            }
        }

        Component {
            id: itemHighlight

            Rectangle {
                width: parent.width - 1
                height: 40
                color: "lightsteelblue"
                radius: 5

                y: clipboardList.currentItem.y
                Behavior on y {
                    SpringAnimation {
                        spring: 5
                        damping: 0.2
                    }
                }
            }
        }
}
