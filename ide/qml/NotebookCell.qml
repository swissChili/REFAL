import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0

ColumnLayout {
    id: root

    required property string code
    required property string result

    RowLayout {
        Layout.fillWidth: true

        Button {
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
                    text += "    ";
                    cursorPosition = pos
                }
            }

            Label {
                Layout.fillWidth: true
                font.family: "monospace"
                text: root.result
            }
        }
    }

    InsertRow {

    }
}
