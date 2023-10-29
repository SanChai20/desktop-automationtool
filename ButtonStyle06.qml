import QtQuick 2.0
import QtQuick.Controls 2.5
import "globalConfiguration.js" as GlobalConfig

Button
{
    id: rootBtn
    property string btnIconCode: ""
    property real btnIconPixelSize: 15
    property real btnIconLetterSpacingSize: 15
    property string buttonTextColor: "black"
    property string buttonTextDownColor: "white"

    contentItem: Text {
        id: btnText
        text: btnIconCode
        font.pixelSize: btnIconPixelSize
        font.letterSpacing: btnIconLetterSpacingSize
        color: buttonTextColor//rootBtn.hovered ? buttonTextDownColor :  buttonTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: rootBtn.hovered ? 0.9 : 0.05
        anchors.fill: btnText
    }
}
