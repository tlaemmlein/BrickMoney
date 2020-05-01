import QtQuick 2.0
import QtQuick.Controls 2.1
import QmlUtils 1.0

Item {
    id: doublespinbox
    width: 140
    height: 40
    property int decimals: 2
    property alias value: valuePreview.value
    property real from: -100000.0
    property real to:    100000.0
    property real stepSize: 0.1
    property alias editable: spinbox.editable
    property alias font: spinbox.font

    SpinBox
        {
            id: spinbox
            property bool init: false
            property real factor: Math.pow(10, decimals)

            function setValue(preview)
            {
                init = true
                value = preview.value * factor
                init = false
                preview.value = value / factor
            }

            DoubleValuePreview
            {
                id: valuePreview
                onValuePreview: spinbox.setValue(preview)
            }

            anchors.fill: parent
            editable: true
            stepSize: doublespinbox.stepSize * factor
            to : doublespinbox.to * factor
            from : doublespinbox.from * factor
            clip: true

            contentItem: TextInput {
                    z: 2
                    text: spinbox.textFromValue(spinbox.value, spinbox.locale)

                    font: spinbox.font
//                    color: "#21be2b"
//                    selectionColor: "#21be2b"
//                    selectedTextColor: "#ffffff"
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter

                    readOnly: !spinbox.editable
                    validator: spinbox.validator
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                }

            onValueChanged:
            {
                if (init)
                    return

                valuePreview.setValueDirect(value / factor)
            }

            validator: DoubleValidator
            {
                bottom: Math.min(spinbox.from, spinbox.to)
                top: Math.max(spinbox.from, spinbox.to)
            }

            textFromValue: function(value, locale)
            {
                return Number(value / factor).toLocaleString(locale, 'f', doublespinbox.decimals)
            }

            valueFromText: function(text, locale)
            {
                doublespinbox.value = Number.fromLocaleString(locale, text)
                return doublespinbox.value * factor
            }
        }
}
