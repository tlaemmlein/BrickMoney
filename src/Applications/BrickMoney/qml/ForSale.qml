import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "helper/"

Rectangle {
    id: forSale
    objectName: "forSale"
    color: "#494FBF"

    Component.onCompleted: {
        console.log(objectName + ":onCompleted")
    }
    Component.onDestruction:  {
        console.log(objectName + ":onDestruction")
    }

    Rectangle {
        id: buttonBarChangeSetList
        width: childrenRect.width + 20
        y : 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        border.width: 1
        border.color: "black"

        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            spacing: 5

            Slider {
                Text {
                    id: zoomText
                    text: zoomSlider.value +"%"
                }
                id: zoomSlider
                objectName: "zoomSlider"
                Component.onDestruction: BrickMoneySettings.zoomFactor = value

                wheelEnabled: true
                anchors.verticalCenter: parent.verticalCenter
                stepSize: 10
                from: 10
                value: BrickMoneySettings.zoomFactor
                to: 300
            }

            RadioButton {
                anchors.verticalCenter: parent.verticalCenter
                checked: BrickMoneySettings.viewSettings === LegoList.ViewSettings.Details
                text: qsTr("Details")
                onCheckedChanged: {
                    if (checked){
                        legoList.viewSettings = LegoList.ViewSettings.Details
                        BrickMoneySettings.viewSettings = LegoList.ViewSettings.Details
                    }
                }
            }

            RadioButton {
                anchors.verticalCenter: parent.verticalCenter
                checked: BrickMoneySettings.viewSettings === LegoList.ViewSettings.Compact
                text: qsTr("Compact")
                onCheckedChanged: {
                    if (checked){
                        legoList.viewSettings = LegoList.ViewSettings.Compact
                        BrickMoneySettings.viewSettings = LegoList.ViewSettings.Compact
                    }
                }
            }
        }
    }

//    LegoList{
//        id: legoList
//        anchors.left: parent.left
//        anchors.leftMargin: 5
//        anchors.top: buttonBarChangeSetList.bottom
//        anchors.topMargin: 5
//        anchors.right: parent.right
//        anchors.rightMargin: 5
//        height: parent.height - buttonBarChangeSetList.height -15

//        model: ForSaleLegoSetTableModel
//        zoom: zoomSlider.value
//    }
}

