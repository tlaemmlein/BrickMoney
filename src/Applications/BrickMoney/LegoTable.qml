import QtQuick 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.12
import de.brickmoney.models 0.1

Item {
    //id: legoTable
    property alias model : legoTableView.model
    property alias rowHeight : legoTableView.rowHeight

    function addLegoSet() {
        legoTableView.model.addLegoSet()
        legoTableView.positionViewAtRow(model.rowCount() -1)
        legoTableView.selection.clear()
        legoTableView.selection.select(model.rowCount() -1)
    }

    function removeLegoSet(){
        if ( model.rowCount() === 0)
        {
            return
        }

        var deletedRowIndex = 0

        legoTableView.selection.forEach( function(rowIndex) {
            //console.log(rowIndex)
            model.removeLegoSet(rowIndex)
            deletedRowIndex = rowIndex
        } )
        legoTableView.selection.clear()

        if ( model.rowCount() === 0)
        {
            return
        }

        //console.log("deletedRowIndex: " +deletedRowIndex)

        if (deletedRowIndex <  model.rowCount() )
        {
            legoTableView.selection.select(deletedRowIndex)
            return
        }

        if (deletedRowIndex-1 >= 0 )
        {
            legoTableView.selection.select(deletedRowIndex-1)
        }

    }

    function isSelected() {
        return legoTableView.selection.count
    }

    function clearSelection() {
        legoTableView.selection.clear()
    }

    C1.TableView {
        id: legoTableView
        anchors.fill: parent
        clip: true
        property int pixelSize: 12
        property int rowHeight : 40

        headerDelegate:
            Rectangle {
            color: "#808080"
            Text {
                text: styleData.value
                minimumPixelSize: 10
                font.pixelSize: legoTableView.pixelSize
            }
            width: childrenRect.width
            height: childrenRect.height
        }

        rowDelegate: Rectangle {
            height: legoTableView.rowHeight
            SystemPalette {
                id: myPalette;
                colorGroup: SystemPalette.Active
            }
            color: {
                var baseColor = styleData.alternate ? "#FFFFFF" : "#f5f5f5"
                styleData.selected ? "#03A9F4" : baseColor
            }
        }

        C1.TableViewColumn {
            id: imageColumn
            title: "Image"
            width: 100
            delegate:
                Image {
                id:  tableViewImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: model.imageFilePath

                TextField {
                    id: tableViewImageUrl
                    text: model.imageName
                    anchors.left: tableViewImage.left
                    anchors.right: tableViewImage.right
                    anchors.top: tableViewImage.top
                    visible: false
                    font.pixelSize: legoTableView.pixelSize
                    readOnly: true
                }

                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: {
                        //console.log("entered");
                        tableViewImageUrl.visible = true
                        tableViewImageUrl.forceActiveFocus()
                    }
                    onExited: {
                        //console.log("exited")
                        tableViewImageUrl.visible = false
                    }
                }
            }
        }

        C1.TableViewColumn {
            id: setNumColumn
            title: "Set number"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                validator: IntValidator {bottom: 0; top: 2147483647;}
                selectByMouse: true
                text: model.setNumber
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.setNumber = text
                }
            }
        }

        C1.TableViewColumn {
            id: descColumn
            title: "Description"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: model.description
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.description = text
                }
            }
        }

        C1.TableViewColumn {
            id: yearColumn
            title: "Year"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                validator: IntValidator {bottom: 0; top: 2147483647;}
                selectByMouse: true
                text: model.year
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.year = text
                }
            }
        }

        C1.TableViewColumn {
            id: recommendedRetailPriceColumn
            title: "RRP[€]"
            width: 100
            delegate:  DoubleSpinBox {
                value: model.recommendedRetailPrice
                stepSize: 0.1
                anchors.fill: parent
                editable: true
                font.pixelSize: legoTableView.pixelSize
                onValueChanged: {
                    model.recommendedRetailPrice = value;
                }
            }
        }

        C1.TableViewColumn {
            id: purchasingPriceColumn
            title: "Purchase Price[€]"
            width: 100
            delegate:  DoubleSpinBox {
                value: model.purchasingPrice
                stepSize: 0.1
                anchors.fill: parent
                editable: true
                font.pixelSize: legoTableView.pixelSize
                onValueChanged: {
                    model.purchasingPrice = value;
                }
            }
        }


        C1.TableViewColumn {
            id: cheaperPercentColumn
            title: "Cheaper [%]"
            width: 100
            delegate:  TextField {
                anchors.fill: parent
                readOnly: true
                validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                text: {
                    var number = model.cheaperPercent
                    number =  number !== null ? number.toLocaleString(locale, 'f', 2) +"%" : 0.0;
                    return number
                }
                font.pixelSize: legoTableView.pixelSize
            }
        }

        C1.TableViewColumn {
            id: sellerColumn
            title: "Seller"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: model.seller
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.seller = text
                }
            }
        }

        C1.TableViewColumn {
            id: purchaseDateColumn
            title: "Purchase Date"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: model.purchaseDate
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                inputMethodHints: Qt.ImhDate
                onEditingFinished: {
                    model.purchaseDate = text
                }
            }
        }

        C1.TableViewColumn {
            id: retailPriceColumn
            title: "Retail Price[€]"
            width: 100
            delegate:  DoubleSpinBox {
                value: model.retailPrice
                stepSize: 0.1
                anchors.fill: parent
                editable: true
                font.pixelSize: legoTableView.pixelSize
                onValueChanged: {
                    model.retailPrice = value;
                }
            }
        }

        C1.TableViewColumn {
            id: profitEurosColumn
            title: "Profit [€]"
            width: 100
            delegate:  TextField {
                anchors.fill: parent
                readOnly: true
                validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                text: {
                    var number = model.profitEuros
                    number =  number !== null ? number.toLocaleString(locale, 'f', 2) : 0.0;
                    return number
                }
                font.pixelSize: legoTableView.pixelSize
            }
        }

        C1.TableViewColumn {
            id: profitPercentColumn
            title: "Profit [%]"
            width: 100
            delegate:  TextField {
                anchors.fill: parent
                readOnly: true
                validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
                text: {
                    var number = model.profitPercent
                    number =  number !== null ? number.toLocaleString(locale, 'f', 2) +"%" : 0.0;
                    return number
                }
                font.pixelSize: legoTableView.pixelSize
            }
        }

        C1.TableViewColumn {
            id: saleDateColumn
            title: "Sale Date"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: model.saleDate
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                inputMethodHints: Qt.ImhDate
                onEditingFinished: {
                    model.saleDate = text
                }
            }
        }

        C1.TableViewColumn {
            id: soldOverColumn
            title: "Sold over"
            width: 100
            delegate: TextField {
                anchors.fill: parent
                text: model.soldOver
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.soldOver = text
                }
            }
        }

        C1.TableViewColumn {
            id: buyerColumn
            title: "Buyer"
            width: 100
            delegate: TextField {
                id: buyerSpinbox
                anchors.fill: parent
                text: model.buyer
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                font.pixelSize: legoTableView.pixelSize
                onEditingFinished: {
                    model.buyer = text
                }
//                Component.onCompleted: {
//                    console.log("completed: " + model.buyer)
//                    buyerSpinbox.text = model !== null ? model.buyer : ""
//                }
            }
        }
    }
}
