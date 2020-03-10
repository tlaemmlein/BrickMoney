import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1
import de.brickmoney.models 0.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1600
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

    Rectangle {
        id: buttonBarChangeSetList
        width: parent.width -10
        //anchors.top: buttonBarManageSetList.bottom
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
            //enabled: _LegoSetIOManager.isProjectReady
            onClicked: legoTable.addLegoSet()
        }

        Button{
            id: deleteButton
            text: "Delete"
            highlighted: true
            flat: false
            enabled: legoTable.isSelected()
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: legoTable.removeLegoSet()
        }


        Button{
            id: clearSelectButton
            text: "Clear Selection"
            highlighted: true
            flat: false
            enabled: legoTable.isSelected()
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: deleteButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: legoTable.clearSelection()
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
            from: 1
            value: 20
            to: 100
        }

    }


     LegoSetTableModel {
        id: mLegoSetModel

        LegoSet {
            imageUrl: "qrc:/images/WonderWoman.png"
            setNumber: 41599
            description: "LEGO® BrickHeadz 41599 Wonder Woman 1"
            year: 2018
            recommendedRetailPrice: 9.99
            purchasingPrice: 7.71
            seller: "Karstadt München"
            purchaseDate: new Date('2019-04-15') //"15.04.2019"
            retailPrice: 1000.95
            saleDate: new Date('2019-08-11') // "11.08.2019"
            soldOver: "www.ebay.de"
            buyer: "Anja Heim Erlenstr. 3 85435 Erding"
        }

        LegoSet {
            imageUrl: "qrc:/images/WonderWoman.png"
            setNumber: 41599
            description: "LEGO® BrickHeadz 41599 Wonder Woman 2 And then there was a famous person which wants all the stuff of them all"
            year: 2018
            recommendedRetailPrice: 9.99
            purchasingPrice: 8.99
            seller: "Karstadt München"
            //purchaseDate: new Date('2019-04-17') //"15.04.2019"
            //retailPrice: 0
            //buyer: "fff"
        }
    }
     LegoList{
         id: legoTable
         anchors.left: parent.left
         anchors.leftMargin: 5
         anchors.top: buttonBarChangeSetList.bottom
         anchors.topMargin: 5
         anchors.right: parent.right
         anchors.rightMargin: 5
//         height: parent.height -15
         height: parent.height - buttonBarChangeSetList.height -15
                 // - buttonBarManageSetList.height -15

         model: mLegoSetModel
         rowHeight: zoomSlider.value
     }

//     LegoTable {
//         id: legoTable
//         anchors.left: parent.left
//         anchors.leftMargin: 5
//         anchors.top: buttonBarChangeSetList.bottom
//         anchors.topMargin: 5
//         anchors.right: parent.right
//         anchors.rightMargin: 5
////         height: parent.height -15
//         height: parent.height - buttonBarChangeSetList.height -15
//                 // - buttonBarManageSetList.height -15

//         model: mLegoSetModel
//         rowHeight: zoomSlider.value
//     }

}



