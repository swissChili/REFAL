import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.0

RowLayout {
    id: root

    property string title: "title"
    property string url: "https://swisschili.sh"
    property alias containsMouse: mouseArea.containsMouse

    Image {
        id: nbIcon
        width: 100
        height: 100
        source: "qrc:///icons/book.svg"
        fillMode: Image.PreserveAspectFit
    }

    Label {
        text: title
        Layout.fillWidth: true

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onClicked: Qt.openUrlExternally(url)
        }
    }
}
