import QtQuick 2.0
import QtQuick.Controls 2.5
import "globalConfiguration.js" as GlobalConfig

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
        font.pixelSize: btnIconPixelSize
        opacity: enabled ? 1.0 : 0.3
        color: rootBtn.down ? GlobalConfig.buttonStyle01_IconDownColor : GlobalConfig.buttonStyle01_IconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: rootBtn.hovered ? 1 : 0.0
        border.color: rootBtn.down ? GlobalConfig.buttonStyle01_BorderDownColor : GlobalConfig.buttonStyle01_BorderColor
        border.width: 1
        radius: 2
        color: rootBtn.down ? GlobalConfig.buttonStyle01_BackGroundDownColor : GlobalConfig.buttonStyle01_BackGroundColor
    }

    hoverEnabled: true
    ToolTip.visible: hovered
    ToolTip.text: qsTr(btnIconTip)
}
