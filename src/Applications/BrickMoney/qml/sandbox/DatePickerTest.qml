import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1024
    height: 480
    title: qsTr("BrickMoney - Die Software für LEGO Investment")

    Button {
        id: testbutton
        x :400
        y: 200
        width: 300
        height: 20
        text: "Date"
    }

    DatePicker{
        width: 100
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        id: myDatePicker
        selectedDate: new Date("2020-01-05")
//        onSelectedDateChanged: {
//            print("Date: " + selectedDate)
//            var localeDate = selectedDate.toLocaleDateString(myDatePicker.locale, Locale.ShortFormat)
//            print("Locale date: " + localeDate)
//            testbutton.text = localeDate
//        }
    }


    Button {
        id: textDate
        x: 10
        y: mainWindow.height -30
        width: 100
        height: 30
        text:  Qt.formatDate(cal.selectedDate, "dd-MM-yyyy")
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
            selectedDate: new Date()
            onClicked:  {
                textDate.text=Qt.formatDate(cal.selectedDate, "dd-MM-yyyy");
                calPopup.close()
            }
        }
    }

}
