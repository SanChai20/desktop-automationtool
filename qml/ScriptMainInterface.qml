import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQml 2.3
//import GameScriptRunSystem 1.0
import GameScriptRunInstance 1.0
import CoverTransparentWindow 1.0
import CoverShowWindow 1.0
import Qt.labs.platform 1.0
import "../js/globalConfiguration.js" as GlobalConfig


Rectangle
{
    id: mainInterface

    property int logWindowHeight: 10
    property int rootWindowWidth
    property int rootWindowHeight
    property int timelineBarHeight: 40
    property string interfaceName: ""
    property int currentBarIndex: 0

    signal windowNameCaptured(var windowName)

    signal disableCloseBtn()
    signal enableCloseBtn()

    color: GlobalConfig.mainBackGroundColor

    function disableAllInteraction()
    {
        captureLogWindow.disableInteraction()
        commandWindow.disableInteraction()
    }

    CaptureLogWindow
    {
        id: captureLogWindow
        wrapperWidth: rootWindowWidth

        anchors.top: mainInterface.top
        anchors.left: mainInterface.left
        anchors.right: mainInterface.right

        captureButtonText: interfaceName
        onCaptureButtonClicked:
        {
            captureTimer.running = true
        }
    }

    CoverShowWindow
    {
        id: readonlyCoverWindow
        visible: captureLogWindow.isShowDebugWindow && captureLogWindow.isDebugEventTriggered
        color: "#00E2FF"

        function setRect(x, y, width, height, clickX, clickY)
        {
            debugImageRect.x = x
            debugImageRect.y = y
            debugImageRect.width = width
            debugImageRect.height = height
            clickRect.x = clickX
            clickRect.y = clickY
        }

        Rectangle
        {
            id: debugImageRect
            color: "red"
            opacity: 1.0

            Rectangle
            {
                id: innerRect
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: parent.left

                anchors.topMargin: 4
                anchors.bottomMargin: 4
                anchors.rightMargin: 4
                anchors.leftMargin: 4
                color: "#00E2FF"
            }

            Text {
                id: clickRect
                text: GlobalConfig.commandPointIcon
                font.family: GlobalConfig.regularFontFamily
                font.pixelSize: GlobalConfig.commandPointPixelSize
                color: "red"
            }
        }
    }

    Covertransparentwindow
    {
        id: selectCoverWindow
        flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
        color: "#00E2FF"

        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                captureTimer.running = false
                scriptSystem.saveImageAndGeneratePreview(currentBarIndex);
                selectCoverWindow.visible = false
            }
        }
    }

    Timer
    {
        id : captureTimer
        interval: 200; running: false; repeat: true
        onTriggered:
        {
            scriptSystem.updateRectCapture(selectCoverWindow.getWindow());
        }
    }

    CommandStepWindow
    {
        id: commandWindow
        anchors.left : captureLogWindow.left
        anchors.right: captureLogWindow.right
        anchors.top: captureLogWindow.bottom
        anchors.topMargin: 4

        height: logWindowHeight - captureLogWindow.height - timelineBarHeight - 4
    }

    FileDialog
    {
        id: importDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Command Files (*.command)"]

        onAccepted:
        {
            console.log(importDialog.file)
            var cleanPath = GlobalConfig.getAbsolutePath(importDialog.file)
            var count = scriptSystem.unpackCommands(cleanPath);
            if(count != 0)
            {
                console.log("count is: " ,count)
                if(commandWindow.commandList.count > count)
                {
                    console.log("commandWindow.commandList.count is: " ,commandWindow.commandList.count)
                    for(var index = commandWindow.commandList.count - 1; index >= count; index--)
                    {
                        console.log("remove index is: " , index)
                        commandWindow.removeElementAt(index);
                    }
                }

                for(var index = 0; index < count; ++index)
                {
                    commandWindow.commandList.set(index,
                    {
                        "delayTime": scriptSystem.queryDelayTimeAt(index),
                        "backColor": GlobalConfig.mainBackGroundColor,
                        "objectImagePath" : scriptSystem.queryImagePathAt(index),
                        "borderImagePath" : scriptSystem.queryBorderImagePathAt(index),
                        "addButtonEnabled" : true,
                        "trashButtonEnabled" : true,
                        "objectImageEnabled" : true,
                        "commandRecordEnabled" : true,
                        "swapFunctionEnabled" : true,
                        "delayTimeTextFieldEnabled" : true,
                        "checkBoxEnabled" : true,
                        "clickComboEnabled" : true,
                        "clickRateX" : scriptSystem.queryClickRateXAt(index),
                        "clickRateY" : scriptSystem.queryClickRateYAt(index),
                        "clickEvent" : scriptSystem.queryClickEventAt(index),
                        "inloop" : scriptSystem.queryLoopAt(index)
                    })
                }
                scriptSystem.clearImportCache();
            }
        }
    }

    FileDialog
    {
        id: exportDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Command Files (*.command)"]
        currentFile: "newCommand.command"

        onAccepted:
        {  
            var cleanPath = GlobalConfig.getAbsolutePath(exportDialog.file)
            console.log(cleanPath)
            for(var index = 0; index < commandWindow.commandList.count; ++index)
            {
                scriptSystem.appendCommand(commandWindow.commandList.get(index).delayTime,
                                            commandWindow.commandList.get(index).clickRateX,
                                            commandWindow.commandList.get(index).clickRateY,
                                            commandWindow.commandList.get(index).inloop,
                                            commandWindow.commandList.get(index).objectImagePath,
                                            commandWindow.commandList.get(index).borderImagePath,
                                            commandWindow.commandList.get(index).clickEvent)
            }
            scriptSystem.packCommands(cleanPath)
        }
    }

    TimelineBar
    {
        id: timelineBar
        anchors.right: commandWindow.right
        anchors.left: commandWindow.left
        anchors.top: commandWindow.bottom
        height: timelineBarHeight
        mainBaseColor: GlobalConfig.mainBackGroundColor

        onStartRun:
        {
            console.log("onStartRun")
            mainInterface.disableCloseBtn()
            disableAllInteraction()
            scriptSystem.resetTask();
            for(var index = 0; index < commandWindow.commandList.count; ++index)
            {
                scriptSystem.appendCommand(commandWindow.commandList.get(index).delayTime,
                                           commandWindow.commandList.get(index).clickRateX,
                                           commandWindow.commandList.get(index).clickRateY,
                                           commandWindow.commandList.get(index).inloop,
                                           commandWindow.commandList.get(index).clickEvent)
                scriptSystem.appendCommand(commandWindow.commandList.get(index).objectImagePath);
            }
            scriptSystem.run();
        }

        Component.onCompleted:
        {
            stopRun.connect(scriptSystem.stop)
            exportButtonClicked.connect(exportDialog.open)
            importButtonClicked.connect(importDialog.open)
            checkCustomLoop.connect(commandWindow.enableLoopControl)
            checkOffAllLoop.connect(commandWindow.switchoffLoopAndDisabled)
            checkOnAllLoop.connect(commandWindow.switchonLoopAndDisabled)
            //detectTypeSelected.connect(scriptSystem.setDetectType)
            //matchTypeSelected.connect(scriptSystem.setMatchType)
            //updateRatioThreshold.connect(scriptSystem.setkNNRatioThreshold)
        }
    }

    ListModel
    {
         id: tempModel
    }

    GameScriptRunInstance
    {
        id: scriptSystem

        onDebugMatch:
        {
            captureLogWindow.isDebugEventTriggered = captureLogWindow.isShowDebugWindow
            readonlyCoverWindow.setRect(x,y, width, height, clickX, clickY)
            readonlyCoverWindow.x = rectX;
            readonlyCoverWindow.y = rectY;
            readonlyCoverWindow.width = rectWidth;
            readonlyCoverWindow.height = rectHeight;
        }

        Component.onCompleted:
        {
            runningProceed.connect(timelineBar.informStopButtonEnabled)
            runningStopped.connect(timelineBar.informRunButtonEnabled)
            runningStopped.connect(captureLogWindow.enableInteraction)
            runningStopped.connect(commandWindow.enableInteraction)
            runningStopped.connect(mainInterface.enableCloseBtn)
            windowRectUpdated.connect(selectCoverWindow.updateWindowRectangle)
            runningIndexUpdated.connect(commandWindow.setRunningIndex)
        }

        onPreviewImageConfigured:
        {
            windowNameCaptured(windowTitle);
            captureLogWindow.configurePreviewImage(GlobalConfig.captureImagePrefix + imagePath)
        }

        onLogUpdated:
        {
            captureLogWindow.logList.append({"logInfo": log});
            var countLimit = 80;
            if(captureLogWindow.logList.count >= countLimit)//clear 40 when exceed 80
            {
                tempModel.clear()
                for(var index = (countLimit / 2); index < countLimit; ++index)
                    tempModel.append(captureLogWindow.logList.get(index))

                captureLogWindow.logList.clear()
                
                for(var index = 0; index < tempModel.count; ++index)
                    captureLogWindow.logList.append(tempModel.get(index))
            }
            captureLogWindow.logCurrentIndex = captureLogWindow.logList.count - 1;
        }
    }
}



