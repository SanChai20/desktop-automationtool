import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQml 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import GlobalSettingsProfile 1.0
import "../js/globalConfiguration.js" as GlobalConfig

Item {

    id: scriptMain

    property int windowContentWidth
    property int windowContentHeight

    function closeAllPopupRaw()
    {
        advancedSettingsPopup.close()
        developerInfoPopup.close()
    }

    function closeAllPopupShell(input)
    {
        closeAllPopupRaw()
    }

    ListModel {
        id: windowTitleModel
        ListElement {
            title: "New Window"
            closeButtonEnabled: true
        }
    }

    Rectangle
    {
        id: splitLine
        x: 0
        y: bar.contentHeight
        width: windowContentWidth
        height: 1
        color: "grey"
    }


    ListView {

        id: listView
        interactive: false
        model: windowTitleModel

        x: 0
        y: bar.contentHeight + splitLine.height
        width: windowContentWidth
        height: windowContentHeight - bar.contentHeight - splitLine.height

        delegate: Component {
            
            ScriptMainInterface
            {
                id: scriptMainInterface
            
                visible: {
                    var isCurrent = (index == listView.currentIndex);
                    return isCurrent
                }
                anchors.fill: parent
                interfaceName: index.toString()
                currentBarIndex: bar.currentIndex
                logWindowHeight: windowContentHeight - bar.contentHeight - splitLine.height
                rootWindowHeight: windowContentHeight
                rootWindowWidth : windowContentWidth
                onWindowNameCaptured:
                {
                    console.log(windowName)
                    title = windowName
                }

                onEnableCloseBtn:
                {
                    closeButtonEnabled = true
                    console.log("onEnableCloseBtn")
                }

                onDisableCloseBtn:
                {
                    closeButtonEnabled = false
                    console.log("onDisableCloseBtn")
                }
            }
        }
    }

    TabBar {
        id: bar
        width: windowContentWidth - GlobalConfig.addWindowButtonSize - 2 * GlobalConfig.btnGapSize
        contentHeight: GlobalConfig.addWindowButtonSize
        wheelEnabled: true
        focusPolicy: Qt.WheelFocus

        background: Rectangle
        {
            anchors.fill: parent
            color: GlobalConfig.mainBackGroundColor
        }

        onCurrentIndexChanged:
        {
            listView.currentIndex = currentIndex
        }



        Repeater {

            model: windowTitleModel

            TabButton {
                id: tabBtn
                width: 200

                RowLayout
                {
                    id: rootLayout
                    anchors.fill: parent

                    Rectangle
                    {
                        id: empty1
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        Layout.minimumWidth: 5
                        Layout.maximumWidth: 5
                        Layout.minimumHeight: 1
                        Layout.maximumHeight: 1

                        opacity: 0.0
                    }

                    Text {
                        id: windowHintIcon
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        text: GlobalConfig.windowHintIcon
                        color: "grey"
                        font.pixelSize: GlobalConfig.timelineButtonFontPixelSize - 4
                        font.family: GlobalConfig.solidFontFamily
                    }

                    Text {
                        id: subWindowTitle
                        Layout.preferredWidth: 125
                        elide: Text.ElideRight
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        text: title
                        color: "white"
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                    }

                    Rectangle
                    {
                        id: empty2
                        Layout.fillWidth: true
                    }

                    Rectangle
                    {
                        id: closeWindowButtonContainer
                        Layout.alignment: Qt.AlignLeft
                        Layout.minimumWidth: GlobalConfig.closeSubLevelButtonSize
                        Layout.maximumWidth: GlobalConfig.closeSubLevelButtonSize
                        Layout.preferredWidth: GlobalConfig.closeSubLevelButtonSize
                        Layout.preferredHeight: GlobalConfig.closeSubLevelButtonSize
                        color: "transparent"

                        ButtonStyle04
                        {
                            id: closeWindowBtn
                            anchors.centerIn: parent
                            btnIconCode: GlobalConfig.closeWindowIcon
                            btnIconFamily: GlobalConfig.regularFontFamily
                            //btnIconTip: GlobalConfig.closeWindowIconTip
                            btnIconPixelSize: GlobalConfig.timelineButtonFontPixelSize
                            //hoverActive: false
                            enabled:
                            {
                                console.log(index + " closeButtonEnabled: " + closeButtonEnabled)
                                return windowTitleModel.count > 1 && closeButtonEnabled
                            }

                            onClicked:
                            {
                                enabled = false
                                windowTitleModel.remove(index)
                            }
                        }

                    }



                }
            }
        }
    }

    Rectangle
    {
        x: windowContentWidth - GlobalConfig.addWindowButtonSize - 2 * GlobalConfig.btnGapSize
        y: 0
        width: GlobalConfig.btnGapSize
        height: GlobalConfig.addWindowButtonSize
        color: GlobalConfig.mainBackGroundColor
    }

    Rectangle
    {
        x: windowContentWidth - GlobalConfig.addWindowButtonSize - GlobalConfig.btnGapSize
        y: 0
        width: GlobalConfig.addWindowButtonSize
        height: GlobalConfig.addWindowButtonSize
        color: GlobalConfig.mainBackGroundColor
        ComboBox
        {
            id: popupComboBox
            //enabled: false
            anchors.fill: parent
            model: ListModel {
                id: cbItems
                ListElement { textFamily: "Font Awesome 6 Free Solid"; textSource: "\uf5fd"; hintTip: "Add New Window" }
                ListElement { textFamily: "Font Awesome 6 Free Solid"; textSource: "\uf098"; hintTip: "Contact Developer"}
                ListElement { textFamily: "Font Awesome 6 Free Solid"; textSource: "\uf085"; hintTip: "Advanced Settings"}
            }

            delegate: ItemDelegate
            {
                id: item
                width: parent.width
                height: parent.width

                Text
                {
                    text: textSource
                    font.family: textFamily
                    font.pixelSize: hovered ? GlobalConfig.comboButtonHoveredPixelSize : GlobalConfig.comboButtonPixelSize
                    anchors.centerIn: parent
                    color: GlobalConfig.buttonStyle01_IconColor
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr(hintTip)

                onClicked:
                {
                    if(index == 0)//addnewWindow
                    {
                        windowTitleModel.append({title: "New Window", closeButtonEnabled : true})
                        bar.currentIndex = windowTitleModel.count - 1
                    }
                    else if(index == 1)//contact developer
                    {
                        developerInfoPopup.open()
                    }
                    else if(index == 2)
                    {
                        globalSettingsProfile.show()
                        advancedSettingsPopup.open()
                    }
                }


            }
        }

    }

    Rectangle
    {
        x: windowContentWidth - GlobalConfig.btnGapSize
        y: 0
        width: GlobalConfig.btnGapSize
        height: GlobalConfig.addWindowButtonSize
        color: GlobalConfig.mainBackGroundColor
    }

    DeveloperContactPopup
    {
        id: developerInfoPopup
        x: windowContentWidth * 0.1
        y: windowContentHeight * 0.2
        width: windowContentWidth * 0.8
        height: windowContentHeight * 0.2
    }

    GlobalSettingsProfile
    {
        id: globalSettingsProfile
        Component.onCompleted:
        {
            showSettings.connect(advancedSettingsPopup.showSettings)
        }
    }

    AdvancedSettingsPopup
    {
        id: advancedSettingsPopup
        x: windowContentWidth * 0.1
        y: windowContentHeight * 0.15
        width: windowContentWidth * 0.8
        height: windowContentHeight * 0.7

        Component.onCompleted:
        {
            matchTypeChanged.connect(globalSettingsProfile.switchMatchType)
            detectTypeChanged.connect(globalSettingsProfile.switchDetectType)
            policyTypeChanged.connect(globalSettingsProfile.switchPolicyType)
            languageTypeChanged.connect(globalSettingsProfile.switchLanguageType)
            kNNRatioChanged.connect(globalSettingsProfile.changekNNRatio)
            closed.connect(globalSettingsProfile.flip)
            //opened.connect(globalSettingsProfile.show)
        }
    }
}
