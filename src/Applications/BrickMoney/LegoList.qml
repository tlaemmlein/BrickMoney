import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import de.brickmoney.models 0.1

Item {
    property alias model : legoListView.model
    property alias rowHeight : legoListView.rowHeight
    function addLegoSet() {
        legoListView.model.addLegoSet()
    }
    function removeLegoSet(){}
    function isSelected(){ return false }
    function clearSelection() {}

    Image {
        id: legoSetImageLarge
        // anchors.fill: parent
        fillMode: Image.PreserveAspectFit
//        source: model.imageUrl
        z: 100
        visible: false
    }


    ListView {
        id : legoListView
        anchors.fill: parent
        clip: true
        property int rowHeight
        property var locale: Qt.locale()

        property int selLegoSetWidth: 20
        property int legoSetInfoWidth: 350
        property int purchasingWidth: 100
        property int cheaperWidth: 80
        property int sellerWidth: 150
        property int purchaseDateWidth: 100
        property int retailPriceWidth: 120
        property int profitEurosWidth: 100
        property int profitPercentWidth: 100
        property int saleDateWidth: 100
        property int soldOverWidth: 150
        property int buyerWidth: 150

        headerPositioning: ListView.OverlayHeader

        header: Rectangle {
            id: headerItem
            width: legoListView.width
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
                    }
                }
            }
        }

        delegate: Rectangle {
            id : rectDelegate
            height: 60
            border.color: "black"
            width: parent.width

            Row{
                anchors.fill: parent
                spacing: 2

                ToolButton{
                    id: removeLegoSetButton
                    Image {
                        anchors.fill: parent
                        source: "qrc:/images/Trash.svg"
                        fillMode: Image.PreserveAspectFit
                    }
                    width: legoListView.selLegoSetWidth
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    Dialog {
                        id: confirmRemoveDialog
                        modal: true
                        title: "Do you want to remove the LegoSet " + index + "?"
                        standardButtons: Dialog.Yes | Dialog.No

                        onAccepted:
                        {
                            console.log("Yes clicked")
                            legoListView.model.removeLegoSet(index)
                        }
                        onRejected: console.log("No clicked")
                    }
                    onClicked: {
                        confirmRemoveDialog.open()
                    }
                }


                Rectangle {
                    id: legoSetInfo
                    width: legoListView.legoSetInfoWidth
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        margins: 4
                    }
                    radius: 10
                    color : "white"
                    border.color: "black"

                    Image {
                        id: legoSetImage
                        width: 50
                        height: 50
                        anchors.left: legoSetInfo.left
                        anchors.leftMargin: 5
                        fillMode: Image.PreserveAspectFit
                        source: model.imageUrl

                        MouseArea {
                            hoverEnabled: true
                            anchors.fill: parent
                            onEntered: {
                                legoSetImage.width = 100
                                legoSetImage.height = 100
                                rectDelegate.height = 100
                            }
                            onExited: {
                                legoSetImage.width = 50
                                legoSetImage.height = 50
                                rectDelegate.height = 60
                            }
                        }
                    }
                    Flow {
                        id: basicLegoSetInfos
                        anchors.left: legoSetImage.right
                        anchors.right: legoSetInfo.right
                        spacing: 5

                        Text{
                            text : model.description
                            height: 12
                            width: legoSetInfo.width - legoSetImage.width - 5
                            Layout.preferredWidth: legoSetInfo.width - legoSetImage.width - 5
                            Layout.maximumWidth: legoSetInfo.width - legoSetImage.width - 5
                            font.pixelSize: 12
                            elide: Text.ElideRight
                            clip: true
                        }

                        Text{
                            text : "Set no.: " + model.setNumber
                            height: 12
                            font.pixelSize: 12
                            clip: true
                        }
                        Text{
                            text : "Year: " + model.year
                            height: 12
                            font.pixelSize: 12
                            clip: true
                        }
                        Text{
                            text : "RRPrice: " + model.recommendedRetailPrice + "€"
                            height: 12
                            font.pixelSize: 12
                            clip: true
                        }
                        Text {
                            id: link_brickmerge
                            textFormat: Text.RichText
                            text: '<html><a href="https://brickmerge.de/'+ model.setNumber + '">BM</a>&nbsp;<a href="https://brickset.com/sets/'+ model.setNumber + '">BS</a></html>'
                                   //<html><style type="text/css"></style><a href="https://brickset.com/sets/'+ model.setNumber + '">BS</a></html>'
                            onLinkActivated: Qt.openUrlExternally(link)
                        }
                    }
                }

                DoubleSpinBox {
                    id: purchasingPriceLegoSet
                    width: legoListView.purchasingWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    value: model.purchasingPrice
                    stepSize: 0.1; editable: true
                    onValueChanged: {
                        model.purchasingPrice = value;
                    }
                }

                TextField {
                    id: cheaperLegoSet
                    width: legoListView.cheaperWidth
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
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.seller
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    font.pixelSize: 12
                    onEditingFinished: {
                        model.seller = text
                    }
                }

                DatePicker{
                    id: purchaseDateLegoSet
                    width: legoListView.purchaseDateWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    selectedDate: model.purchaseDate
                }

                DoubleSpinBox {
                    id: retailPriceLegoSet
                    width: legoListView.retailPriceWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    value: model.retailPrice
                    stepSize: 0.1; editable: true
                    onValueChanged: {
                        model.retailPrice = value;
                    }
                }

                TextField {
                    id: profitEurosLegoSet
                    width: legoListView.profitEurosWidth
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

                DatePicker{
                    id: saleDateLegoSet
                    width: legoListView.saleDateWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    selectedDate: model.saleDate
                }

                TextField {
                    id: soldOverLegoSet
                    width: legoListView.soldOverWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.soldOver
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    font.pixelSize: 12
                    onEditingFinished: {
                        model.soldOver = text
                    }
                }

                TextField {
                    id: buyerLegoSet
                    width: legoListView.buyerWidth
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    text: model.buyer
                    selectByMouse: true
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    font.pixelSize: 12
                    onEditingFinished: {
                        model.buyer = text
                    }
                }
            }
        }
    }
}
