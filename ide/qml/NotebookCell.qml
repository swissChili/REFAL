import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0

Item {
    id: root

    required property string code
    required property string result

    signal insertBelowClicked()
    signal codeEditingFinished(string code)
    signal cellFocused()

    height: column.height

    MouseArea {
        id: selectCell

        anchors.fill: column

        onClicked: root.cellFocused()
    }

    ColumnLayout {
        id: column

        width: parent.width

        RowLayout {
            id: row
            Layout.fillWidth: true

            RoundButton {
                Layout.alignment: Qt.AlignTop
                icon.source: "qrc:///icons/play-circle.svg"
                icon.color: Material.color(Material.Grey, Material.Shade600)
                flat: true
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: code
                    font.family: "monospace"
                    text: root.code
                    selectByMouse: true
                    wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere

                    Keys.onTabPressed: {
                        var pos = cursorPosition + 4
                        text = text.slice(0, cursorPosition) + "    " + text.slice(cursorPosition);
                        cursorPosition = pos
                    }

                    Keys.onEscapePressed: {
                        root.forceActiveFocus()
                    }

                    onEditingFinished: {
                        root.codeEditingFinished(text)
                    }

                    onFocusChanged: if (focus) root.cellFocused()
                }

                Label {
                    visible: root.result.length > 0
                    Layout.fillWidth: true
                    font.family: "monospace"
                    text: root.result
                }
            }

            RoundButton {
                icon.source: "qrc:///icons/trash.svg"
                icon.color: Material.color(Material.Grey, Material.theme == Material.Dark ? Material.Shade400 : Material.Shade600)
                flat: true
            }
        }

        InsertRow {
            onInsertClicked: root.insertBelowClicked()
        }
    }
}
