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

    property string btnDownColor : GlobalConfig.buttonStyle01_IconDownColor

    contentItem: Text {
        id: btnTetx
        text: btnIconCode
        font.family: btnIconFamily
        font.pixelSize: btnIconPixelSize
        opacity: enabled ? 1.0 : 0.3
        color: rootBtn.down ? btnDownColor : GlobalConfig.buttonStyle01_IconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    background: Rectangle {
        opacity: 0.0
        anchors.fill: btnTetx
    }
    hoverEnabled: true
    ToolTip.visible: hovered
    ToolTip.text: qsTr(btnIconTip)
}
