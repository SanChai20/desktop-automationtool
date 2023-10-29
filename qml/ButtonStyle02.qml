import QtQuick 2.0
import QtQuick.Controls 2.5
import "../js/globalConfiguration.js" as GlobalConfig

Button
{
    id: rootBtn
    property string btnIconCode: ""
    property string btnIconFamily: ""
    property string btnIconTip: ""
    property real btnIconPixelSize: 15

    contentItem: Text {
        text: btnIconCode
        font.family: btnIconFamily
        font.pixelSize: rootBtn.hovered ? btnIconPixelSize + 4 : btnIconPixelSize
        opacity: enabled ? 1.0 : 0.3
        color: rootBtn.down ? GlobalConfig.buttonStyle02_IconDownColor : GlobalConfig.buttonStyle02_IconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: rootBtn.hovered ? 1 : 0.0
        border.color: rootBtn.down ? GlobalConfig.buttonStyle02_BorderDownColor : GlobalConfig.buttonStyle02_BorderColor
        border.width: 1
        radius: 2
        color: rootBtn.down ? GlobalConfig.buttonStyle02_BackGroundDownColor : GlobalConfig.buttonStyle02_BackGroundColor
    }

    hoverEnabled: true
    ToolTip.visible: hovered
    ToolTip.text: qsTr(btnIconTip)
}
