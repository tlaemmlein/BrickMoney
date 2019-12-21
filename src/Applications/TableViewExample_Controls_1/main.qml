import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TableView {
        id: tableView
        objectName: "tableView"
        horizontalScrollBarPolicy: -1
        selectionMode: SelectionMode.SingleSelection
        anchors.fill: parent

        TableViewColumn {
            id: titleColumn
            title: "Title"
            role: "title"
            movable: false
            resizable: false
            width: tableView.viewport.width - authorColumn.width
        }

        TableViewColumn {
            id: authorColumn
            title: "Author"
            role: "author"
            movable: false
            resizable: false
            width: tableView.viewport.width / 3
        }

        model: ListModel {
            id: libraryModel
            ListElement {
                title: "A Masterpiece"
                author: "Gabriel"
            }
            ListElement {
                title: "Brilliance"
                author: "Jens"
            }
            ListElement {
                title: "Outstanding"
                author: "Frederik"
            }
        }

        rowDelegate: Rectangle {
            height: 50
            SystemPalette {
                id: myPalette;
                colorGroup: SystemPalette.Active
            }
            color: {
                var baseColor = styleData.alternate ? myPalette.alternateBase : myPalette.base
                return styleData.selected ? myPalette.highlight : baseColor
            }

        }

        itemDelegate: Rectangle {
            Text {
                anchors { verticalCenter: parent.verticalCenter; left: parent.left }
                color: "black"
                text: styleData.value
            }

            MouseArea {
                id: cellMouseArea
                anchors.fill: parent
                onClicked: {
                    // Column index are zero based
                    if(styleData.column === 1){
                        loader.visible = true
                        loader.item.forceActiveFocus()
                    }
                }
            }

            Loader {
                id: loader
                anchors { verticalCenter: parent.verticalCenter; left: parent.left}
                height: parent.height
                width: parent.width
                visible: false
                sourceComponent: visible ? input : undefined

                Component {
                    id: input
                    TextField {
                        anchors { fill: parent }
                        text: ""
                        onAccepted:{
                            // DO STUFF
                            loader.visible = false
                        }

                        onActiveFocusChanged: {
                            if (!activeFocus) {
                                loader.visible = false
                            }
                        }
                    }
                }
            }
        }
    }
}
