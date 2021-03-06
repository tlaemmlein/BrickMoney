import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.12
import "helper/"


ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 1600
    height: 600
    title: qsTr("BrickMoney Vers. 0.2 - The software for LEGO Investment")

    Component.onCompleted: {
        //console.log(objectName + ":onCompleted")
        if (BrickMoneySettings.mainIsMaximized){
            showMaximized()
        }
        else {
            var rect = BrickMoneySettings.mainWindow
            mainWindow.x = rect.x
            mainWindow.y = rect.y
            mainWindow.width = rect.width
            mainWindow.height = rect.height
        }
    }

    Component {
        id: importLegoSetsComp
        ImportDialog {
            id: importLegoSetsDialog
            onAccepted: importLegoSetsDialog.destroy()
            onRejected: importLegoSetsDialog.destroy()
        }
    }


    menuBar: MenuBar{
        Menu {
            title:qsTr("&File")
            Action {
                id : loadActionId
                text: qsTr("&Load project")
                icon.source: "qrc:/images/load.svg"
                onTriggered: {
                    //console.log("Clicked on load")
                    loadFileDialog.open()
                }
            }
            MenuSeparator {}
            Action {
                id : saveActionId
                text: qsTr("&Save project")
                icon.source: "qrc:/images/save.svg"
                enabled: BrickMoneyDataManager.brickMoneyIsDirty
                onTriggered: {
                    //console.log("Clicked on save")
                    BrickMoneyProject.save()
                }
            }
            Action {
                id : saveAsActionId
                text: qsTr("&Save project as...")
                onTriggered: {
                    //console.log("Clicked on save as")
                    saveAsFileDialog.open()
                }
            }
        }
        Menu {
            title: qsTr("Data")
            Action {
                id: importDialogID
                text: qsTr("Import...")
                onTriggered: { var dg = importLegoSetsComp.createObject(mainWindow); dg.open() }
            }
        }

        Menu {
           title:qsTr("&Language")
           Action {
               id : germanLangActionId
               text: qsTr("German")
               onTriggered: BrickMoneyTrans.selectLanguage("german")
           }
           Action {
               id : englishLangActionId
               text: qsTr("English")
               onTriggered: BrickMoneyTrans.selectLanguage("english")
           }

        }

    }

    BrickMoneyFileDialog {
        id: loadFileDialog
        objectName: "loadFileDialog"
        title: qsTr("Load BrickMoney project...")
        nameFilters: ["JSON files (*.json)"]
        fileMode: BrickMoneyFileDialog.OpenFile
        folder: BrickMoneySettings.brickMoneyFilePath
        onAccepted: {
            //console.log(objectName + ": onAccepted: You choose: " + currentFile)

            if ( !BrickMoneyProject.checkBrickMoneyProject(currentFile ) )
            {
                // TODO Error saveAsFileDialog
                console.error(objectName + ": onAccepted: Unable to load BrickMoney project from " + currentFile + ".")
                return
            }

            BrickMoneySettings.brickMoneyFilePath = currentFile
            BrickMoneyProject.load()
        }
        onRejected: {
            //console.log(objectName + ":Canceled")
        }
    }

    BrickMoneyFileDialog {
        id: saveAsFileDialog
        objectName: "saveFileDialog"
        title: qsTr("Save BrickMoney project as...")
        nameFilters: ["JSON files (*.json)"]
        fileMode: BrickMoneyFileDialog.SaveFile
        folder: BrickMoneySettings.brickMoneyFilePath
        onAccepted: {
            //console.log(objectName + ": onAccepted: You choose: " + currentFile)
            BrickMoneySettings.brickMoneyFilePath = currentFile
            BrickMoneyProject.save()
        }
        onRejected: {
            //console.log(objectName + ":Canceled")
        }
    }

    MessageDialog {
        id: messageDialogQuit
        text: qsTr("The document has been modified.")
        informativeText: qsTr("Do you want to save your changes?")
        standardButtons: StandardButton.Yes | StandardButton.No | StandardButton.Cancel
        onYes: {
            BrickMoneyProject.save()
            Qt.quit()
        }
        onNo: Qt.quit()
    }

    onClosing:{
        close.accepted = false
        onTriggered:
        {
            if (BrickMoneyDataManager.brickMoneyIsDirty === true) {
                messageDialogQuit.open()
            } else {
                BrickMoneySettings.mainWindow = Qt.rect(mainWindow.x, mainWindow.y, mainWindow.width, mainWindow.height)
                BrickMoneySettings.mainIsMaximized = (Window.Maximized === mainWindow.visibility)
                Qt.quit()
            }
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
            ToolButton { action: germanLangActionId}
            ToolButton { action: englishLangActionId}
        }
    }

    footer: TabBar {
        id : mTabBar
        width: parent.width

        currentIndex: mSwipeId.currentIndex

        TabButton {
            text : qsTr("Overview")
            onClicked: {
                mSwipeId.currentIndex = 0
                //console.log("mSwipeId.currentIndex = 0")
            }
        }
        TabButton {
            text : qsTr("In Stock") + " (" + InStockLegoSetTableModel.NumberOfLegoSets + ")"
            onClicked: {
                mSwipeId.currentIndex = 1
                //console.log("mSwipeId.currentIndex = 1")
            }
        }
        TabButton {
            text : qsTr("For Sale") + " (" + ForSaleLegoSetTableModel.NumberOfLegoSets + ")"
            onClicked: {
                mSwipeId.currentIndex = 2
                //console.log("mSwipeId.currentIndex = 2")
            }
        }
        TabButton {
            text : qsTr("Sold") + " (" + SoldLegoSetTableModel.NumberOfLegoSets + ")"
            onClicked: {
                mSwipeId.currentIndex = 3
                //console.log("mSwipeId.currentIndex = 3")
            }
        }
    }

    SwipeView {
        id : mSwipeId
        Component.onCompleted: contentItem.interactive = false
        anchors.fill: parent
        currentIndex: 0

        Overview{
            id: overviewId
        }
        InStock {
            id: inStockId
        }
        ForSale {
            id: activeId
        }
        Sold {
            id: soldId
        }
    }
}



