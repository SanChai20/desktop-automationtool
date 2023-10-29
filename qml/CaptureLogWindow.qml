import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../js/globalConfiguration.js" as GlobalConfig

Rectangle
{
    id: backgroundRect

    property string backColor: "#101010"
    property bool isShowDebugWindow : false
    property bool isDebugEventTriggered : false
    property bool isCaptured : false

    property alias logList : logListModel
    property alias logCurrentIndex: listView.currentIndex

    property real wrapperWidth
    property real edgeWidth: 1
    property real logBorderWidth: 5
    property string captureButtonText: ""

    signal captureButtonClicked();

    function disableInteraction()
    {
        right01_CaptureBtn.enabled = false
    }

    function enableInteraction()
    {
        right01_CaptureBtn.enabled = true
    }

    function configurePreviewImage(imgSource)
    {
        if(previewImg.source == imgSource)
        {
            var oldSource = previewImg.source
            previewImg.source = ""
            previewImg.source = oldSource
        }
        else
            previewImg.source = imgSource;

        right01_CaptureBtn.buttonTextColor = "red"
        right01_CaptureBtn.btnIconTip = GlobalConfig.captureButtonIconTip2
    }

    color: GlobalConfig.logWindowEdgeBorderColor
    height: GlobalConfig.captureImagePreviewSize + edgeWidth * 2
    width: wrapperWidth

    Rectangle
    {
        id: innerRect
        color: backColor
        anchors.centerIn: parent
        width: wrapperWidth - edgeWidth * 2
        height: GlobalConfig.captureImagePreviewSize

        //PREVIEW WINDOW
        Rectangle
        {
            id: previewImgRect
            anchors.right: innerRect.right
            anchors.top: innerRect.top
            anchors.bottom: innerRect.bottom

            color: backColor
            width: GlobalConfig.captureImagePreviewSize

            BorderImage
            {
                id: previewImg
                anchors.centerIn: parent
                asynchronous: true
                cache: false
                width: sourceSize.width > sourceSize.height ? GlobalConfig.captureImagePreviewSize : GlobalConfig.captureImagePreviewSize * (sourceSize.width / sourceSize.height)
                height: sourceSize.height > sourceSize.width ? GlobalConfig.captureImagePreviewSize : GlobalConfig.captureImagePreviewSize * (sourceSize.height / sourceSize.width)
            }

            ButtonStyle03
            {
                id: right01_CaptureBtn

                anchors.right: parent.right
                anchors.rightMargin: GlobalConfig.buttonGapMarginRight

                anchors.bottom: parent.bottom
                anchors.bottomMargin: GlobalConfig.buttonGapMarginBottom

                btnIconCode: GlobalConfig.captureButtonIcon
                btnIconFamily: GlobalConfig.solidFontFamily
                btnIconTip: GlobalConfig.captureButtonIconTip
                btnIconPixelSize: GlobalConfig.buttonTextPixelSize

                buttonTextDownColor: "grey"
                buttonTextColor: "green"

                onClicked:
                {
                    captureButtonClicked();
                    isCaptured = true;
                }
            }

            ButtonStyle03
            {
                id: right02_CleanLogBtn

                anchors.right: right01_CaptureBtn.left
                anchors.rightMargin: GlobalConfig.buttonGapMarginRight

                anchors.bottom: parent.bottom
                anchors.bottomMargin: GlobalConfig.buttonGapMarginBottom

                btnIconCode: GlobalConfig.cleanButtonIcon
                btnIconFamily: GlobalConfig.regularFontFamily
                btnIconTip: GlobalConfig.cleanButtonIconTip
                btnIconPixelSize: GlobalConfig.buttonTextPixelSize

                enabled: logListModel.count >= 1

                onClicked:
                {
                    logListModel.clear();
                    //captureButtonClicked();
                }
            }

            ButtonStyle03
            {
                id: right03_ShowDebugBtn

                anchors.right: right02_CleanLogBtn.left
                anchors.rightMargin: GlobalConfig.buttonGapMarginRight

                anchors.bottom: parent.bottom
                anchors.bottomMargin: GlobalConfig.buttonGapMarginBottom

                btnIconCode: isShowDebugWindow ? GlobalConfig.closeDebugCoverIcon : GlobalConfig.showDebugCoverIcon
                btnIconFamily: GlobalConfig.regularFontFamily
                btnIconTip: isShowDebugWindow ? GlobalConfig.closeDebugCoverIconTip : GlobalConfig.showDebugCoverIconTip
                btnIconPixelSize: GlobalConfig.buttonTextPixelSize
                enabled: isCaptured

                onClicked:
                {
                    isShowDebugWindow = !isShowDebugWindow
                    if(!isShowDebugWindow)
                    {
                        isDebugEventTriggered = false
                    }
                }
            }
        }

        //LOG WINDOW
        Rectangle
        {
            id: logWindow
            anchors
            {
                top: parent.top
                topMargin: logBorderWidth
                bottom: innerRect.bottom
                bottomMargin: logBorderWidth
                left: innerRect.left
                leftMargin: logBorderWidth
                right: previewImgRect.left
                rightMargin: logBorderWidth
            }

            color: backColor

            ListModel
            {
                id: logListModel
            }

            Component
            {
                id: delegateComp
                RowLayout
                {
                    width: parent.width
                    TextEdit {
                        id: logText
                        readOnly: true
                        selectByMouse: true
                        font.pixelSize: 14
                        text: logInfo
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                        color: GlobalConfig.capturelogwindowLogFontColor
                        selectionColor : GlobalConfig.capturelogwindowLogSelectedColor
                    }
                }
            }

            ListView
            {
                id: listView
                clip: true
                model: logListModel
                anchors.fill: parent
                orientation: ListView.Vertical
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                delegate: delegateComp
            }
        }
    }
}

