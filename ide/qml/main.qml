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

    Notebook {
        id: notebook
    }

    Component.onCompleted: {
        notebook.cellModel.addCell("Refal { = Hi!; }", "");
        notebook.cellModel.addCell("<Refal>", "Hi!");
        notebook.cellModel.addCell("Hello there", "Hello there");
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

        TabBar {
            id: bar

            Layout.fillWidth: true

            TabButton {
                text: "Example Workspace"
                width: implicitWidth
            }

            TabButton {
                text: "Another Workspace"
                width: implicitWidth
            }

            TabButton {
                text: "Testing"
                width: implicitWidth
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
                model: notebook.cellModel
                clip: true

                header: ColumnLayout {
                    width: codeEditor.width

                    DocumentPadding {
                        Layout.bottomMargin: 0

                        Label {
                            font.pointSize: 18
                            text: "Notebook"
                        }
                    }

                    InsertRow {
                        onInsertClicked: notebook.cellModel.insertRows(notebook.cellModel.index(0, 0), 1);
                    }
                }

                delegate: NotebookCell {
                    id: notebookCell

                    required property var model
                    required property var index
                    required property var uuid
                    required property int status

                    width: codeEditor.width - 5

                    code: model.code
                    result: model.result.trim()
                    status: model.status

                    onCodeEditingFinished: model.code = code

                    onInsertBelowClicked: {
                        console.info(index);
                        notebook.cellModel.insertRows(notebook.cellModel.index(index + 1, 0), 1);
                    }

                    onRunClicked: {
                        console.info("Cell run clicked")
                        notebook.runCell(uuid)
                    }
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
