import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0

import "helper/"

Item {
    property alias model : legoTableView.model
    property alias contentY : legoTableView.contentY
    function resizeCols() {
        for ( var index = 0; index < legoTableView.model.columnCount(); ++index) {
            var width = Math.min(600, legoTableView.model.columnWidth(index))
            headerRepeater.itemAt(index).setWidth(width)
        }
        legoTableView.forceLayout()
    }

    Row {
        id: header
        width: legoTableView.contentWidth
        height: 40
        x: -legoTableView.contentX
        z: 1
        spacing: 4
        Repeater {
            id: headerRepeater
            model: legoTableView.model.columnCount()
            SortableColumnHeading {
                //table: legoTableView
                initialWidth: Math.min(600, legoTableView.model.columnWidth(index)); height: parent.height
                text: legoTableView.model.headerData(index, Qt.Horizontal)
                initialSortOrder: legoTableView.model.initialSortOrder(index)
                onSorting: {
                    for (var i = 0; i < headerRepeater.model; ++i)
                        if (i !== index)
                            headerRepeater.itemAt(i).stopSorting()
                    legoTableView.model.sort(index, state == "up" ? Qt.AscendingOrder : Qt.DescendingOrder)
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

        columnWidthProvider: function(column) { return headerRepeater.itemAt(column).width }

        delegate:
            DelegateChooser {
            role: "type"

            DelegateChoice {
                roleValue: "image"
                Rectangle {
                    color: "white"
                    implicitHeight: imageID.implicitHeight

                    Image {
                        id: imageID
                        width: parent.width
                        //height: 50
                        fillMode: Image.PreserveAspectFit
                        source: model.display
                    }
                }
            }

            DelegateChoice{
                Rectangle {
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
    }
}
