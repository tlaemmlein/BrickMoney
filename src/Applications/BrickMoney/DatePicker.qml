import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1


Item {
    id: datePicker
    property date selectedDate: new Date()
    property var locale: Qt.locale()
    property int fontPixelSize: 12
    width: 100
    height: 30



    Button {
        id: textDate
        anchors.fill: parent
        text:  datePicker.selectedDate.toLocaleDateString(datePicker.locale, Locale.ShortFormat)
        font.pixelSize: datePicker.fontPixelSize
        onClicked:{
            calPopup.open()
        }
    }

    Popup {
        id: calPopup
        anchors.centerIn: Overlay.overlay
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        onOpened: {
            cal.visible = true
        }

        onClosed: {
            cal.visible = false
        }

        C1.Calendar{
            id:cal
            anchors.fill: parent
            visible: false
            selectedDate: datePicker.selectedDate
            onClicked:  {
                datePicker.selectedDate = cal.selectedDate
                datePicker.selectedDateChanged(cal.selectedDate) //emit signal to be sure
                calPopup.close()
            }
        }
    }

}
