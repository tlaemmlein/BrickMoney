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

            RoundButton{
                id: addButton
                text: qsTr("Add")
                background: Rectangle { id: addButtonRect; color: "lightgreen";  radius: addButton.radius; opacity: .8; border.width: 0;}
                onHoveredChanged: {
                    if (hovered){
                        addButtonRect.opacity = 1
                        addButtonRect.border.width = 1
                    } else {
                        addButtonRect.opacity = .8
                        addButtonRect.border.width = 0
                        addButtonRect.color = "lightgreen"
                    }
                }
                onPressed:  addButtonRect.color = "lightgrey"
                onReleased: addButtonRect.color = "lightgreen"

                highlighted: true
                anchors.verticalCenter: parent.verticalCenter
                height: 0.8 * buttonBarChangeSetList.height
                onClicked: {
                    var dg = newLegoSetComp.createObject(this)
                    dg.openDialog()
                }
            }

            RoundButton{
                id: forceLayoutBt
                text: qsTr("Force Layout")
                background: Rectangle { id: forceLayoutLegoSetRect; color: "lightblue";  radius: forceLayoutBt.radius; opacity: .8; border.width: 0;}
                onHoveredChanged: {
                    if (hovered){
                        forceLayoutLegoSetRect.opacity = 1
                        forceLayoutLegoSetRect.border.width = 1
                    } else {
                        forceLayoutLegoSetRect.opacity = .8
                        forceLayoutLegoSetRect.border.width = 0
                        forceLayoutLegoSetRect.color = "lightblue"
                    }
                }
                onPressed:  forceLayoutLegoSetRect.color = "lightgrey"
                onReleased: forceLayoutLegoSetRect.color = "lightblue"

                onClicked: legoTable.forceLayout()
                highlighted: true
                anchors.verticalCenter: parent.verticalCenter
                height: 0.8 * buttonBarChangeSetList.height
            }

            RoundButton{
                id: resizeCols
                text: qsTr("Resize Cols")

                background: Rectangle { id: resizeColsRect; color: "lightblue";  radius: resizeCols.radius; opacity: .8; border.width: 0;}
                onHoveredChanged: {
                    if (hovered){
                        resizeColsRect.opacity = 1
                        resizeColsRect.border.width = 1
                    } else {
                        resizeColsRect.opacity = .8
                        resizeColsRect.border.width = 0
                        resizeColsRect.color = "lightblue"
                    }
                }
                onPressed:  resizeColsRect.color = "lightgrey"
                onReleased: resizeColsRect.color = "lightblue"

                highlighted: true
                anchors.verticalCenter: parent.verticalCenter
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
