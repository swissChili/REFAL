import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0

Rectangle {
    id: root

    color: Material.color(Material.Grey, Material.Shade800)
    height: 2
    Layout.fillWidth: true
    Layout.topMargin: 14
    Layout.bottomMargin: 14

    transitions: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 100
            easing.type: Easing.OutCubic
        }
    }

    MouseArea {
        id: mouseArea
        height: 30
        width: parent.width
        anchors.centerIn: parent
        hoverEnabled: true

        states: [
            State {
                when: mouseArea.containsMouse
                PropertyChanges {
                    target: root
                    opacity: 1
                }
            },
            State {
                when: !mouseArea.containsMouse
                PropertyChanges {
                    target: root
                    opacity: 0
                }
            }
        ]
    }

    Item {
        id: insertRow
        anchors.centerIn: parent
        height: 24
        width: 24

        Button {
            id: addButton
            anchors.centerIn: parent
            icon.source: "qrc:///icons/add.svg"
            icon.color: Material.color(Material.Grey, Material.Shade400)
            flat: true
        }
    }
}
