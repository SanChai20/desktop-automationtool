import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import "../js/globalConfiguration.js" as GlobalConfig

Rectangle {

    id: root
    property alias commandList : commandListModel
    property real runningIndex : 0

    function setRunningIndex(index)
    {
        runningIndex = index
    }

    function disableInteraction()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "addButtonEnabled", false);
            commandListModel.setProperty(modelIndex, "trashButtonEnabled", false);
            commandListModel.setProperty(modelIndex, "objectImageEnabled", false);
            commandListModel.setProperty(modelIndex, "commandRecordEnabled", false);
            commandListModel.setProperty(modelIndex, "swapFunctionEnabled", false);
            commandListModel.setProperty(modelIndex, "delayTimeTextFieldEnabled", false);
            commandListModel.setProperty(modelIndex, "checkBoxEnabled", false);
            commandListModel.setProperty(modelIndex, "clickComboEnabled", false);

        }
    }

    function enableInteraction()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "addButtonEnabled", true);
            commandListModel.setProperty(modelIndex, "trashButtonEnabled", true);
            commandListModel.setProperty(modelIndex, "objectImageEnabled", true);
            commandListModel.setProperty(modelIndex, "commandRecordEnabled", true);
            commandListModel.setProperty(modelIndex, "swapFunctionEnabled", true);
            commandListModel.setProperty(modelIndex, "delayTimeTextFieldEnabled", true);
            commandListModel.setProperty(modelIndex, "checkBoxEnabled", true);
            commandListModel.setProperty(modelIndex, "clickComboEnabled", true);
        }
    }

    function switchonLoopAndDisabled()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "inloop", true);
            commandListModel.setProperty(modelIndex, "checkBoxEnabled", false);
        }
    }

    function switchoffLoopAndDisabled()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "inloop", false);
            commandListModel.setProperty(modelIndex, "checkBoxEnabled", false);
        }
    }

    function enableLoopControl()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "checkBoxEnabled", true);
        }
    }

    color: GlobalConfig.mainBackGroundColor

    Component.onCompleted:
    {
        commandListModel.append({"delayTime": 0.0,
                                "backColor": GlobalConfig.mainBackGroundColor,
                                "objectImagePath" : "",
                                "borderImagePath" : "",
                                "addButtonEnabled" : true,
                                "trashButtonEnabled" : true,
                                "objectImageEnabled" : true,
                                "commandRecordEnabled" : true,
                                "swapFunctionEnabled" : true,
                                "delayTimeTextFieldEnabled" : true,
                                "checkBoxEnabled" : true,
                                "clickComboEnabled" : true,
                                "clickRateX" : 0.0,
                                "clickRateY" : 0.0,
                                "clickEvent" : 0,
                                "inloop" : false})
    }

    ListModel
    {
        id: commandListModel
    }

    function highlightElement(elementIndex)
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "backColor", GlobalConfig.mainBackGroundColor);
        }
        commandListModel.setProperty(elementIndex, "backColor", GlobalConfig.commandSelectedColor);
    }

    function insertElementAfter(elementIndex)
    {
        commandListModel.insert(elementIndex + 1, {"delayTime": 0.0,
                                                    "backColor" : GlobalConfig.mainBackGroundColor,
                                                    "objectImagePath" : "",
                                                    "borderImagePath" : "",
                                                    "addButtonEnabled" : true,
                                                    "trashButtonEnabled" : true,
                                                    "objectImageEnabled" : true,
                                                    "commandRecordEnabled" : true,
                                                    "swapFunctionEnabled" : true,
                                                    "delayTimeTextFieldEnabled" : true,
                                                    "checkBoxEnabled" : true,
                                                    "clickComboEnabled" : true,
                                                    "clickRateX" : 0.0,
                                                    "clickRateY" : 0.0,
                                                    "clickEvent" : 0,
                                                    "inloop" : false})
    }

    function removeElementAt(elementIndex)
    {
        commandListModel.remove(elementIndex)
    }

    function enableAllAddButtons()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "addButtonEnabled", true);
        }
    }

    function disableAllAddButtons()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "addButtonEnabled", false);
        }
    }

    function enableAllTrashButtons()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "trashButtonEnabled", true);
        }
    }

    function disableAllTrashButtons()
    {
        for(var modelIndex = 0; modelIndex < commandListModel.count; ++modelIndex)
        {
            commandListModel.setProperty(modelIndex, "trashButtonEnabled", false);
        }
    }

    Component
    {
        id: delegateComp

        Rectangle
        {
            id: wrapperRect
            width: parent.width - 15
            height: GlobalConfig.commandLineHeight
            color: backColor

            MouseArea
            {
                id: mouseArea
                anchors.fill: wrapperRect

                onClicked:
                {
                    forceActiveFocus();
                    highlightElement(index);
                }
            }

            RowLayout
            {

                width: parent.width
                spacing: 5

                Rectangle
                {
                    id: empty4
                    Layout.alignment: Qt.AlignLeft
                    Layout.minimumWidth: 5
                    Layout.maximumWidth:  5
                    Layout.preferredWidth: 5
                    Layout.preferredHeight: 1
                    color: backColor
                }

                CheckBox
                {
                    id: inloopChecker
                    Layout.alignment: Qt.AlignLeft

                    Layout.minimumWidth: GlobalConfig.commandLineHeight
                    Layout.maximumWidth:  GlobalConfig.commandLineHeight
                    Layout.preferredWidth: GlobalConfig.commandLineHeight
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    enabled: checkBoxEnabled

                    checked: inloop
                    nextCheckState: function() {
                        highlightElement(index);
                        if (checkState === Qt.Checked)
                        {
                            console.log("unchecked!");
                            inloop = false
                            return Qt.Unchecked
                        }
                        else
                        {
                            console.log("checked!");
                            inloop = true
                            return Qt.Checked
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr(GlobalConfig.checkBoxTip)
                }

                Rectangle
                {
                    id: indexNumberHolder
                    Layout.minimumWidth: 20
                    Layout.maximumWidth:  25
                    Layout.preferredWidth: 25
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    Text {
                        id: indexNumber
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: index
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }

                Rectangle
                {
                    id: switchRect
                    Layout.minimumWidth: GlobalConfig.commandLineHeight
                    Layout.maximumWidth: GlobalConfig.commandLineHeight
                    Layout.preferredWidth: GlobalConfig.commandLineHeight
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    ButtonStyle02
                    {
                        id: switchBtn
                        anchors.centerIn: switchRect
                        btnIconCode: swapFunctionEnabled ? GlobalConfig.switchIcon : GlobalConfig.rightArrowIcon
                        btnIconFamily: GlobalConfig.solidFontFamily
                        btnIconTip: swapFunctionEnabled ? GlobalConfig.switchIconTip : GlobalConfig.rightArrowIconTip
                        btnIconPixelSize: GlobalConfig.commandButtonFontPixelSize
                        visible: swapFunctionEnabled ? true : (runningIndex == index)
                    }

                    MouseArea
                    {
                        id: mouseArea01
                        anchors.fill: parent
                        enabled: swapFunctionEnabled
                        onPressed:
                        {
                            forceActiveFocus();
                            highlightElement(index);
                        }

                        preventStealing: true
                        onPositionChanged:
                        {
                            var otherIndex = listView.indexAt(mouseArea01.mouseX + wrapperRect.x, mouseArea01.mouseY + wrapperRect.y);
                            if(otherIndex !== -1 && pressed)
                            {
                                if(index !== otherIndex)
                                {
                                    commandListModel.move(index, otherIndex, 1);
                                }
                            }
                        }
                    }
                }

                Rectangle
                {
                    id: contentText01Holder
                    Layout.minimumWidth: contentText01.contentWidth
                    Layout.maximumWidth: contentText01.contentWidth
                    Layout.preferredWidth: contentText01.contentWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor
                    Text {
                        id: contentText01
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: "If image"
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }

                Rectangle
                {
                    id: matchItemsHolder
                    Layout.minimumWidth: GlobalConfig.commandLineHeight
                    Layout.maximumWidth: GlobalConfig.commandLineHeight
                    Layout.preferredWidth: GlobalConfig.commandLineHeight
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft

                    Component.onCompleted:
                    {
                        color = "grey"
                    }

                    Rectangle
                    {
                        id: subMatchHolder
                        color: "black"
                        anchors.centerIn: parent
                        width: GlobalConfig.commandLineHeight - 2
                        height: GlobalConfig.commandLineHeight - 2

                        ImageFileBrowserDialog
                        {
                            id: browserDialog
                            onImgFileSelected:
                            {
                                objectImagePath = absolutePath
                                borderImagePath = file
                            }
                        }

                        BorderImage
                        {
                            id: matchObjectImg
                            width: sourceSize.width > sourceSize.height ? (GlobalConfig.commandLineHeight - 2) : (GlobalConfig.commandLineHeight - 2) * (sourceSize.width / sourceSize.height)
                            height: sourceSize.height > sourceSize.width ? (GlobalConfig.commandLineHeight - 2) : (GlobalConfig.commandLineHeight - 2) * (sourceSize.height / sourceSize.width)
                            anchors.centerIn: parent
                            asynchronous: true
                            cache: false
                            source:  borderImagePath
                            onStatusChanged:
                            {
                                console.log(borderImagePath)
                                if (matchObjectImg.status == Image.Ready)
                                    matchItemsHolder.color = GlobalConfig.commandTextFieldFontColor
                            }
                        }

                        MouseArea
                        {
                            id: mouseArea02
                            anchors.fill: parent
                            enabled: objectImageEnabled
                            onEntered:
                            {
                                ToolTip.visible = true
                                subMatchHolder.width = GlobalConfig.commandLineHeight - 4
                                subMatchHolder.height = GlobalConfig.commandLineHeight - 4
                            }

                            onExited:
                            {
                                ToolTip.visible = false
                                subMatchHolder.width = GlobalConfig.commandLineHeight - 2
                                subMatchHolder.height = GlobalConfig.commandLineHeight - 2
                            }

                            hoverEnabled: true
                            ToolTip.text:  matchObjectImg.source != "" ? qsTr(GlobalConfig.objectImageTip2) : qsTr(GlobalConfig.objectImageTip)
                            ToolTip.visible: false

                            onClicked:
                            {
                                highlightElement(index);
                                browserDialog.open()
                            }
                        }
                    }
                }

                Rectangle
                {
                    id: contentText02Holder
                    Layout.minimumWidth: contentText02.contentWidth
                    Layout.maximumWidth: contentText02.contentWidth
                    Layout.preferredWidth: contentText02.contentWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor
                    Text {
                        id: contentText02
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: "matched:"
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }

                Rectangle
                {
                    id: contentComboHolder
                    Layout.minimumWidth: GlobalConfig.clickComboWidth
                    Layout.maximumWidth: GlobalConfig.clickComboWidth
                    Layout.preferredWidth: GlobalConfig.clickComboWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    ComboBox {
                        id: clickCombo
                        anchors.fill: parent
                        currentIndex: clickEvent
                        enabled: clickComboEnabled
                        model: ListModel {
                            id: clickItems
                            ListElement { textSource: "left click"; }
                            ListElement { textSource: "left double click"; }
                            ListElement { textSource: "right click"; }
                            ListElement { textSource: "right double click"; }
                        }
                        delegate: ItemDelegate
                        {
                            text: textSource
                            font.pixelSize: GlobalConfig.timelineFontPixelSize
                            width: parent.width

                            onClicked:
                            {
                                clickEvent = index;
                                //matchTypeChanged(index);
                            }
                        }
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                    }
                }

                Rectangle
                {
                    id: contentText03Holder
                    Layout.minimumWidth: contentText03.contentWidth
                    Layout.maximumWidth: contentText03.contentWidth
                    Layout.preferredWidth: contentText03.contentWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor
                    Text {
                        id: contentText03
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: "at"
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }

                Rectangle
                {
                    id: recordCommandHolder
                    Layout.minimumWidth: commandText01.contentWidth + 6
                    Layout.maximumWidth: commandText01.contentWidth + 6
                    Layout.preferredWidth: commandText01.contentWidth + 6
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    Label {
                        id: commandText01
                        text: "location"
                        enabled: commandRecordEnabled
                        property string toolTipText: "message"
                        anchors.centerIn: parent
                        ToolTip.text: GlobalConfig.commandPointIconTip
                        ToolTip.visible: ma.containsMouse

                        color: GlobalConfig.commandTextFieldFontColor
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                        MouseArea {
                            id: ma
                            anchors.fill: parent
                            hoverEnabled: true
                            onPressed:
                            {
                                commandText01.color = "black"
                            }
                            onReleased:
                            {
                                commandText01.color = GlobalConfig.commandTextFieldFontColor
                            }
                            onClicked:
                            {
                                highlightElement(index);
                                popup.setImageURL(matchObjectImg.source);
                                popup.setClickPos(clickRateX, clickRateY, index)
                                popup.open()
                            }
                        }
                    }
                }





                Rectangle
                {
                    id: contentText04Holder
                    Layout.minimumWidth: contentText04.contentWidth
                    Layout.maximumWidth: contentText04.contentWidth
                    Layout.preferredWidth: contentText04.contentWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    Text {
                        id: contentText04
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: "after "
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }


                Rectangle
                {
                    id: textField02Holder
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: 80
                    Layout.preferredWidth: 55
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft

                    color: backColor

                    TextField {

                        enabled: delayTimeTextFieldEnabled
                        id: textField02
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: -5
                        anchors.left: parent.left
                        anchors.right: parent.right
                        color: GlobalConfig.commandTextFieldFontColor
                        selectByMouse: true
                        validator: DoubleValidator
                        {
                            bottom: 0;
                            top: 100;
                            decimals: 2;
                            notation: DoubleValidator.StandardNotation
                        }
                        text: delayTime.toFixed(2)
                        placeholderText: qsTr("number")
                        font.pixelSize: 12
                        onEditingFinished:
                        {
                            delayTime = parseFloat(textField02.text)
                        }

                        onPressed:
                        {
                            highlightElement(index);
                        }

                    }
                }


                Rectangle
                {
                    id: contentText05Holder
                    Layout.minimumWidth: contentText05.contentWidth
                    Layout.maximumWidth: contentText05.contentWidth
                    Layout.preferredWidth: contentText05.contentWidth
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    Text {
                        id: contentText05
                        anchors.centerIn: parent
                        color: GlobalConfig.commandFontColor
                        text: "seconds delay"
                        font.pixelSize: GlobalConfig.commandFontPixelSize
                    }
                }

                Rectangle
                {
                    id: empty
                    Layout.fillWidth: true
                }


                Rectangle
                {
                    id: removeRect
                    Layout.minimumWidth: GlobalConfig.commandLineHeight
                    Layout.maximumWidth: GlobalConfig.commandLineHeight
                    Layout.preferredWidth: GlobalConfig.commandLineHeight
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor

                    ButtonStyle02
                    {
                        id: removeBtn
                        anchors.centerIn: removeRect
                        btnIconCode: GlobalConfig.trashIcon
                        btnIconFamily: GlobalConfig.solidFontFamily
                        btnIconTip: GlobalConfig.trashIconTip
                        btnIconPixelSize: GlobalConfig.commandButtonFontPixelSize

                        enabled: trashButtonEnabled
                        visible: commandListModel.count > 1

                        onClicked:
                        {
                            if(index != -1)
                            {
                                enabled = false;
                                removeElementAt(index);
                            }
                        }
                    }
                }

                Rectangle
                {
                    id: addRect
                    Layout.minimumWidth: GlobalConfig.commandLineHeight
                    Layout.maximumWidth: GlobalConfig.commandLineHeight
                    Layout.preferredWidth: GlobalConfig.commandLineHeight
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignLeft
                    color: backColor
                    enabled: addButtonEnabled
                    ButtonStyle02
                    {
                        id: addBtn
                        anchors.centerIn: addRect
                        btnIconCode: GlobalConfig.addIcon
                        btnIconFamily: GlobalConfig.regularFontFamily
                        btnIconTip: GlobalConfig.addIconTip
                        btnIconPixelSize: GlobalConfig.commandButtonFontPixelSize

                        onClicked:
                        {
                            forceActiveFocus();
                            highlightElement(index);
                            insertElementAfter(index);
                        }
                    }
                }

                Rectangle
                {
                    id: emptyRect
                    Layout.minimumWidth: 5
                    Layout.maximumWidth: 5
                    Layout.preferredWidth: 5
                    Layout.preferredHeight: GlobalConfig.commandLineHeight
                    Layout.alignment: Qt.AlignRight
                    color: backColor
                }
            }
        }
    }

    ListView
    {
        id: listView
        clip: true
        spacing: 5
        model: commandListModel
        anchors.fill: parent
        orientation: ListView.Vertical
        snapMode: ListView.NoSnap
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        delegate: delegateComp

        move: Transition
        {
            NumberAnimation{property: "y"; duration: 200}
        }

        moveDisplaced: Transition
        {
            NumberAnimation{property: "y"; duration: 200}
        }

        //add: Transition {
        //    id: addTrans
        //    NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
        //    //NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        //
        //    PathAnimation {
        //        duration: 200
        //        path: Path {
        //            startX: addTrans.ViewTransition.destination.x + 200
        //            startY: addTrans.ViewTransition.destination.y
        //            PathCurve { relativeX: 0; relativeY: 0 }
        //            PathCurve { relativeX: -100; relativeY: 0 }
        //            PathCurve {
        //                x: addTrans.ViewTransition.destination.x
        //                y: addTrans.ViewTransition.destination.y
        //            }
        //        }
        //    }
        //
        //    onRunningChanged:
        //    {
        //        if(!running)
        //        {
        //            enableAllAddButtons();
        //            enableAllTrashButtons();
        //        }
        //        else
        //        {
        //            disableAllTrashButtons();
        //            disableAllAddButtons();
        //        }
        //    }
        //}
        //
        //addDisplaced: Transition {
        //    NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
        //    NumberAnimation { property: "opacity"; to: 1.0 }
        //    //NumberAnimation { property: "scale"; to: 1.0 }
        //}

        ScrollBar.vertical: ScrollBar
        {
            id: scrollBar
            active: true
            width: 15
        }
    }

    Popup
    {
        function setImageURL(url)
        {
            if(previewImg.source == url)
            {
                var oldSource = previewImg.source
                previewImg.source = ""
                previewImg.source = oldSource
            }
            else
                previewImg.source = url;
        }

        function setClickPos(clickRateX, clickRateY, writeId)
        {
            marker.x = clickRateX * previewImg.width
            marker.y = clickRateY * previewImg.height
            writeIndex = writeId
        }

        function writeClickPosBack(clickX, clickY)
        {
            commandListModel.setProperty(writeIndex, "clickRateX", clickX / previewImg.width)
            commandListModel.setProperty(writeIndex, "clickRateY", clickY / previewImg.height)

            console.log("clickXRate", clickX / previewImg.width, "clickRateY", clickY / previewImg.height )
        }

        property real writeIndex: 0

        id: popup
        width: 250
        height: 250
        anchors.centerIn: parent
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        BorderImage {
            id: previewImg
            width: sourceSize.width > sourceSize.height ? (250) : (250) * (sourceSize.width / sourceSize.height)
            height: sourceSize.height > sourceSize.width ? (250) : (250) * (sourceSize.height / sourceSize.width)
            anchors.centerIn: parent
            asynchronous: false
            cache: false

            Text {
                id: marker
                text: GlobalConfig.commandPointIcon
                font.family: GlobalConfig.regularFontFamily
                font.pixelSize: GlobalConfig.commandPointPixelSize
                visible: previewImg.source != ""
                color: "red"
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    popup.writeClickPosBack( marker.x = mouseX, marker.y = mouseY)
                    console.log("mouseX", mouseX, "mouseY", mouseY)
                }
            }
        }
    }
}
