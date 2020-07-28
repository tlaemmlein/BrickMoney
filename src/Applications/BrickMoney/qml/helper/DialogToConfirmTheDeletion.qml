import QtQuick 2.0
import QtQuick.Controls 2.12

Dialog {
    id: dialogToConfirmTheDeletion
    property string dialogTitle
    property string contentText

    function setActiveFocusToYesButton() { yesDeleteButton.forceActiveFocus(); }

    modal: true
    title: dialogTitle

    contentItem: Text {
        font: dialogToConfirmTheDeletion.font
        text: contentText
        elide: Text.ElideMiddle
    }

    footer:
        DialogButtonBox {
        id: dialogBox
        Button {
            id: yesDeleteButton
            text: qsTr("Yes")
            focus: true
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            Keys.onEnterPressed: dialogToConfirmTheDeletion.accept()
            Keys.onReturnPressed: dialogToConfirmTheDeletion.accept()
        }
        Button {
            text: qsTr("No")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
    }
}
