import QtQuick 2.12
import QtQuick.Window 2.12
import TableModel 0.1
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as C1

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

    function listProperty(item)
    {
        for (var p in item)
        console.log(p + ": " + item[p]);
    }

    Rectangle {
        id: buttonBar
        width: parent.width -10
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: 50
        color: "#00000000"
        radius: 8
        border.width: 2

        Button{
            id: addButton
            text: "Add"
            highlighted: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                //console.log("Add")
                _TableModel.newEntry()
                tableView.positionViewAtRow(_TableModel.rowCount() -1)
                tableView.selection.clear()
                tableView.selection.select(_TableModel.rowCount() -1)
            }
        }

        Button{
            id: deleteButton
            text: "Delete"
            highlighted: true
            flat: false
            enabled: tableView.selection.count > 0 ? true: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: {
                //console.log("Delete")
                if ( _TableModel.rowCount() === 0)
                {
                   return
                }

                var deletedRowIndex = 0

                tableView.selection.forEach( function(rowIndex) {
                    //console.log(rowIndex)
                    _TableModel.deleteEntry(rowIndex)
                    deletedRowIndex = rowIndex
                } )
                tableView.selection.clear()

                if ( _TableModel.rowCount() === 0)
                {
                   return
                }

                //console.log("deletedRowIndex: " +deletedRowIndex)

                if (deletedRowIndex <  _TableModel.rowCount() )
                {
                    tableView.selection.select(deletedRowIndex)
                    return
                }

                if (deletedRowIndex-1 >= 0 )
                {
                    tableView.selection.select(deletedRowIndex-1)
                }

            }
        }

        Button{
            id: clearSelectButton
            text: "Clear Selection"
            highlighted: true
            flat: false
            enabled: tableView.selection.count
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: deleteButton.right
            anchors.leftMargin: 5
            height: 40
            onClicked: tableView.selection.clear()
        }
    }


    C1.TableView {
        id: tableView
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: buttonBar.bottom
        anchors.topMargin: 5
        width: parent.width -5
        height: parent.height - buttonBar.height - 15
        clip: true

        model: _TableModel

        rowDelegate: Rectangle {
            height: 70
            SystemPalette {
                id: myPalette;
                colorGroup: SystemPalette.Active
            }
            color: {
                var baseColor = styleData.alternate ? myPalette.alternateBase : myPalette.base
                return styleData.selected ? myPalette.highlight : baseColor
            }

        }

        C1.TableViewColumn {
            id: imageColumn
            role: "image"
            title: "Bild"
            width: 100
            delegate: Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: styleData.value
            }
        }

        C1.TableViewColumn {
            id: setNumColumn
            role: "setnumber"
            title: "Set Nummer"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                validator: IntValidator {bottom: 0; top: 2147483647;}
                selectByMouse: true
                text: styleData.value
                onEditingFinished: {
                    var roleID = _TableModel.roleID(setNumColumn.role)
                    var q_model_index = _TableModel.index(styleData.row, styleData.column)
                    var data_changed = _TableModel.setData(q_model_index, text, roleID)
                }
            }

        }

        C1.TableViewColumn {
            id: descColumn
            role: "description"
            title: "Bezeichnung"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: styleData.value
                selectByMouse: true
                onEditingFinished: {
                    var roleID = _TableModel.roleID(descColumn.role)
                    var q_model_index = _TableModel.index(styleData.row, styleData.column)
                    var data_changed = _TableModel.setData(q_model_index, text, roleID)
                }
            }
        }
    }

}
