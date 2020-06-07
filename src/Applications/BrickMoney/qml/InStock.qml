import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import de.brickmoney.models 0.1
import "helper/"

Rectangle {
    id: inStock
    objectName: "inStock"
    //color: "#0511F2"

    Component.onCompleted: {
        console.log(objectName + ":onCompleted")
    }
    Component.onDestruction:  {
        console.log(objectName + ":onDestruction")
		if ( newLegoSetDialog != null) {
			newLegoSetDialog.destroy()
		}
    }

    Component {
        id: newLegoSetComp
        NewLegoSetDialog{
            id : newLegoSetDialog
            objectName: "newLegoSetDialog"
            onAddLegoSetNumber:{
                console.log(objectName + ":onAddLegoSetNumber")
                var legoSet = InStockLegoSetTableModel.addLegoSet(setNumber)
                legoSet.purchaseDate = purchaseDate
                legoSet.purchasingPrice = purchasingPrice
                legoSet.seller = seller
                console.log(objectName + "seller: " + seller)
            }
        }

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
			Text {
			  text: qsTr("In Stock")
			  font.pointSize: 24
			  verticalAlignment : Text.AlignVCenter
			}

            Rectangle {
                border.color: buttonBarChangeSetList.color
                color: buttonBarChangeSetList.color
                width: 15
				height: addButton.height
            }

            RoundButton{
                id: addButton
                text: qsTr("Add")
                highlighted: true
                anchors.verticalCenter: parent.verticalCenter
                height: 40
                onClicked: {
                    var dg = newLegoSetComp.createObject(this)
                    dg.openDialog()
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
                        //legoTable.viewSettings = LegoList.ViewSettings.Details
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
                        legoTable.viewSettings = LegoList.ViewSettings.Compact
                        BrickMoneySettings.viewSettings = LegoList.ViewSettings.Compact
                    }
                }
            }

            Button {
                id: resizeCols
                text: qsTr("Resize cols")
                onClicked: legoTable.resizeCols()
            }

            TextField {
                id: tfFilter
                implicitWidth: parent.width / 4
                onTextEdited: legoTable.contentY = 0
            }
        }
    }

    LegoTable{
        id: legoTable
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: buttonBarChangeSetList.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        height: parent.height - buttonBarChangeSetList.height -15

        model: {
                 InStockLegoSetTableSortModel.filterText = tfFilter.text
                 return InStockLegoSetTableSortModel
        }
        //zoom: zoomSlider.value
    }
}
