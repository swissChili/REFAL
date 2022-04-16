import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.0

RowLayout {
    id: root

    property string name: "Hello.refnb"
    property alias containsMouse: mouseArea.containsMouse
    signal clicked()

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

            onClicked: root.clicked()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:24;width:300}
}
##^##*/
