import QtQuick 2.12
import QtQuick.Window 2.12
import TableModel 0.1
import QtQuick.Controls 2.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")


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
                console.log("Add")
                _TableModel.newEntry()
                tableView.contentY = tableView.contentHeight - tableView.height

            }
        }

        Button{
            id: deleteButton
            text: "Delete"
            highlighted: true
            flat: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addButton.right
            anchors.leftMargin: 5
            height: 40
        }


    }


    TableView {
        id: tableView
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: buttonBar.bottom
        anchors.topMargin: 5
        width: parent.width -5
        height: parent.height - buttonBar.height - 15
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        model: _TableModel



        delegate: Rectangle {
            id: delegateRect
            implicitWidth: 100
            implicitHeight: 50
            border.color: "black"
            border.width: 2
            color: (heading==true)?"red":"green"
            Text {
                text: tabledata
                font.pointSize: 12
                anchors.centerIn: parent
            }
            MouseArea {
                       anchors.fill: parent
                       hoverEnabled: true
                       cursorShape: Qt.PointingHandCursor

                       onClicked: {
                           console.log("onClicked")
                       }

                       onHoveredChanged: {
                           console.log("onHoveredChanged: " + _TableModel)
                           delegateRect.color = "yellow"
                       }
                   }
        }
    }

}
