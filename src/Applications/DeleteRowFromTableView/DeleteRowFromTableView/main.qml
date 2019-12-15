import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")



    Item {
        id:root
        anchors.fill: parent
        anchors.margins: 8

        Component {
            id: comboDelegate

            ComboBox {
                editable:true
                model: ListModel {
                    ListElement { text: "checkbox" }
                    ListElement { text: "edit" }
                    ListElement { text: "anyotheroption" }
                }
            }
        }



        Component {
            id: comboDelegatetext
            Text {
                text: itemText
            }
        }


        ColumnLayout {
            id: mainLayout
            anchors.fill: parent
            spacing: 4


            GroupBox {
                id: gridBox
                title: "Grid layout"
                Layout.fillWidth: true


                GridLayout {
                    id: gridLayout
                    anchors.fill: parent
                    rows: 4
                    flow: GridLayout.TopToBottom

                    ToolButton {  iconSource: "../images/window-new.png"
                        Accessible.name: "create a new row"
                        onClicked: {
                            myModel.append({prompt: "pmpt", variable:"LINEDIT",tabname:"TABNAME"})
                            console.log(ListView.currentIndex)
                        }
                        tooltip: "creaate a new row" }

                    ToolButton { iconSource: "../images/up.png"
                        onClicked:{

                            tableview.selection.forEach( function(rowIndex) {console.log(rowIndex)} )
                        }
                        tooltip: "go up" }

                    ToolButton { Accessible.name: "Save as"
                        iconSource: "../images/down.png"
                        tooltip: "go down." }

                    ToolButton {
                        iconSource: "../images/process-stop.png"

                        onClicked: {
                            myModel.remove(ListView.currentIndex)
                            //console.log(tablemodel.currentIndex)
                        }
                        tooltip: "Delete the row" }



                    TableView{
                        id: tablemodel
                        Layout.rowSpan: 4
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        alternatingRowColors: true

                        //anchors.fill: parent

                        TableViewColumn {
                            role: "index"
                            title: "#"
                            width: 35
                            resizable: false
                            movable: false
                        }
                        TableViewColumn {
                            role: "prompt"
                            title: "Prompt"
                            width: 120
                            delegate: Component {
                                id: codeDelegate
                                TextEdit {
                                    visible:true
                                    text: ""

                                }
                            }
                        }

                        TableViewColumn {
                            role: "variable"
                            title: "Variable"
                            width: 120
                            delegate: Component {
                                id: codeDelegate1
                                TextEdit {
                                    text: ""


                                }
                            }
                        }
                        TableViewColumn {
                            role: "type"
                            title: "Type"
                            width: 200
                            visible: true
                        }
                        TableViewColumn {
                            role: "tabname"
                            title: "Tab Name"
                            Layout.fillWidth: true
                            visible: true
                        }
                        model: ListModel {

                            id: myModel
                        }
                        itemDelegate: Item {
                            height: 20
                            Loader {
                                Loader {
                                    property string itemText: styleData.value
                                    sourceComponent: styleData.column === 3 ? comboDelegate : comboDelegatetext
                                }
                            }
                        }



                    }
                }
            }



            Rectangle{
                Layout.fillHeight: true
                Layout.fillWidth: true
                GridLayout {
                    //id: gridLayout
                    rows: 1
                    flow: GridLayout.TopToBottom
                    anchors.fill: parent
                    GridLayout{
                        id: grid1
                        columns: 2
                        anchors.fill: parent
                        GroupBox {
                            //id: gridBox
                            title: "Grid layout"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                        GroupBox {
                            //id: gridBox
                            title: "Grid layout"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                    }

                }
            }
        }
    }
}
