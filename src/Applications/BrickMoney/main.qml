import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import LegoSetIOManager 0.1
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

    //    Material.theme: Material.Dark
    //    Material.accent: Material.Purple

    Rectangle {
        id: buttonBarManageSetList
        width: parent.width -10
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        // color: "#00000000"
        radius: 8
        border.width: 0

        Button{
            Material.accent: Material.Orange
            id: newSetListButton
            text: "New"
            transformOrigin: Item.Center
            rotation: 0
            flat: false
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                console.log(newSetListButton.text)
                newProjectFolderDialog.open()
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
                openProjectDialog.open()
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
            enabled: _LegoSetIOManager.isProjectReady
            onClicked: {
                console.log(saveSetListButton.text)
                _LegoSetIOManager.saveProject()
            }
        }

        Rectangle {
            id: legoSetProjectNameRect
            width: 195
            anchors.left: saveSetListButton.right
            anchors.top: buttonBarManageSetList.top
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            height: 40
            color: "#ffffff"
            // color: "#00000000"
            radius: 8
            border.width: 0

            Text {
                id: projectNameLabel
                text: qsTr("Project:")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
                font.pixelSize: 12
            }

            Text {
                id: projectNameValue
                text: _LegoSetIOManager.ProjectFolder
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: projectNameLabel.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                font.pixelSize: 12
            }
        }



    }

    MessageDialog {
        id: errorMessageDialog
        title: "Error with brickMoney folder"
        text: "The brickMoney project folder is not ready."
        onVisibleChanged: overlayRect.visible = visible
    }

    Rectangle {
        id: overlayRect
        width: mainWindow.width;
        height: mainWindow.height;
        color: "#0f0000"
        z: 5
        opacity: 0.75
        visible: false
    }

    FolderDialog {
        id: newProjectFolderDialog
        title: "Please choose or create a folder for the BrickMoney project"
        onAccepted: {
            console.log("You chose: " + newProjectFolderDialog.folder)
            _LegoSetIOManager.ProjectFolder = newProjectFolderDialog.folder
            if (!_LegoSetIOManager.isProjectReady)
            {
                errorMessageDialog.open()
                return;
            }
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    FolderDialog {
        id: openProjectDialog
        title: "Please select BrickMoney project folder"
        onAccepted: {
            console.log("You chose: " + openProjectDialog.folder)
            _LegoSetIOManager.loadProject(openProjectDialog.folder)
            if (!_LegoSetIOManager.isProjectReady)
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
        id: buttonBarChangeSetList
        width: parent.width -10
        anchors.top: buttonBarManageSetList.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        // color: "#00000000"
        radius: 8
        border.width: 0

        Button{
            id: addButton
            text: "Add"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            enabled: _LegoSetIOManager.isProjectReady
            onClicked: {
                //console.log("Add")
                _LegoSetTableModel.newEntry()
                legoTable.positionViewAtRow(_LegoSetTableModel.rowCount() -1)
                legoTable.selection.clear()
                legoTable.selection.select(_LegoSetTableModel.rowCount() -1)
            }
        }

        Button{
            id: deleteButton
            text: "Delete"
            highlighted: true
            flat: false
            enabled: legoTable.selection.count > 0 ? true: false
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

                legoTable.selection.forEach( function(rowIndex) {
                    //console.log(rowIndex)
                    _LegoSetTableModel.deleteEntry(rowIndex)
                    deletedRowIndex = rowIndex
                } )
                legoTable.selection.clear()

                if ( _LegoSetTableModel.rowCount() === 0)
                {
                    return
                }

                //console.log("deletedRowIndex: " +deletedRowIndex)

                if (deletedRowIndex <  _LegoSetTableModel.rowCount() )
                {
                    legoTable.selection.select(deletedRowIndex)
                    return
                }

                if (deletedRowIndex-1 >= 0 )
                {
                    legoTable.selection.select(deletedRowIndex-1)
                }

            }
        }

        Button{
            id: clearSelectButton
            text: "Clear Selection"
            highlighted: true
            flat: false
            enabled: legoTable.selection.count
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: deleteButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: legoTable.selection.clear()
        }

        Slider {
            Text {
                id: zoomText
                text: qsTr("Zoom")
            }
            id: zoomSlider
            wheelEnabled: false
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.left: clearSelectButton.right
            from: 5
            value: 50
            to: 300
        }

    }

    LegoTable {
        id: legoTable
    }

}



