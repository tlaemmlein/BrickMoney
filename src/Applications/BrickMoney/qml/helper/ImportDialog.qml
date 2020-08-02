import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Dialog {
    id: importLegoSetsDialog

    Component.onCompleted: ImportLegoSetTableModel.clearAll()

    BrickMoneyFileDialog {
        id: loadFromCsvDialog
        objectName: "importFileDialog"
        title: qsTr("Import from csv file...")
        nameFilters: ["CSV files (*.csv)"]
        fileMode: BrickMoneyFileDialog.OpenFile
        folder: BrickMoneySettings.brickMoneyFilePath
        onAccepted: BrickMoneyProject.loadDataFrom(currentFile)
    }

    implicitWidth: parent.width
    implicitHeight: parent.height
    parent: Overlay.overlay
    focus: true
    modal: true
    title: qsTr("Import Lego Sets")
    standardButtons: Dialog.Close

    contentItem:
        Column {
            id: importDialogLayout
            spacing: 10
            Rectangle {
                id: buttonBarChangeSetList
                //width: childrenRect.width + 20
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

                    BrickMoneyRoundButton {
                        id: loadFromCsvBtn
                        text: qsTr("Load from csv file...")
                        pressedColor: "lightgrey"
                        releasedColor: "orange"
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: loadFromCsvDialog.open()
                    }

                    BrickMoneyRoundButton {
                        id: importToInStock
                        visible: ImportLegoSetTableModel.SelectionIsDirty
                        text: qsTr("To In Stock")
                        pressedColor: "lightgrey"
                        releasedColor: BrickMoneyStyle.inStockColor
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: BrickMoneyProject.moveSelectedLegoSets(ImportLegoSetTableModel, InStockLegoSetTableModel)
                    }

                    BrickMoneyRoundButton {
                        id: importToForSale
                        visible: ImportLegoSetTableModel.SelectionIsDirty
                        text: qsTr("To For Sale")
                        pressedColor: "lightgrey"
                        releasedColor: BrickMoneyStyle.forSaleColor
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: BrickMoneyProject.moveSelectedLegoSets(ImportLegoSetTableModel, ForSaleLegoSetTableModel)
                    }

                    BrickMoneyRoundButton {
                        id: importToSold
                        visible: ImportLegoSetTableModel.SelectionIsDirty
                        text: qsTr("To Sold")
                        pressedColor: "lightgrey"
                        releasedColor: BrickMoneyStyle.soldColor
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: BrickMoneyProject.moveSelectedLegoSets(ImportLegoSetTableModel, SoldLegoSetTableModel)
                    }

                    BrickMoneyRoundButton {
                        id: deleteLegoSets
                        visible: ImportLegoSetTableModel.SelectionIsDirty
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
                                contentText: "ID(s): " + ImportLegoSetTableModel.getSelectedLegoSetIDs()
                                onAccepted: {ImportLegoSetTableModel.removeSelectedLegoSets(); confirmDeleteDialog.destroy(); }
                                onRejected: confirmDeleteDialog.destroy()
                            }
                        }
                    }

                    BrickMoneyRoundButton {
                        id: forceLayoutBt
                        text: qsTr("Force Layout")
                        //visible: (legoTable.model.NumberOfLegoSets > 0)
                        pressedColor: "lightgrey"
                        releasedColor: "lightblue"
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: legoTable.forceLayout()
                    }

                    BrickMoneyRoundButton {
                        id: resizeCols
                        text: qsTr("Resize Cols")
                        //visible: legoTable.model.NumberOfLegoSets > 0
                        pressedColor: "lightgrey"
                        releasedColor: "lightblue"
                        height: 0.8 * buttonBarChangeSetList.height
                        onClicked: legoTable.resizeCols()
                    }

                    Text {
                      text: qsTr("Filter:")
                      //visible: legoTable.model.NumberOfLegoSets > 0
                      font.pointSize: 14
                      height: 0.8 * buttonBarChangeSetList.height
                      verticalAlignment : Text.AlignVCenter
                    }

                    TextField {
                        id: tfFilter
                        //visible: legoTable.model.NumberOfLegoSets > 0
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
                         ImportLegoSetTableSortModel.filterText = tfFilter.text
                         return ImportLegoSetTableSortModel
                }
            }


        }
}

