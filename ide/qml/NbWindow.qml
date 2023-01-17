import QtQuick 2.5
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import sh.swisschili.REFAL 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 800
    title: "Refal Notebook — " + notebook.savePath
    visible: true

    Material.theme: Material.Light
    Material.accent: Material.Orange

    minimumWidth: 600
    minimumHeight: 400

    required property ApplicationWindow welcomeWindow

    function openNotebook(path) {
        notebook.open(path);
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            Action {
                text: qsTr("&New")

                onTriggered: {
                    welcomeWindow.openNotebook();
                }
            }

            Action {
                text: qsTr("&Save")
                shortcut: "Ctrl+s"

                onTriggered: {
                    notebook.save()
                }
            }

            Action {
                text: qsTr("&Open")
                shortcut: "Ctrl+o"

                onTriggered: {
                    notebook.open();
                }
            }
        }

        Menu {
            title: qsTr("&View")

            Action {
                text: qsTr("&Welcome Window")
                checkable: true

                checked: welcomeWindow.visible

                onTriggered: {
                    welcomeWindow.toggleVisible()
                }
            }

            Action {
                id: varInspector
                text: qsTr("&Variable Inspector")

                checkable: true
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

            Action {
                text: qsTr("Run &All")

                onTriggered: notebook.runAll();
            }

            Action {
                text: qsTr("&Reset Runtime State")

                onTriggered: notebook.reset();
            }
        }
    }

    Notebook {
        id: notebook

        onSaveError: (message) =>
        {
            console.error(message)
        }

        onSaved: welcomeWindow.recentModel.add(notebook.savePath)
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

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
                                text: notebook.savePath != "" ? notebook.savePath.split("/").pop().split(".").slice(0,-1).join('.') : qsTr("Notebook")
                            }

                            Label {
                                visible: codeEditor.count === 0

                                text: qsTr("Looks like you haven't created any cells yet. Click the + button below to create one.")
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
                        notebook.runCell(uuid)
                    }

                    onQuitClicked: {
                        notebook.quitCell(uuid)
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

                visible: varInspector.checked

                Label {
                    anchors.centerIn: parent
                    text: qsTr("Variables")
                }
            }
        }
    }
}
