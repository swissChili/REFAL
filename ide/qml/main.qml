import QtQuick 2.5
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import sh.swisschili.REFAL 1.0

ApplicationWindow {
    id: root
    width: 1080
    height: 720
    title: "Refal Notebook -- " + notebook.savePath
    visible: true

    Material.theme: Material.Light
    Material.accent: Material.Orange

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            Action {
                text: "&Save"
                shortcut: "Ctrl+s"

                onTriggered: {
                    notebook.save()
                }
            }
        }

        Menu {
            title: qsTr("&Runtime")

            Action {
                text: qsTr("Run &Selected Cell")
                shortcut: "Ctrl+Return"

                onTriggered: {
                    if (codeEditor.currentItem !== null) {
                        notebook.runCell(codeEditor.currentItem.uuid)
                    }
                }
            }
        }
    }

    Notebook {
        id: notebook

        onSaveError: (message) =>
        {
            console.error(message)
        }
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

        TabBar {
            id: bar

            Layout.fillWidth: true

            TabButton {
                text: "Notebook"
                width: implicitWidth
            }

            TabButton {
                text: "Another Notebook"
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

                spacing: 5

                header: ColumnLayout {
                    width: codeEditor.width

                    Pane {
                        Layout.bottomMargin: 0

                        ColumnLayout {
                            Label {
                                font.pointSize: 18
                                text: "Notebook"
                            }

                            Label {
                                visible: codeEditor.count === 0

                                text: "Looks like you haven't created any cells yet. Click the + button below to create one."
                            }
                        }
                    }

                    InsertRow {
                        onInsertClicked: notebook.cellModel.insertCellBefore(0)
                    }

                    Item {
                        height: 5 // JANK!
                    }
                }

                delegate: NotebookCell {
                    id: notebookCell

                    required property var model
                    required property var index
                    required property var uuid

                    width: codeEditor.width

                    code: model.code
                    result: model.result.trim()
                    status: model.status
                    resultType: model.resultType
                    cellActive: codeEditor.currentIndex === index

                    onCodeEditingFinished: code => model.code = code

                    onInsertBelowClicked: {
                        console.info(index);
                        notebook.cellModel.insertCellBefore(index + 1);
                    }

                    onRunClicked: {
                        console.info("Cell run clicked")
                        notebook.runCell(uuid)
                    }

                    onCellFocused: {
                        codeEditor.currentIndex = index
                    }

                    onDeleteClicked: {
                        notebook.cellModel.deleteCellAt(index)
                    }

                    onCellUnfocused: {
                        codeEditor.currentIndex = -1
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
