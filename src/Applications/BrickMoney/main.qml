import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import LegoSetTableModel 0.1
import LegoSetIOManager 0.1
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls.Material 2.1
import Qt.labs.platform 1.1

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

//    Material.theme: Material.Dark
//    Material.accent: Material.Purple

    MessageDialog {
        id: errorMessageDialog
        title: "Error with brickMoney folder"
        text: "The brickMoney project folder is not empty."
        onVisibleChanged: overlayRect.visible = visible
    }

    Rectangle {
        id: overlayRect
        width: window.width;
        height: window.height;
        color: "#0f0000"
        z: 5
        opacity: 0.75
        visible: false
    }

    FolderDialog {
        id: folderDialog
        title: "Please choose or create an empty brickMoney project folder"
        onAccepted: {
                console.log("You chose: " + folderDialog.folder)
                    if (!_LegoSetIOManager.isProjectFolderReady(folderDialog.folder))
                    {
                        errorMessageDialog.open()
                        return;
                    }
                }
            onRejected: {
                console.log("Canceled")
            }
    }


    Rectangle {
        id: buttonBarManageSetList
        width: parent.width -10
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        // color: "#00000000"
        radius: 8
        border.width: 2

        Button{
            Material.accent: Material.Orange
            id: newSetListButton
            text: "New"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                console.log(newSetListButton.text)
                folderDialog.open()
            }
        }

        Button{
            id: openSetListButton
            text: "Open"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: newSetListButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                console.log(openSetListButton.text)
            }
        }

        Button{
            id: saveSetListButton
            text: "Save"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: openSetListButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                console.log(saveSetListButton.text)
            }
        }

        Button{
            id: saveAsSetListButton
            text: "Save as"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: saveSetListButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                console.log(saveAsSetListButton.text)
            }
        }
    }

    Rectangle {
        id: buttonBarChangeSetList
        width: parent.width -10
        anchors.top: buttonBarManageSetList.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        // color: "#00000000"
        radius: 8
        border.width: 2

        Button{
            id: addButton
            text: "Add"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                //console.log("Add")
                _LegoSetTableModel.newEntry()
                tableView.positionViewAtRow(_LegoSetTableModel.rowCount() -1)
                tableView.selection.clear()
                tableView.selection.select(_LegoSetTableModel.rowCount() -1)
            }
        }

        Button{
            id: deleteButton
            text: "Delete"
            highlighted: true
            flat: false
            enabled: tableView.selection.count > 0 ? true: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                //console.log("Delete")
                if ( _LegoSetTableModel.rowCount() === 0)
                {
                   return
                }

                var deletedRowIndex = 0

                tableView.selection.forEach( function(rowIndex) {
                    //console.log(rowIndex)
                    _LegoSetTableModel.deleteEntry(rowIndex)
                    deletedRowIndex = rowIndex
                } )
                tableView.selection.clear()

                if ( _LegoSetTableModel.rowCount() === 0)
                {
                   return
                }

                //console.log("deletedRowIndex: " +deletedRowIndex)

                if (deletedRowIndex <  _LegoSetTableModel.rowCount() )
                {
                    tableView.selection.select(deletedRowIndex)
                    return
                }

                if (deletedRowIndex-1 >= 0 )
                {
                    tableView.selection.select(deletedRowIndex-1)
                }

            }
        }

        Button{
            id: clearSelectButton
            text: "Clear Selection"
            highlighted: true
            flat: false
            enabled: tableView.selection.count
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: deleteButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: tableView.selection.clear()
        }

        Slider {
            Text {
                id: zoomText
                text: qsTr("Zoom")
            }
            id: zoomSlider
            anchors.left: clearSelectButton.right
            from: 5
            value: 50
            to: 300
        }

    }


    C1.TableView {
        id: tableView
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: buttonBarChangeSetList.bottom
        anchors.topMargin: 5
        width: parent.width -5
        height: parent.height - buttonBarChangeSetList.height - 15
        clip: true

        model: _LegoSetTableModel

        headerDelegate: Text {
            text: styleData.value
            minimumPixelSize: 12
            font.pixelSize: 14
        }

        rowDelegate: Rectangle {
            height: zoomSlider.value
            SystemPalette {
                id: myPalette;
                colorGroup: SystemPalette.Active
            }
            color: {
//                var baseColor = styleData.alternate ? myPalette.alternateBase : myPalette.base
                var baseColor = styleData.alternate ? "#FFFFFF" : "#f5f5f5"
                styleData.selected ? "#03A9F4" : baseColor
                //return styleData.selected ? myPalette.highlight : baseColor
            }

        }

        C1.TableViewColumn {
            id: imageColumn
            role: "image"
            title: "Bild"
            width: 100
            delegate: Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: styleData.value
            }
        }

        C1.TableViewColumn {
            id: setNumColumn
            role: "setnumber"
            title: "Set Nummer"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                validator: IntValidator {bottom: 0; top: 2147483647;}
                selectByMouse: true
                text: styleData.value
                onEditingFinished: {
                    var roleID = _LegoSetTableModel.roleID(setNumColumn.role)
                    var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                    var data_changed = _LegoSetTableModel.setData(q_model_index, text, roleID)
                }
            }

        }

        C1.TableViewColumn {
            id: descColumn
            role: "description"
            title: "Bezeichnung"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: styleData.value
                selectByMouse: true
                onEditingFinished: {
                    var roleID = _LegoSetTableModel.roleID(descColumn.role)
                    var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                    var data_changed = _LegoSetTableModel.setData(q_model_index, text, roleID)
                }
            }
        }
    }

}
