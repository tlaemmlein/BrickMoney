import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import de.brickmoney.models 0.1

Item {
    property alias model : legoListView.model
    property alias zoom : legoListView.zoom

    function addLegoSet() {
        legoListView.model.addLegoSet()
    }
    function removeLegoSet(){}
    function isSelected(){ return false }
    function clearSelection() {}


    ListView {
        id : legoListView
        anchors.fill: parent
        clip: true
        property int zoom
        property real zFactor: zoom/100
        property var locale: Qt.locale()

        property int selLegoSetWidth: 20 * legoListView.zFactor
        property int legoSetInfoWidth: 350 * legoListView.zFactor
        property int purchasingWidth: 100 * legoListView.zFactor
        property int cheaperWidth: 80 * legoListView.zFactor
        property int sellerWidth: 150 * legoListView.zFactor
        property int purchaseDateWidth: 100 * legoListView.zFactor
        property int retailPriceWidth: 120 * legoListView.zFactor
        property int profitEurosWidth: 100 * legoListView.zFactor
        property int profitPercentWidth: 100 * legoListView.zFactor
        property int saleDateWidth: 100 * legoListView.zFactor
        property int soldOverWidth: 150 * legoListView.zFactor
        property int buyerWidth: 150 * legoListView.zFactor

        property int fontPixelSize: 10 * legoListView.zFactor

        headerPositioning: ListView.OverlayHeader

        header: Rectangle {
            id: headerItem
            height:30
            z: 2

            color : "gray"

            Row{
                anchors.fill: parent
                spacing: 2

                Rectangle {
                    id: selectionHeader
                    width: legoListView.selLegoSetWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                }

                Rectangle {
                    id: legoSetInfoHeader
                    width: legoListView.legoSetInfoWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Lego set Information")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: purchasingPriceHeader
                    width: legoListView.purchasingWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Purchasing Price €")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: cheaperHeader
                    width: legoListView.cheaperWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Cheaper")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: sellerHeader
                    width: legoListView.sellerWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Seller")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: purchasingDateHeader
                    width: legoListView.purchaseDateWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Purchase Date")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: retailPriceHeader
                    width: legoListView.retailPriceWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Retail Price €")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: profitEurosHeader
                    width: legoListView.profitEurosWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Profit €")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: profitPercentHeader
                    width: legoListView.profitPercentWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Profit %")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: saleDateHeader
                    width: legoListView.saleDateWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Sale Date")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: soldOverHeader
                    width: legoListView.soldOverWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Sold over")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }

                Rectangle {
                    id: buyerHeader
                    width: legoListView.buyerWidth
                    border.color: "black"
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Text {
                        text: qsTr("Buyer")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        font.pixelSize: legoListView.fontPixelSize
                    }
                }
            }
        }

        delegate: Rectangle {
            id : rectDelegate
            height: 80 * legoListView.zFactor
            border.color: "black"

            Row{
                anchors.fill: parent
                spacing: 2
                Rectangle {
                    width: legoListView.selLegoSetWidth
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    color: "white"
                    border.color: "black"

                    ToolButton{
                        id: removeLegoSetButton
                        Image {
                            anchors.fill: parent
                            source: "qrc:/images/Trash.svg"
                            fillMode: Image.PreserveAspectFit
                        }
                        anchors.fill: parent
                        onClicked: {
                            var dg = confirmRemoveComp.createObject(this)
                            dg.open()
                            dg.setActiveFocusToYesButton()
                        }
                    }
                }


                Component {
                    id: confirmRemoveComp
                    Dialog {
                        id: confirmRemoveDialog
                        objectName: "confirmRemoveDialog"
                        modal: true
                        title: "Do you want to remove the LegoSet " + index + "?"

                        function setActiveFocusToYesButton() { yesRemoveButton.forceActiveFocus(); }
                        width: 450

                        footer:
                            DialogButtonBox {
                            id: dialogBox
                            Button {
                                id: yesRemoveButton
                                text: qsTr("Yes")
                                focus: true
                                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                                Keys.onEnterPressed: confirmRemoveDialog.accept()
                                Keys.onReturnPressed: confirmRemoveDialog.accept()
                            }
                            Button {
                                text: qsTr("No")
                                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                            }
                        }

                        onAccepted: { legoListView.model.removeLegoSet(index); confirmRemoveDialog.destroy() }
                        onRejected: confirmRemoveDialog.destroy();
                    }
                }


                Rectangle {
                    id: legoSetInfo
                    width: legoListView.legoSetInfoWidth
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    color : "white"
                    border.color: "black"

                    Image {
                        id: legoSetImage
                        width: 50 * legoListView.zFactor
                        height: 50 * legoListView.zFactor
                        anchors.left: legoSetInfo.left
                        anchors.leftMargin: 5
                        fillMode: Image.PreserveAspectFit
                        source: model.imageUrl
                    }
                    Flow {
                        id: basicLegoSetInfos
                        anchors.left: legoSetImage.right
                        anchors.right: legoSetInfo.right
                        spacing: 5

                        Row {
                            id: setNumberRow
                            height: 25 * legoListView.zFactor
                            Label {
                                text: qsTr("LEGO set no:")
                                font.pixelSize: 12 * legoListView.zFactor
                                anchors.verticalCenter: parent.verticalCenter
                                elide: Label.ElideRight
                                height: setNumberRow.height
                            }

                            Button {
                                id: prevLegoSetButton
                                text:"<"
                                font.pixelSize: 12 * legoListView.zFactor
                                height: setNumberRow.height
                                onClicked: model.setNumber = LegoSetInfoGenerator.previousSetNumber(setNumberField.value)
                            }

                            TextField {
                                id : setNumberField
                                objectName: "setNumberField"
                                font.pixelSize: 12 * legoListView.zFactor
                                width: 100 * legoListView.zFactor
                                height: setNumberRow.height
                                validator: IntValidator {bottom: 0; top: 2147483647;}
                                selectByMouse: true
                                property int value: parseInt(text)
                                text: model.setNumber
                                horizontalAlignment: TextInput.AlignHCenter

                                onEditingFinished:  model.setNumber = value
                                Keys.onUpPressed:   model.setNumber = LegoSetInfoGenerator.nextSetNumber(value)
                                Keys.onDownPressed: model.setNumber = LegoSetInfoGenerator.previousSetNumber(value)
                            }

                            Button {
                                id: nextLegoSetButton
                                text:">"
                                font.pixelSize: 12 * legoListView.zFactor
                                height: setNumberRow.height
                                onClicked: model.setNumber = LegoSetInfoGenerator.nextSetNumber(setNumberField.value)
                            }
                        }


                        Text{
                            text : model.description
                            height: 14 * legoListView.zFactor
                            width: legoSetInfo.width - legoSetImage.width - 5
                            Layout.preferredWidth: legoSetInfo.width - legoSetImage.width - 5
                            Layout.maximumWidth: legoSetInfo.width - legoSetImage.width - 5
                            font.pixelSize: legoListView.fontPixelSize
                            elide: Text.ElideRight
                            clip: true
                        }

                        Text{
                            text : "Year: " + model.year
                            height: 12 * legoListView.zFactor
                            font.pixelSize: legoListView.fontPixelSize
                            clip: true
                        }
                        Text{
                            text : "RRPrice: " + model.recommendedRetailPrice + "€"
                            height: 12 * legoListView.zFactor
                            font.pixelSize: legoListView.fontPixelSize
                            clip: true
                        }
                        Text {
                            id: link_brickmerge
                            textFormat: Text.RichText
                            height: 12 * legoListView.zFactor
                            font.pixelSize: legoListView.fontPixelSize
                            text: '<html><a href="https://brickmerge.de/'+ model.setNumber + '">BM</a>&nbsp;<a href="https://brickset.com/sets/'+ model.setNumber + '">BS</a></html>'
                                   //<html><style type="text/css"></style><a href="https://brickset.com/sets/'+ model.setNumber + '">BS</a></html>'
                            onLinkActivated: Qt.openUrlExternally(link)
                        }
                    }
                }

                DoubleSpinBox {
                    id: purchasingPriceLegoSet
                    width: legoListView.purchasingWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    from: 0.0
                    value: model.purchasingPrice
                    stepSize: 0.1; editable: true
                    onValueChanged: {
                        model.purchasingPrice = value;
                    }
                }

                TextField {
                    id: cheaperLegoSet
                    width: legoListView.cheaperWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    readOnly: true
                    validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                    text: {
                        var number = model.cheaperPercent
                        number =  number !== null ? number.toLocaleString(locale, 'f', 2) +" %" : 0.0;
                        return number
                    }
                }

                TextField {
                    id: sellerLegoSet
                    width: legoListView.sellerWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.seller
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    onEditingFinished: {
                        model.seller = text
                    }
                }

                Rectangle {
                    width: legoListView.purchaseDateWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    color: "white"
                    border.color: "black"
                    DatePicker{
                        id: purchaseDateLegoSet
                        anchors.fill: parent
                        selectedDate: model.purchaseDate
                    }
                }

                DoubleSpinBox {
                    id: retailPriceLegoSet
                    width: legoListView.retailPriceWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    from: 0.0
                    value: model.retailPrice
                    stepSize: 0.1; editable: true
                    onValueChanged: {
                        model.retailPrice = value;
                    }
                }

                TextField {
                    id: profitEurosLegoSet
                    width: legoListView.profitEurosWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    readOnly: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                    text: {
                        var number = model.profitEuros
                        number =  number !== null ? number.toLocaleString(locale, 'f', 2) : 0.0;
                        return number
                    }
                }

                TextField {
                    id: profitPercentLegoSet
                    width: legoListView.profitPercentWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    readOnly: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                    text: {
                        var number = model.profitPercent
                        number =  number !== null ? number.toLocaleString(locale, 'f', 2) +"%" : 0.0;
                        return number
                    }
                }

                Rectangle {
                    width: legoListView.purchaseDateWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    color: "white"
                    border.color: "black"
                    DatePicker{
                        id: saleDateLegoSet
                        anchors.fill: parent
                        selectedDate: model.saleDate
                    }
                }

                TextField {
                    id: soldOverLegoSet
                    width: legoListView.soldOverWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.soldOver
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    onEditingFinished: {
                        model.soldOver = text
                    }
                }

                TextField {
                    id: buyerLegoSet
                    width: legoListView.buyerWidth
                    font.pixelSize: legoListView.fontPixelSize
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.buyer
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    onEditingFinished: {
                        model.buyer = text
                    }
                }
            }
        }
    }
}
