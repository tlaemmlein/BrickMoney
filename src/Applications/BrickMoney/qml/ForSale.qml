import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import de.brickmoney.styles 1.0
import "helper/"

Rectangle {
    id: forSale
    objectName: "forSale"
    color: BrickMoneyStyle.forSaleColor

    Component.onCompleted: {
        //console.log(objectName + ":onCompleted")
    }
    Component.onDestruction:  {
        //console.log(objectName + ":onDestruction")
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
              text: qsTr("For Sale")
              font.pointSize: 14
              height: 0.8 * buttonBarChangeSetList.height
              verticalAlignment : Text.AlignVCenter
            }

            Rectangle {
                border.color: buttonBarChangeSetList.color
                color: buttonBarChangeSetList.color
                width: 15
                height: deleteLegoSets.height
            }

            BrickMoneyRoundButton {
                id: deleteLegoSets
                visible: ForSaleLegoSetTableModel.SelectionIsDirty
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
                        contentText: "ID(s): " + ForSaleLegoSetTableModel.getSelectedLegoSetIDs()
                        onAccepted: {ForSaleLegoSetTableModel.removeSelectedLegoSets(); confirmDeleteDialog.destroy(); }
                        onRejected: confirmDeleteDialog.destroy()
                    }
                }
            }

            BrickMoneyRoundButton {
                id: moveToInStock
                visible: ForSaleLegoSetTableModel.SelectionIsDirty
                text: qsTr("Move to In Stock")
                pressedColor: "lightgrey"
                releasedColor: BrickMoneyStyle.inStockColor
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: {
                    BrickMoneyProject.moveSelectedLegoSets(ForSaleLegoSetTableModel, InStockLegoSetTableModel)
                }
            }

            BrickMoneyRoundButton {
                id: moveToSold
                visible: ForSaleLegoSetTableModel.SelectionIsDirty
                text: qsTr("Move to Sold")
                pressedColor: "lightgrey"
                releasedColor: BrickMoneyStyle.soldColor
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: {
                    BrickMoneyProject.moveSelectedLegoSets(ForSaleLegoSetTableModel, SoldLegoSetTableModel)
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
                 ForSaleLegoSetTableSortModel.filterText = tfFilter.text
                 return ForSaleLegoSetTableSortModel
        }
    }

}

