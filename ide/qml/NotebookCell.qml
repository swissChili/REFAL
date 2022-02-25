import QtQuick 2.5
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0

import sh.swisschili.REFAL 1.0

Item {
    id: root

    required property string code
    required property string result
    property int status: Cell.IDLE
    property int resultType: Cell.EXPRESSION
    property bool cellActive: false

    signal insertBelowClicked()
    signal codeEditingFinished(string code)
    signal cellFocused()
    signal cellUnfocused()
    signal runClicked()
    signal deleteClicked()

    height: column.height

    Keys.onEscapePressed: {
		root.cellUnfocused()
		code.focus = false
	}

    ColumnLayout {
        id: column

        width: parent.width - 20
        anchors.centerIn: parent

        Item {
            implicitWidth: row.implicitWidth
            implicitHeight: row.implicitHeight
            Layout.fillWidth: true

            Pane {
                anchors.fill: parent
                anchors.topMargin: -5
                anchors.bottomMargin: -5

                Material.elevation: root.cellActive ? 4 : 0
            }

            MouseArea {
                id: selectCell

                anchors.fill: row

                onClicked: root.cellFocused()
            }

            RowLayout {
                anchors.fill: parent
                id: row

                RoundButton {
                    Layout.alignment: Qt.AlignTop
                    icon.source: iconForState(root.state)
                    icon.color: Material.color(Material.Grey, Material.Shade600)
                    flat: true

                    onClicked: root.runClicked()

                    function iconForState(state) {
                        if (state === Cell.RUNNING)
                            return "qrc:///icons/square.svg"

                        return "qrc:///icons/play-circle.svg"
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: code

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        font.family: "monospace"
                        text: root.code
                        selectByMouse: true
                        wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere

                        placeholderText: "Write some code..."

                        Keys.onTabPressed: {
                            var pos = cursorPosition + 4
                            text = text.slice(0, cursorPosition) + "    " + text.slice(cursorPosition);
                            cursorPosition = pos
                        }

                        onTextChanged: {
                            root.codeEditingFinished(text)
                        }

                        onPressed: root.cellFocused()

                        onFocusChanged: if (focus) root.cellFocused()
                        onActiveFocusChanged: if (activeFocus) root.cellFocused()
                    }

                    Label {
                        visible: root.result.length > 0
                        Layout.fillWidth: true
                        font.family: "monospace"
                        text: root.result
                        textFormat: root.resultType === Cell.EXPRESSION ? Text.PlainText : Text.RichText

                        Layout.bottomMargin: 5
                    }
                }

                RoundButton {
					Layout.alignment: Qt.AlignTop

                    icon.source: "qrc:///icons/menu.svg"
                    icon.color: Constants.buttonGrey
                    flat: true

                    onClicked: cellContextMenu.popup()

                    Menu {
                        id: cellContextMenu

                        MenuItem {
                            icon.source: "qrc:///icons/trash.svg"
                            icon.color: Material.color(Material.Red)
                            text: "Delete"

                            onClicked: root.deleteClicked()
                        }
                    }
                }
            }
        }

        InsertRow {
            onInsertClicked: root.insertBelowClicked()
        }
    }
}
