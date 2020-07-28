import QtQuick 2.0
import QtQuick.Controls 2.12

RoundButton{
    id: brickMoneyButton
    property string pressedColor: "lightgrey"
    property string releasedColor: "lightgreen"
    background: Rectangle { id: brickMoneyButtonRect; color: releasedColor;  radius: brickMoneyButton.radius; opacity: .8; border.width: 0;}
    onHoveredChanged: {
        if (hovered){
            brickMoneyButtonRect.opacity = 1
            brickMoneyButtonRect.border.width = 1
        } else {
            brickMoneyButtonRect.opacity = .8
            brickMoneyButtonRect.border.width = 0
            brickMoneyButtonRect.color = releasedColor
        }
    }
    onPressed:  brickMoneyButtonRect.color = pressedColor
    onReleased: brickMoneyButtonRect.color = releasedColor

    highlighted: true
    anchors.verticalCenter: parent.verticalCenter
}
