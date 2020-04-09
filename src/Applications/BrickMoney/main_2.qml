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
    height: 600
    title: qsTr("BrickMoney - Die Software für LEGO Investment")
    color: "lightblue"

    Rectangle {
        id: buttonBarChangeSetList
        width: addButton.width + zoomSlider.width  +10 // parent.width -10
        y : 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        // color: "#00000000"
        //radius: 8
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
                input.openDialog()
            }
            NewLegoSetDialog{
                id : input
                Component {
                  id: legoSetComp
                  LegoSet {}
                }
                onAddLegoSetNumber:{
                    console.log( "onAddLegoSetNumber : " + setNumber)
                    var ds = mLegoSetModel.dataSource
                    var legoSet = legoSetComp.createObject()
                    legoSet.setNumber = setNumber
                    legoSet.purchaseDate = purchaseDate
                    legoSet.purchasingPrice = purchasingPrice
                    legoSet.seller = seller
                    ds.addLegoSet(legoSet)
                }
            }
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
            anchors.left: addButton.right
            from: 1
            value: 20
            to: 100
        }

    }


     LegoSetTableModel {
        id: mLegoSetModel

        LegoSet {
            setNumber: 41599
            purchasingPrice: 7.71
            seller: "Karstadt München"
            purchaseDate: new Date('2019-04-15') //"15.04.2019"
            retailPrice: 1000.95
            saleDate: new Date('2019-08-11') // "11.08.2019"
            soldOver: "www.ebay.de"
            buyer: "Anja Heim Erlenstr. 3 85435 Erding"
        }

        LegoSet {
            setNumber: 70608
            purchasingPrice: 8.99
            seller: "Karstadt München"
            //purchaseDate: new Date('2019-04-17') //"15.04.2019"
            //retailPrice: 0
            //buyer: "fff"
        }

        LegoSet {
            setNumber: 75212
            purchasingPrice: 116.47
            seller: "www.steinehelden.de"
            purchaseDate: new Date('2018-07-07')
        }

        LegoSet {setNumber: 75208; purchasingPrice: 21.89; seller: "www.toysrus.de"; purchaseDate: new Date('2018-07-11') }

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



