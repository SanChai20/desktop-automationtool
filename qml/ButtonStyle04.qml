import QtQuick 2.0
import QtQuick.Controls 2.5
import "../js/globalConfiguration.js" as GlobalConfig

Button
{
    id: rootBtn
    property string btnIconCode: ""
    property string btnIconFamily: ""
    property real btnIconPixelSize: 15

    property string buttonTextDownColor: GlobalConfig.buttonStyle02_IconDownColor
    property string buttonTextColor: GlobalConfig.buttonStyle02_IconColor

    contentItem: Text {
        id: btnText
        text: btnIconCode
        font.family: btnIconFamily
        font.pixelSize: rootBtn.hovered ? btnIconPixelSize + 4 : btnIconPixelSize
        opacity: enabled ? 1.0 : 0.3
        color: rootBtn.down ? buttonTextDownColor : buttonTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: 0.0
        anchors.fill: btnText
    }
}
