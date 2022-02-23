import QtQuick 2.5
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import sh.swisschili.REFAL 1.0

ApplicationWindow {
    id: root
    width: 1080
    height: 720
    title: "Notebook"
    visible: true

    Material.theme: Material.Light
    Material.accent: Material.Orange

    CellModel {
        id: model
    }

    Component.onCompleted: {
        model.addCell("Refal { = Hi!; }", "");
        model.addCell("<Refal>", "Hi!");
        model.addCell("Hello there", "Hello there");
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

        TabBar {
            id: bar

            Layout.fillWidth: true

            TabButton {
                text: "Example Workspace"
            }

            TabButton {
                text: "Another Workspace"
            }

            TabButton {
                text: "Testing"
            }
        }

        SplitView {
            id: split
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal

            ListView {
                id: codeEditor
                SplitView.fillWidth: true
                SplitView.minimumWidth: 400
                model: model
                clip: true

                delegate: NotebookCell {
                    width: codeEditor.width - 5
                }
            }

            Item {
                id: variables
                SplitView.minimumWidth: 240

                Label {
                    anchors.centerIn: parent
                    text: "Vars"
                }
            }
        }
    }
}