import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.12
import de.brickmoney.models 0.1
import Qt.labs.platform 1.1 as QP

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 1600
    height: 600
    title: qsTr("BrickMoney Vers. 0.1 - The software for LEGO Investment")
    color: "lightblue"

    Component.onCompleted: {
        console.log(objectName + ":onCompleted")
    }

    menuBar: MenuBar{
        Menu {
            title:qsTr("&File")
            Action {
                id : loadActionId
                text: qsTr("&Load project")
                icon.source: "qrc:/images/load.svg"
                onTriggered: {
                    console.log("Clicked on load")
                    loadFileDialog.open()
                }
            }
            MenuSeparator {}
            Action {
                id : saveActionId
                text: qsTr("&Save project")
                icon.source: "qrc:/images/save.svg"
                enabled: BrickMoneySettings.brickMoneyIsDirty
                onTriggered: {
                    console.log("Clicked on save")
                    LegoSetTableModelGeneral.dataSource.saveLegoSets()
                }
            }
            Action {
                id : saveAsActionId
                text: qsTr("&Save project as...")
                onTriggered: {
                    console.log("Clicked on save as")
                    saveAsFileDialog.open()
                }
            }
        }
    }

    QP.FileDialog {
        id: loadFileDialog
        objectName: "loadFileDialog"
        title: qsTr("Load BrickMoney project...")
        nameFilters: ["CSV files (*.csv)"]
        fileMode: QP.FileDialog.OpenFile
        folder: BrickMoneySettings.brickMoneyFilePath
        onAccepted: {
            console.log(objectName + ": onAccepted: You choose: " + currentFile)
            LegoSetTableModelGeneral.clearAll()
            var ds = LegoSetTableModelGeneral.dataSource
            ds.loadLegoSets(currentFile)
        }
        onRejected: {
            console.log(objectName + ":Canceled")
        }
    }

    QP.FileDialog {
        id: saveAsFileDialog
        objectName: "saveFileDialog"
        title: qsTr("Save BrickMoney project as...")
        nameFilters: ["CSV files (*.csv)"]
        fileMode: QP.FileDialog.SaveFile
        folder: BrickMoneySettings.brickMoneyFilePath
        onAccepted: {
            console.log(objectName + ": onAccepted: You choose: " + currentFile)
            var ds = LegoSetTableModelGeneral.dataSource
            ds.saveLegoSets(currentFile)
        }
        onRejected: {
            console.log(objectName + ":Canceled")
        }
    }

    header: ToolBar {
        Row {
            anchors.fill: parent
            spacing: 4
            ToolButton { action: loadActionId }
            ToolButton{ action: saveActionId }
            ToolButton{ id: saveAsTbId; action: saveAsActionId }

            Rectangle {
                border.color: "black"
                color: "black"
                width: 3
                anchors.top: saveAsTbId.top
                anchors.bottom: saveAsTbId.bottom
            }

            Text{
                id: brickMoneyFilePathText
                text: BrickMoneySettings.brickMoneyFilePath
                anchors.verticalCenter: parent.verticalCenter
                //verticalAlignment: Text.AlignVCenter
                font.pixelSize: 14
            }
        }
    }

    Rectangle {
        id: buttonBarChangeSetList
        width: addButton.width + zoomSlider.width  +10
        y : 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        border.width: 1
        border.color: "black"

        RoundButton{
            id: addButton
            text: "Add"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                newLegoSetDialog.openDialog()
            }
            NewLegoSetDialog{
                id : newLegoSetDialog
                objectName: "newLegoSetDialog"
                onAddLegoSetNumber:{
                    console.log(objectName + ":onAddLegoSetNumber")
                    var legoSet = LegoSetTableModelGeneral.addLegoSet(setNumber)
                    legoSet.purchaseDate = purchaseDate
                    legoSet.purchasingPrice = purchasingPrice
                    legoSet.seller = seller
                    console.log(objectName + "seller: " + seller)
                }
            }
        }

        Slider {
            Text {
                id: zoomText
                text: zoomSlider.value +"%"
            }
            id: zoomSlider
            objectName: "zoomSlider"
            Component.onDestruction: BrickMoneySettings.zoomFactor = value

            wheelEnabled: true
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.left: addButton.right
            stepSize: 10
            from: 10
            value: BrickMoneySettings.zoomFactor
            to: 300
        }

        RowLayout{
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.left: zoomSlider.right

            RadioButton {
                checked: true
                text: qsTr("Details")
                onClicked: legoList.viewSettings = LegoList.ViewSettings.Details
            }
            RadioButton {
                text: qsTr("Compact")
                onClicked: legoList.viewSettings = LegoList.ViewSettings.Compact
            }
        }

    }

    MessageDialog {
        id: messageDialogQuit
        text: qsTr("The document has been modified.")
        informativeText: qsTr("Do you want to save your changes?")
        standardButtons: StandardButton.Yes | StandardButton.No | StandardButton.Cancel
        onYes: {
            LegoSetTableModelGeneral.dataSource.saveLegoSets()
            Qt.quit()
        }
        onNo: Qt.quit()
    }

    onClosing:{
        close.accepted = false
        onTriggered:
        {
            if (BrickMoneySettings.brickMoneyIsDirty === true) {
                messageDialogQuit.open()
            } else {
                Qt.quit()
            }
        }
    }

    LegoList{
        id: legoList
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: buttonBarChangeSetList.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        height: parent.height - buttonBarChangeSetList.height -15

        model: LegoSetTableModelGeneral
        zoom: zoomSlider.value
    }
}



