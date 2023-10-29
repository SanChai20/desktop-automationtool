import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.3
import QtQml 2.3
import QtQuick.Layouts 1.3
import "globalConfiguration.js" as GlobalConfig

ApplicationWindow {
    id: rootWindow
    visible: true
    title: qsTr("JeeJee")
    width: GlobalConfig.applicationWindowWidth
    height: GlobalConfig.applicationWindowHeight
    minimumWidth: GlobalConfig.applicationWindowMinimumWidth
    minimumHeight: GlobalConfig.applicationWindowMinimumHeight
    maximumWidth: GlobalConfig.applicationWindowMaximumWidth
    maximumHeight: GlobalConfig.applicationWindowMaximumHeight
    //flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint
    Material.accent: Material.Pink
    //Material.background: Material.BlueGrey
    onClosing:
    {

    }

    ScriptMainContent
    {
        id: mainContent
        x: 0
        y: 0
        windowContentWidth: rootWindow.width
        windowContentHeight: rootWindow.height
    }
}
