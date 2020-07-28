import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0
import QtQml 2.15

import "helper/"

Item {
    property alias model : legoTableView.model
    property alias contentY : legoTableView.contentY
    function resizeCols() {
        //console.log("resizeCols")
		legoTableView.refresh()
    }

    function forceLayout() {
        legoTableView.forceLayout()
    }

    QtObject {
        id: theme
        property font tableFont: Qt.font({
            //family: 'Encode Sans',
            //weight: Font.Black,
            //italic: false,
            pointSize: 8
        })
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
                initialWidth: Math.min(600, legoTableView.model.columnWidth(index, theme.tableFont))
                height: parent.height
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
		property var locale: Qt.locale()

		Component.onCompleted: {
            model.modelReset.connect(refresh)
        }


		function refresh() {
            //console.log("refresh")
            for ( var index = 0; index < model.columnCount(); ++index) {
                var width = Math.min(600, model.columnWidth(index, theme.tableFont))
                //console.log(index + " " + width)
                headerRepeater.itemAt(index).setWidth(width)
            }
            forceLayout()
        }

        columnWidthProvider: function(column) { return headerRepeater.itemAt(column).width }

        delegate:
            DelegateChooser {
            role: "type"

            DelegateChoice{
                roleValue: "selection"
                Rectangle {
                    color: "#EEE"
                    implicitHeight: selectionBox.implicitHeight
                    CheckBox {
                        id: selectionBox
                        font: theme.tableFont
                        checked: model.display
                        anchors.fill: parent
                        onCheckedChanged: model.display = checked
                    }
                }
            }

            DelegateChoice {
                roleValue: "image"
                Rectangle {
                    color: "transparent"
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

            DelegateChoice {
                roleValue: "date"
                Rectangle {
                    color: "white"
                    border.color: "black"
                    DatePicker{
                        //width: parent.width
                        anchors.fill: parent
                        //fontPixelSize: legoListView.fontPixelSize
                        selectedDate: model.display
                        onSelectedDateChanged: model.display = selectedDate
                    }
                }
            }

            DelegateChoice {
                roleValue: "readonlydouble"
                Rectangle {
                    color: "#EEE"
                    implicitHeight: textReadOnlyDoubleID.implicitHeight
                    Text {
                        id: textReadOnlyDoubleID
                        anchors.fill: parent
                        horizontalAlignment : Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        rightPadding: 2
                        elide: column == 49 ? Text.ElideLeft : Text.ElideRight
                        font: theme.tableFont
                        text: {
                            var number = model.display
                            number =  number !== null ? number.toLocaleString(locale, 'f', 2) : 0.0;
                            return number
                        }
                    }
                }
            }

            DelegateChoice {
                roleValue: "double"
                Rectangle {
                    color: "#EEE"
                    implicitHeight: textDoubleID.implicitHeight
                    TextField {
                        id: textDoubleID
                        background: Rectangle {
                            color:   textDoubleID.activeFocus ? "white" : "transparent"
                            border.color: textDoubleID.activeFocus ? "#21be2b" : "transparent"
                        }

                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignRight
                        validator: DoubleValidator {bottom: 0.0; top: 1000000.0; decimals: 2}
                        selectByMouse: true
                        font: theme.tableFont
						property string initValue
                        text: {
                            var number = model.display
                            number =  number !== null ? number.toLocaleString(locale, 'f', 2) : 0.0;
							initValue = number
                            return number
                        }
                        onEditingFinished:  {model.display = Number.fromLocaleString(locale, text); focus = false}
                        Keys.onEscapePressed:{ text = initValue; focus = false }
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                selectAll()
                            }
                        }
                    }
                }
            }

            DelegateChoice {
                roleValue: "text"
                Rectangle {
                    color: "#EEE"
                    implicitHeight: textID.implicitHeight
                    TextField {
                        id: textID
                        background: Rectangle {
                            color:   textID.activeFocus ? "white" : "transparent"
                            border.color: textID.activeFocus ? "#21be2b" : "transparent"
                        }

                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignRight
                        selectByMouse: true
                        font: theme.tableFont
						property string initValue
                        text: {
                            initValue = model.display
                            return model.display
                        }
                        onEditingFinished:  {model.display = text; focus = false}
                        Keys.onEscapePressed:{ text = initValue; focus = false }
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                selectAll()
                            }
                        }
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
                        font: theme.tableFont
                        anchors.fill: parent
                        horizontalAlignment : Text.AlignRight
                        rightPadding: 2
                        verticalAlignment: Text.AlignVCenter
                        elide: column == 49 ? Text.ElideLeft : Text.ElideRight
                    }
                }
            }
        }
    }
}
