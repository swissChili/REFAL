import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.0

RowLayout {
    id: root

    property string name: "Hello.refnb"
    property alias containsMouse: mouseArea.containsMouse
    signal clicked()
    signal removeClicked()

    Image {
        id: nbIcon
        width: 100
        height: 100
        source: "qrc:///icons/document.svg"
        fillMode: Image.PreserveAspectFit
    }

    Label {
        text: name
        Layout.fillWidth: true

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
                if (mouse.button == Qt.LeftButton)
                    root.clicked();
                else if (mouse.button == Qt.RightButton)
                    contextMenu.popup();
            }
        }

        Menu {
            id: contextMenu

            MenuItem {
                text: qsTr("Remove")

                onClicked: root.removeClicked()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:24;width:300}
}
##^##*/
