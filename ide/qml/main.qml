import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.11
import Qt.labs.settings 1.0

import sh.swisschili.REFAL 1.0

ApplicationWindow {
    id: root

    title: "REFAL Studio"

    width: 680
    height: 360

    minimumWidth: 680
    minimumHeight: 360

    Material.theme: Material.Light
    Material.accent: Material.Orange

    visible: true

    property alias recentModel: recents

    function openNotebook(path=null) {
        let NbWindow = Qt.createComponent("qrc:///qml/NbWindow.qml");
        let window = NbWindow.createObject(null, {welcomeWindow: root});

        if (path !== null)
        {
            window.openNotebook(path)
        }
    }

    function toggleVisible() {
        if (visible)
            hide();
        else
            show();
    }

    RecentModel {
        id: recents
    }

    Label {
        id: textRefal
        text: qsTr("REFAL")
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: 36
        anchors.leftMargin: 36
        anchors.topMargin: 29
        font.weight: Font.Black
        font.bold: true
        font.italic: false
    }

    Label {
        id: textStudio
        y: 29
        text: qsTr("Studio")
        anchors.verticalCenter: textRefal.verticalCenter
        anchors.left: textRefal.right
        font.pixelSize: 36
        anchors.leftMargin: 6
        font.bold: false
        font.italic: false
        font.weight: Font.Medium
        color: Material.color(Material.Orange)
    }

    Rectangle {
        id: sepRect
        x: 364
        width: 1
        color: "#cecece"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        anchors.topMargin: 78
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Flickable {
        id: notebooksFlick
        anchors.left: textRefal.left
        anchors.right: sepRect.left
        anchors.top: textRefal.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: -8
        anchors.rightMargin: 16
        anchors.bottomMargin: 16
        anchors.topMargin: 6
        clip: true
        flickableDirection: Flickable.VerticalFlick

        ColumnLayout {
            id: notebooksCol
            spacing: 12

            RowLayout {
                id: nbBtnsRow
                spacing: 12
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: false
                Layout.leftMargin: 8
                Layout.rightMargin: 8

                Button {
                    id: newNotebookBtn
                    text: qsTr("New Notebook")
                    font.bold: true
                    highlighted: true

                    onClicked: {
                        root.openNotebook();
                    }
                }

                Button {
                    id: openNotebookBtn
                    text: qsTr("Open Existing")
                }
            }

            Repeater {
                id: notebooksList

                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: false

                model: recents

                delegate: RecentNotebook {
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8

                    name: path.split("/").pop()

                    ToolTip.text: path
                    ToolTip.visible: containsMouse
                    ToolTip.delay: 1000

                    onClicked: root.openNotebook(path)
                    onRemoveClicked: recents.remove(path)
                }
            }

            Label {
                Layout.leftMargin: 8
                Layout.rightMargin: 8

                visible: notebooksList.count == 0
                text: qsTr("Your recent notebooks will appear here")
            }
        }
    }

    ListView {
        id: tipsList
        anchors.left: sepRect.right
        anchors.right: parent.right
        anchors.top: textRefal.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.topMargin: 6
        anchors.leftMargin: 16
        anchors.rightMargin: 36
        spacing: 12

        model: [
            {url: "https://wiki.swisschili.sh/wiki/REFAL", title: "REFAL Studio Wiki"},
            {url: "https://wiki.swisschili.sh/wiki/REFAL/Cookbook", title: "REFAL Cookbook"},
            {url: "http://refal.botik.ru/book/html/", title: "REFAL-5 Programming Guide (en)"},
            {url: "http://refal.net/rf5_frm.htm", title: "REFAL-5 Programming Guide (ru)"}
        ]

        delegate: Tip {
            Layout.fillWidth: true

            url: modelData.url
            title: modelData.title

            ToolTip.text: modelData.url
            ToolTip.visible: containsMouse
            ToolTip.delay: 1000
        }
    }
}
