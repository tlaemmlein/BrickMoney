import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import de.brickmoney.models 0.1
import "helper/"

Rectangle {
    id: inStock
    objectName: "inStock"

    Component.onCompleted: {
        //console.log(objectName + ":onCompleted")
    }
    Component.onDestruction:  {
        //console.log(objectName + ":onDestruction")
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
                //console.log(objectName + ":onAddLegoSetNumber")
                var legoSet = InStockLegoSetTableModel.addLegoSet(setNumber)
                legoSet.purchaseDate = purchaseDate
                legoSet.purchasingPrice = purchasingPrice
                legoSet.seller = seller
                //console.log(objectName + "seller: " + seller)
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
              font.pointSize: 14
              height: 0.8 * buttonBarChangeSetList.height
			  verticalAlignment : Text.AlignVCenter
			}

            Rectangle {
                border.color: buttonBarChangeSetList.color
                color: buttonBarChangeSetList.color
                width: 15
				height: addButton.height
            }

            BrickMoneyRoundButton {
                id: addButton
                text: qsTr("Add")
                pressedColor: "lightgrey"
                releasedColor: "lightgreen"
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: { var dg = newLegoSetComp.createObject(this); dg.openDialog() }
            }

            BrickMoneyRoundButton {
                id: deleteLegoSets
                enabled: InStockLegoSetTableModel.SelectionIsDirty
                text: qsTr("Delete")
                pressedColor: "lightgrey"
                releasedColor: "red"
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: {
                    var dg = confirmDeleteDialogComp.createObject(this)
                    dg.open()
                    dg.setActiveFocusToYesButton()
                }

                Component {
                    id: confirmDeleteDialogComp
                    DialogToConfirmTheDeletion{
                        id: confirmDeleteDialog
                        width: 450
                        dialogTitle: qsTr("Do you want to delete the LegoSet(s)?")
                        contentText: "ID(s): " + InStockLegoSetTableModel.getSelectedLegoSetIDs()
                        onAccepted: {InStockLegoSetTableModel.removeSelectedLegoSets(); confirmDeleteDialog.destroy(); }
                        onRejected: confirmDeleteDialog.destroy()
                    }
                }
            }

            BrickMoneyRoundButton {
                id: forceLayoutBt
                text: qsTr("Force Layout")
                pressedColor: "lightgrey"
                releasedColor: "lightblue"
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: legoTable.forceLayout()
            }

            BrickMoneyRoundButton {
                id: resizeCols
                text: qsTr("Resize Cols")
                pressedColor: "lightgrey"
                releasedColor: "lightblue"
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: legoTable.resizeCols()
            }

            Text {
              text: qsTr("Filter:")
              font.pointSize: 14
              height: 0.8 * buttonBarChangeSetList.height
              verticalAlignment : Text.AlignVCenter
            }

            TextField {
                id: tfFilter
                anchors.verticalCenter: parent.verticalCenter
                height: 0.8 * buttonBarChangeSetList.height
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
    }
}
