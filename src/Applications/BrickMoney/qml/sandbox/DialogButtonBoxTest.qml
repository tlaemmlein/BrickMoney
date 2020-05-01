import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1024
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

    ToolButton{
        id: removeLegoSetButton
        objectName: "removeLegoSetButton"
        width: 200
        height: 50
        Image {
            anchors.fill: parent
            source: "qrc:/images/Trash.svg"
            fillMode: Image.PreserveAspectFit
        }

        onClicked: {
            console.log(objectName + ":onClicked")
            var dg = confirmRemoveComp.createObject(this)
            console.log(objectName + ": confirmRemoveComp is created")
            dg.open()
            console.log(objectName + ": confirmRemoveComp is opend")
            dg.setActiveFocusToYesButton()
        }
    }

    Component {
        id: confirmRemoveComp
        Dialog {
            id: confirmRemoveDialog
            objectName: "confirmRemoveDialog"
            modal: true
            property int index: 0
            title: "Do you want to remove the LegoSet " + index + "?"
            width: 450

            function setActiveFocusToYesButton() {
                console.log(objectName +": setActiveFocus")
                yesRemoveButton.forceActiveFocus()
            }


            footer: //FocusScope {
                    DialogButtonBox {
                id: dialogBox
                Button {
                    id: yesRemoveButton
                    objectName: "yesRemoveButton"
                    text: "Yes"
                    focus: true
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                    Keys.onEnterPressed: {
                        console.log("onEnterPressed")
                        confirmRemoveDialog.accept()
                    }

                    Component.onCompleted:
                    {
                        console.log(objectName + ":onCompleted: ")
                        //yesRemoveButton.forceActiveFocus()
                        //console.log(objectName + " HasFocus: " + activeFocus)
                    }
                }
                Button {
                    text: qsTr("No")
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                }
            }

            onAccepted:
            {
                console.log("Yes clicked")
                //legoListView.model.removeLegoSet(index)
                confirmRemoveDialog.destroy()
            }
            onRejected:
            {
                console.log("No clicked");
                confirmRemoveDialog.destroy();
            }

            Component.onCompleted:
            {
                console.log(objectName + ":onCompleted: ")
                //yesRemoveButton.forceActiveFocus()
                //console.log(objectName + " yesRemoveButton has focus: " + yesRemoveButton.activeFocus)
            }
            Component.onDestruction: console.log("onDestruction")
        }
    }

}
