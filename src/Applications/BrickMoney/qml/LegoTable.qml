import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.12
import "helper/"

Item {
    property alias model : legoTableView.model

    Row {
        id: header
        width: legoTableView.contentWidth
        height: 40
        x: -legoTableView.contentX
        z: 1
        spacing: 4
        Repeater {
            model: legoTableView.model.columnCount()
            Rectangle {
                width: legoTableView.model.columnWidth(index); height: parent.height
                color: "orange"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 4
                    width: parent.width - 4
                    text: legoTableView.model.headerData(index, Qt.Horizontal)
                }
            }
        }
    }
    TableView {
        id: legoTableView
        anchors.fill: parent
        anchors.topMargin: header.height
        clip: true
        columnSpacing: 4; rowSpacing: 4
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        ScrollBar.vertical: ScrollBar { interactive:true; contentItem: Rectangle { color:"#c2c2c2"; radius: width / 2} }
        ScrollBar.horizontal: ScrollBar {contentItem: Rectangle { color:"#c2c2c2"; radius: width / 2} }

        columnWidthProvider: function(column) { return Math.min(600, model.columnWidth(column)) }

        delegate: Rectangle {
            color: "#EEE"
            implicitHeight: text.implicitHeight
            Text {
                id: text
                text: model.display
                width: parent.width
                horizontalAlignment : Text.AlignRight
                elide: column == 49 ? Text.ElideLeft : Text.ElideRight
                font.preferShaping: false
            }
        }
    }
}
