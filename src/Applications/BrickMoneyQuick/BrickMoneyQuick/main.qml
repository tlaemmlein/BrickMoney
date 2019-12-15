import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.3
import TableModel 0.1

ApplicationWindow  {
    visible: true
    width: 640
    height: 480
    title: qsTr("BrickMoney - Software für LEGO Investment")

    Material.theme: Material.Dark
    Material.accent: Material.Purple





//    menuBar: MenuBar {
//        Menu {
//            title: qsTr("&File")
//            Action { text: qsTr("&New...") }
//            Action { text: qsTr("&Open...") }
//            Action { text: qsTr("&Save") }
//            Action { text: qsTr("Save &As...") }
//            MenuSeparator { }
//            Action { text: qsTr("&Quit") }
//        }
//        Menu {
//            title: qsTr("&Edit")
//            Action { text: qsTr("Cu&t") }
//            Action { text: qsTr("&Copy") }
//            Action { text: qsTr("&Paste") }
//        }
//        Menu {
//            title: qsTr("&Help")
//            Action { text: qsTr("&About") }
//        }
//    }

    header: TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Auf Lager")
        }
        TabButton {
            text: qsTr("Aktiv")
        }
        TabButton {
            text: qsTr("Verkauft")
        }
    }

    StackLayout {
        id: mainStack
        width: parent.width
        height: parent.height
        currentIndex: bar.currentIndex
        Item {
            id: storageTab

           TableView {
               //anchors.fill: parent
               width: mainStack.width
               height: mainStack.height
               columnSpacing: 1
               rowSpacing: 1
               clip: true

               model: TableModel {}

               delegate: Rectangle {
                   implicitWidth: 100
                   implicitHeight: 50
                   Text {
                       text: display
                   }
               }
           }

        }
        Item {
            id: activeTab
            Button {
                text: "Aktiv"
            }
        }
        Item {
            id: soldTab
            Button {
                text: "Verkauft"
            }
        }
    }



}
