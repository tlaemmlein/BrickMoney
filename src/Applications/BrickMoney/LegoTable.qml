import QtQuick 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.12
import LegoSetTableModel 0.1


C1.TableView {
    id: tableView
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.top: buttonBarChangeSetList.bottom
    anchors.topMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    height: parent.height - buttonBarChangeSetList.height
            - buttonBarManageSetList.height -15
    clip: true

    model: _LegoSetTableModel

    headerDelegate:
        Rectangle {
        color: "#808080"
        Text {
            text: styleData.value
            minimumPixelSize: 12
            font.pixelSize: 14
        }
        width: childrenRect.width
        height: childrenRect.height
    }

    rowDelegate: Rectangle {
        height: zoomSlider.value
        SystemPalette {
            id: myPalette;
            colorGroup: SystemPalette.Active
        }
        color: {
            //                var baseColor = styleData.alternate ? myPalette.alternateBase : myPalette.base
            var baseColor = styleData.alternate ? "#FFFFFF" : "#f5f5f5"
            styleData.selected ? "#03A9F4" : baseColor
            //return styleData.selected ? myPalette.highlight : baseColor
        }

    }

    C1.TableViewColumn {
        id: imageColumn
        role: "image"
        title: "Image"
        width: 100
        delegate:
            Image {
            id:  tableViewImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: styleData.value

            TextField {
                id: tableViewImageUrl
                text: styleData.value
                anchors.left: tableViewImage.left
                anchors.right: tableViewImage.right
                anchors.top: tableViewImage.top
                visible: false
                font.pixelSize: 10
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
        role: "setnumber"
        title: "Set number"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            validator: IntValidator {bottom: 0; top: 2147483647;}
            selectByMouse: true
            text: styleData.value
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(setNumColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }

    }

    C1.TableViewColumn {
        id: descColumn
        role: "description"
        title: "Description"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            text: styleData.value
            selectByMouse: true
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(descColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

    C1.TableViewColumn {
        id: yearColumn
        role: "year"
        title: "Year"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            validator: IntValidator {bottom: 0; top: 2147483647;}
            selectByMouse: true
            text: styleData.value
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(yearColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

    C1.TableViewColumn {
        id: rrpColumn
        role: "recommendedRetailPrice"
        title: "RRP[€]"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            validator: DoubleValidator {bottom: 0.0; top: 2147483647.0;}
            selectByMouse: true
            text: styleData.value
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(rrpColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

    C1.TableViewColumn {
        id: purchasingPriceColumn
        role: "purchasingPrice"
        title: "PPrice[€]"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            validator: DoubleValidator {bottom: 0.0; top: 2147483647.0;}
            selectByMouse: true
            text: styleData.value
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(purchasingPriceColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

    C1.TableViewColumn {
        id: cheaperPercentColumn
        role: "cheaperPercent"
        title: "Cheaper [%]"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            readOnly: true
            validator: DoubleValidator {bottom: -2147483647.0; top: 2147483647.0;}
            text: {
                var raw = styleData.value
                return raw !== null ? raw.toFixed(2) +"%" : 0.0;
            }
        }
    }

    C1.TableViewColumn {
        id: sellerColumn
        role: "seller"
        title: "Seller"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            text: styleData.value
            selectByMouse: true
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(sellerColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

    C1.TableViewColumn {
        id: purchaseDateColumn
        role: "purchaseDate"
        title: "Purchase Date"
        width: 100
        delegate: TextField {
            anchors.fill: parent
            text: styleData.value
            selectByMouse: true
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            onEditingFinished: {
                var roleID = _LegoSetTableModel.roleID(purchaseDateColumn.role)
                var q_model_index = _LegoSetTableModel.index(styleData.row, styleData.column)
                _LegoSetTableModel.setData(q_model_index, text, roleID)
            }
        }
    }

}

