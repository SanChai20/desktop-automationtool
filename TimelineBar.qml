import QtQuick 2.0
import QtQml.Models 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "globalConfiguration.js" as GlobalConfig

Rectangle {

    id: root

    property string mainBaseColor: "#303030"

    signal updateRatioThreshold(var threshold);

    signal startRun();
    signal stopRun();

    signal checkOffAllLoop;
    signal checkOnAllLoop;
    signal checkCustomLoop;

    signal exportButtonClicked();
    signal importButtonClicked();

   //signal detectTypeSelected(var index);
   //signal matchTypeSelected(var index);

    function informStopButtonEnabled()
    {
        playBtn.inRunning = true;
        playBtn.enabled = true;
        console.log("informStopButtonEnabled")
    }

    function informRunButtonEnabled()
    {
        playBtn.inRunning = false;
        playBtn.enabled = true;
        console.log("informRunButtonEnabled")
    }

    color: mainBaseColor

    Rectangle
    {
        id: splitLine

        color: "grey"
        width: root.width
        height: 1


    }


    RowLayout
    {
        width: root.width
        spacing: 5

        Rectangle
        {
            id: leftEmpty
            Layout.fillHeight: true
            width: 10
            Layout.alignment: Qt.AlignLeft
            color: mainBaseColor
        }

        ButtonStyle02
        {
            id: importBtn

            Layout.minimumWidth: root.height
            Layout.maximumWidth:  root.height
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
            Layout.alignment: Qt.AlignRight

            btnIconCode: GlobalConfig.importIcon
            btnIconFamily: GlobalConfig.solidFontFamily
            btnIconTip: GlobalConfig.importIconTip
            btnIconPixelSize: GlobalConfig.timelineButtonFontPixelSize

            enabled: !playBtn.inRunning && playBtn.enabled

            onClicked:
            {
                importButtonClicked();
            }
        }

        ButtonStyle02
        {
            id: exportBtn

            Layout.minimumWidth: root.height
            Layout.maximumWidth:  root.height
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
            Layout.alignment: Qt.AlignRight

            btnIconCode: GlobalConfig.exportIcon
            btnIconFamily: GlobalConfig.solidFontFamily
            btnIconTip: GlobalConfig.exportIconTip
            btnIconPixelSize: GlobalConfig.timelineButtonFontPixelSize

            onClicked:
            {
                exportButtonClicked();
            }
        }


        Rectangle
        {
            id: empty
            Layout.fillWidth: true
            color: mainBaseColor
        }

        ComboBox {

            Layout.minimumWidth: 150
            Layout.maximumWidth:  200
            Layout.preferredWidth: 180
            Layout.preferredHeight: root.height
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            currentIndex: 1
            model: ListModel {
                id: cbItems
                ListElement { textSource: "No loop execution"; }
                ListElement { textSource: "All loop execution"; }
                ListElement { textSource: "Custom loop execution"; }
            }

            delegate: ItemDelegate
            {
                text: textSource
                font.pixelSize: GlobalConfig.timelineFontPixelSize
                width: parent.width

                onClicked:
                {
                    if(index == 0)
                    {
                        checkOffAllLoop();
                    }
                    else if(index == 1)
                    {
                        checkOnAllLoop();
                    }
                    else
                    {
                        checkCustomLoop();
                    }
                }
            }
            font.pixelSize: GlobalConfig.timelineFontPixelSize
        }

        ButtonStyle01
        {
            id: playBtn

            property bool inRunning: false
            //enabled: false
            //text: "播放"
            Layout.minimumWidth: root.height
            Layout.maximumWidth:  root.height
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
            Layout.alignment: Qt.AlignRight

            btnIconCode: inRunning ? GlobalConfig.stopRunningIcon : GlobalConfig.beginRunningIcon
            btnIconFamily: GlobalConfig.solidFontFamily
            btnIconTip: inRunning ? GlobalConfig.stopRunningIconTip : GlobalConfig.beginRunningIconTip
            btnIconPixelSize: GlobalConfig.timelineButtonFontPixelSize

            onClicked:
            {
                //inRunning = !inRunning
                if(!inRunning)
                {
                    enabled = false;
                    startRun();
                }
                else
                {
                    enabled = false;
                    stopRun();
                }
            }
        }

        Rectangle
        {
            id: rightEmpty
            Layout.fillHeight: true
            width: 10
            Layout.alignment: Qt.AlignRight
            color: mainBaseColor
        }
    }
}
