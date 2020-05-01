import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480

    ListView {
        id: list;
        objectName: "list"
        width: parent.width
        height: parent.height
//        contentWidth:
//        {
//            var dg = rectDelegateComp.createObject(this)
//            console.log(objectName + " dg.width: " + dg.width)
//            return dg.width
//        }
        contentWidth: widthOne + widthTwo + widthThree + rowSpacing * 2
        model: 3
        clip : true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        //verticalLayoutDirection: ListView.BottomToTop

        property int widthOne: 300
        property int widthTwo: 350
        property int widthThree: 25
        property int rowSpacing: 10

        ScrollBar.vertical: ScrollBar{
            //contentItem: Rectangle {
//                implicitWidth: 6
//                implicitHeight: 100
//                radius: width / 2
                //color: "black"
            //}
        }

        ScrollBar.horizontal: ScrollBar{
            //contentItem: Rectangle {
//                //implicitWidth: 6
//                //implicitHeight: 100
//                radius: width / 2
                //color: "red"
            //}
        }


        Component{
            id: rectDelegateComp
            Rectangle {
                id : rectDelegate
                objectName: "rectDelegate"
                height: 50;
                //color: (model.index %2 === 0 ? "darkgray" : "lightgray");
                //border.color: "black"

                Component.onCompleted: {
                    console.log(objectName + " width: " + width)
                }

                Row{
                    id: myRow
                    objectName: "Row"
                    Component.onCompleted: {
                        console.log(objectName + " onCompleted width: " + width)
                    }
                    onPositioningComplete: {
                        console.log(objectName + " onPositioningComplete width: " + width)
                    }

                    anchors.fill: parent
                    spacing: list.rowSpacing
                    Rectangle {
                        width: list.widthOne
                        anchors.top: parent.top; anchors.bottom: parent.bottom
                        color: "gray"; border.color: "black"
                        Component.onCompleted: console.log("width: " + width)
                    }
                    Rectangle {
                        width: list.widthTwo
                        anchors.top: parent.top; anchors.bottom: parent.bottom
                        color: "lightyellow"; border.color: "black"
                        Component.onCompleted: console.log("width: " + width)
                    }
                    Rectangle {
                        width: list.widthThree
                        anchors.top: parent.top; anchors.bottom: parent.bottom
                        color: "blue"; border.color: "black"
                        Component.onCompleted: console.log("width: " + width)
                    }
                }
            }
        }

        delegate: rectDelegateComp
    }

}
