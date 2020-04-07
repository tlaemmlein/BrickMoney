import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Item {
    id : rootId
    function openDialog(){
        inputDialog.open()
    }

    property alias setNumber: setNumberField.value
    //signal inputDialogAccepted
    signal addLegoSetNumber

    Dialog {
        id: inputDialog

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 600
        parent: Overlay.overlay

        focus: true
        modal: true
        title: "New Lego Set"
        //standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            id: inputDialogLayout
            spacing: 20
            anchors.fill: parent
            Row {
                height: 30
                Label {
                    text: "LEGO set number: "
                    anchors.verticalCenter: parent.verticalCenter
                    elide: Label.ElideRight
                }

                Button {
                    id: prevLegoSetButton
                    text:"<"
                    onClicked: {
                        console.log("prevLegoSetButton");
                        LegoSetInfoGenerator.previousSetNumber(setNumberField.value)
                    }
                }

                TextField {
                    id : setNumberField
                    //anchors.fill: parent
                    width: 150
                    validator: IntValidator {bottom: 0; top: 2147483647;}
                    selectByMouse: true
                    property int value: parseInt(text)
                    text: "41632"
                    horizontalAlignment: TextInput.AlignHCenter

                    Component.onCompleted: {
                        LegoSetInfoGenerator.querySetNumber(value)
                    }

                    // font.pixelSize: legoTableView.pixelSize
                    onEditingFinished: {
                        console.log("onEditingFinished")
                        LegoSetInfoGenerator.querySetNumber(value)
                    }
                    Keys.onUpPressed:
                    {
                        console.log("Key up");
                        LegoSetInfoGenerator.nextSetNumber(value)
                    }
                    Keys.onDownPressed:
                    {
                        console.log("Key down");
                        LegoSetInfoGenerator.previousSetNumber(value)
                    }

                }

                Button {
                    id: nextLegoSetButton
                    text:">"
                    onClicked: {
                        console.log("nextLegoSetButton");
                        LegoSetInfoGenerator.nextSetNumber(setNumberField.value)
                    }
                }

            }

            Row {
                Image {
                    id: legoSetImage
                    width: 150
                    height: 150
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/images/Empty.svg"
                }

                Grid{
                    columns: 2
                    spacing: 2
                    Text { id: legsetName; text: "Name:"; font.pixelSize: 16 }
                    Text {

                        id: description
                        text: ""
                        font.pixelSize: 16
                        //elide: Text.ElideRight
                        width: inputDialog.width - legoSetImage.width - inputDialogLayout.spacing - 1.2*legsetName.width
                        wrapMode: Text.Wrap
                    }

                    Text {text: "Year:"; font.pixelSize: 16 }
                    Text {id: year; text: ""; font.pixelSize: 16 }

                    Text {text: "RRP:"; font.pixelSize: 16 }
                    Text {id: rrprice; text: ""; font.pixelSize: 16 }
                }
            }

            Row {
                spacing: 5

                RoundButton {
                    id: addLegoSetButton
                    text:"Add"
                    background: Rectangle { id: addLegoSetRect; color: "lightblue";  radius: addLegoSetButton.radius; opacity: .8; border.width: 0;}
                    onClicked: {
                        console.log("addLegoSetButton");
                        rootId.addLegoSetNumber()
                    }
                    onHoveredChanged: {
                        if (hovered){
                            addLegoSetRect.opacity = 1
                            addLegoSetRect.border.width = 1
                        } else {
                            addLegoSetRect.opacity = .8
                            addLegoSetRect.border.width = 0
                            addLegoSetRect.color = "lightblue"
                        }
                    }
                    onPressed:  addLegoSetRect.color = "lightgrey"
                    onReleased: addLegoSetRect.color = "lightblue"

                }
                RoundButton {
                    id: addAndCloseLegoSetButton
                    text:"Add && Close"
                    background: Rectangle { id: addAndCloseLegoSetRect; color: "orange";  radius: addAndCloseLegoSetButton.radius; opacity: .8; border.width: 0; }
                    onClicked: {
                        console.log("addAndCloseLegoSetButton");
                        inputDialog.accept()
                    }
                    onHoveredChanged: {
                        if (hovered){
                            addAndCloseLegoSetRect.opacity = 1
                            addAndCloseLegoSetRect.border.width = 1
                        } else {
                            addAndCloseLegoSetRect.opacity = .8
                            addAndCloseLegoSetRect.border.width = 0
                            addAndCloseLegoSetRect.color = "orange"
                        }
                    }
                    onPressed:  addAndCloseLegoSetRect.color = "lightgrey"
                    onReleased: addAndCloseLegoSetRect.color = "orange"

                }
                RoundButton {
                    id: closeLegoSetButton
                    text:"Close"
                    background: Rectangle { id: closeLegoSetRect; color: "red";  radius: closeLegoSetButton.radius; opacity: .8; border.width: 0; }
                    onClicked: {
                        console.log("closeLegoSetButton");
                        inputDialog.reject()
                    }
                    onHoveredChanged: {
                        if (hovered){
                            closeLegoSetRect.opacity = 1
                            closeLegoSetRect.border.width = 1
                        } else {
                            closeLegoSetRect.opacity = .8
                            closeLegoSetRect.border.width = 0
                            closeLegoSetRect.color = "red"
                        }
                    }
                    onPressed:  closeLegoSetRect.color = "lightgrey"
                    onReleased: closeLegoSetRect.color = "red"
                }

            }
        }

        Component.onCompleted: {
            console.log("onCompleted")
            LegoSetInfoGenerator.setNumber.connect(onLegoSetNumber)
            LegoSetInfoGenerator.imageUrl.connect(onLegoSetImageUrl)
            LegoSetInfoGenerator.description.connect(onLegoSetDescription)
            LegoSetInfoGenerator.year.connect(onLegoSetYear)
            LegoSetInfoGenerator.recommendedRetailPrice.connect(onLegoSetRRPrice)
            LegoSetInfoGenerator.setNumberNotFound.connect(onSetNumberNotFound)
        }

        function onLegoSetNumber(setnum)
        {
            console.log("onLegoSetNumber: "  +setnum)
            if ( setnum !== setNumberField.value)  {
                setNumberField.text = setnum
            }
        }

        function onLegoSetImageUrl(imageUrl) {
            console.log("onLegoSetImageUrl: "  +imageUrl)
            legoSetImage.source = imageUrl
        }

        function onLegoSetDescription(desc) {
            console.log("onLegoSetDescription: " + desc)
            description.text = desc
        }

        function onLegoSetYear(value) {
            console.log("onLegoSetYear: " + value)
            year.text = value
        }

        function onLegoSetRRPrice(value) {
            console.log("onLegoSetRRPrice: " + value)
            rrprice.text = value +"€"
        }

        function onSetNumberNotFound() {
            console.log("onSetNumberNotFound")
            legoSetImage.source = "qrc:/images/Empty.svg"
            description.text = "n.a."
            year.text = "n.a."
            rrprice.text = "n.a."
        }

        onAccepted: {
            console.log("Accepted adding lego set")
            rootId.addLegoSetNumber()

        }
        onRejected: {
            console.log("Rejected adding lego set")
        }
    }
}

