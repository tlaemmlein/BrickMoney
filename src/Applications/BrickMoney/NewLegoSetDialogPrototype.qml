import QtQuick 2.0

Item {
    Column {
        id: column
        x: 0
        y: 0
        width: 640
        height: 480

        Row {
            id: row
            width: 598
            height: 63

            Text {
                id: element
                text: "Lego Set Number: "
                font.pixelSize: 12
            }
        }
    }

}
