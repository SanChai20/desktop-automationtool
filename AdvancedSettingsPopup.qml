import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "globalConfiguration.js" as GlobalConfig


Popup {

    id: popRoot
    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    function showSettings(matchTypeIndex, detectTypeIndex, policyTypeIndex, languageTypeIndex, kNNRatio)
    {
        priorityTypeCombo.currentIndex = 0
        matchTypeCombo.currentIndex = matchTypeIndex
        detectTypeCombo.currentIndex = detectTypeIndex
        textField01.text = kNNRatio.toFixed(2).toString()
        policyTypeCombo.currentIndex = policyTypeIndex
        languageTypeCombo.currentIndex = languageTypeIndex
    }

    property real layoutRatio: 0.25

    signal matchTypeChanged(var index);
    signal detectTypeChanged(var index);
    signal policyTypeChanged(var index);
    signal languageTypeChanged(var index);
    signal kNNRatioChanged(real ratio);

    RowLayout
    {
        id: nameAndParam
        width: parent.width * 0.9
        height: parent.height * 0.95
        anchors.centerIn: parent
        spacing: 10

        ColumnLayout
        {
            id: nameLayout
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            Layout.minimumWidth: parent.width * layoutRatio
            Layout.maximumWidth: parent.width * layoutRatio
            Layout.preferredWidth: parent.width * layoutRatio
            Layout.preferredHeight: 40

            spacing: 5

            Rectangle
            {
                id: name00_priorityType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                //TEXT
                Text
                {
                    id: name00_Text
                    anchors.centerIn: parent
                    text: "Priority"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }

            Rectangle
            {
                id: name01_matchType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                //TEXT
                Text
                {
                    id: name01_Text
                    anchors.centerIn: parent
                    text: "Matching Algorithm"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }

            Rectangle
            {
                id: name02_detectType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                //TEXT
                Text
                {
                    id: name02_Text
                    anchors.centerIn: parent
                    text: "Detecting Algorithm"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }

            Rectangle
            {
                id: name01b_kNNRatio
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                //TEXT
                Text
                {
                    id: name01b_Text
                    visible: matchTypeCombo.currentIndex == 0 || matchTypeCombo.currentIndex == 2
                    anchors.centerIn: parent
                    text: "kNN Threshold"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }

            Rectangle
            {
                id: name03_policyType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                //TEXT
                Text
                {
                    id: name03_Text
                    anchors.centerIn: parent
                    text: "Execution Strategy"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }

            Rectangle
            {
                id: name04_languageType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                //TEXT
                Text
                {
                    id: name04_Text
                    anchors.centerIn: parent
                    text: "Language"
                    font.pixelSize: GlobalConfig.namePixelSize
                    color: GlobalConfig.nameFontColor
                }
            }
        }

        ColumnLayout
        {
            id: paramLayout
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            Layout.minimumWidth: parent.width * (1.0 - layoutRatio)
            Layout.maximumWidth: parent.width * (1.0 - layoutRatio)
            Layout.preferredWidth: parent.width * (1.0 - layoutRatio)
            Layout.preferredHeight: 40

            spacing: 5

            Rectangle
            {
                id: param00_priorityType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                ComboBox {
                    id: priorityTypeCombo
                    anchors.centerIn: parent
                    width: parent.width * 0.95
                    height: parent.height * 0.95

                    //currentIndex: 0
                    model: ListModel {
                        id: priorityItems
                        ListElement { textSource: "Custom"; }
                        ListElement { textSource: "Accuracy priority"; }
                        ListElement { textSource: "Speed priority"; }
                        ListElement { textSource: "False positive rate priority"; }
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
                                matchTypeCombo.enabled = true
                                detectTypeCombo.enabled = true
                                textField01.enabled = true
                            }
                            else if(index == 1)
                            {
                                matchTypeCombo.enabled = false
                                detectTypeCombo.enabled = false
                                textField01.enabled = true
                                matchTypeCombo.currentIndex = 1
                                matchTypeChanged(1)
                                detectTypeCombo.currentIndex = 1
                                detectTypeChanged(1)
                            }
                            else if(index == 2)
                            {
                                matchTypeCombo.enabled = false
                                detectTypeCombo.enabled = false
                                textField01.enabled = false

                                matchTypeCombo.currentIndex = 2
                                matchTypeChanged(2)
                                detectTypeCombo.currentIndex = 0
                                detectTypeChanged(0)
                                var knnRatio = 0.70
                                textField01.text = knnRatio.toString()
                                kNNRatioChanged(knnRatio)
                            }
                            else if(index == 3)
                            {
                                matchTypeCombo.enabled = false
                                detectTypeCombo.enabled = false
                                textField01.enabled = false
                                matchTypeCombo.currentIndex = 0
                                matchTypeChanged(0)
                                detectTypeCombo.currentIndex = 1
                                detectTypeChanged(1)
                                var knnRatio = 0.15
                                textField01.text = knnRatio.toString()
                                kNNRatioChanged(knnRatio)
                            }
                        }
                    }
                    font.pixelSize: GlobalConfig.timelineFontPixelSize
                }
            }

            Rectangle
            {
                id: param01_matchType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                ComboBox {
                    id: matchTypeCombo
                    anchors.centerIn: parent
                    width: parent.width * 0.95
                    height: parent.height * 0.95

                    //currentIndex: 1
                    model: ListModel {
                        id: matchItems
                        ListElement { textSource: "BRUTE_KNN"; }
                        ListElement { textSource: "BRUTE_COMMON"; }
                        ListElement { textSource: "FLANNBASED_KNN"; }
                        ListElement { textSource: "FLANNBASED_COMMON"; }
                    }
                    delegate: ItemDelegate
                    {
                        text: textSource
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                        width: parent.width

                        onClicked:
                        {
                            matchTypeChanged(index);
                        }
                    }
                    font.pixelSize: GlobalConfig.timelineFontPixelSize
                }
            }

            Rectangle
            {
                id: param02_detectType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                ComboBox {
                    id: detectTypeCombo
                    anchors.centerIn: parent
                    width: parent.width * 0.95
                    height: parent.height * 0.95

                    //currentIndex: 1
                    model: ListModel {
                        id: detectItems
                        ListElement { textSource: "SURF"; }
                        ListElement { textSource: "SIFT"; }
                        ListElement { textSource: "KAZE"; }
                    }
                    delegate: ItemDelegate
                    {
                        text: textSource
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                        width: parent.width

                        onClicked:
                        {
                            detectTypeChanged(index);
                        }
                    }
                    font.pixelSize: GlobalConfig.timelineFontPixelSize
                }


            }

            Rectangle
            {
                id: param01b_kNNratio
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                TextField {
                    id: textField01
                    visible: matchTypeCombo.currentIndex == 0 || matchTypeCombo.currentIndex == 2
                    anchors.centerIn: parent
                    width: parent.width * 0.9
                    height: parent.height * 0.95

                    color: GlobalConfig.commandTextFieldFontColor
                    selectByMouse: true
                    validator: DoubleValidator
                    {
                        bottom: 0;
                        top: 100;
                        decimals: 2;
                        notation: DoubleValidator.StandardNotation
                    }
                    placeholderText: qsTr("range from 0.0 to 1.0, default is 0.4")
                    font.pixelSize: 14
                    onEditingFinished:
                    {
                        kNNRatioChanged(parseFloat(textField01.text))
                    }
                }
            }

            Rectangle
            {
                id: param03_policyType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                ComboBox {
                    id: policyTypeCombo
                    anchors.centerIn: parent
                    width: parent.width * 0.95
                    height: parent.height * 0.95

                    //currentIndex: 0
                    model: ListModel {
                        id: policyItems
                        ListElement { textSource: "stop command execution after detection failure"; }
                        ListElement { textSource: "execute current command in a loop after detection failure"; }
                        ListElement { textSource: "execute next command after detection failure"; }
                    }
                    delegate: ItemDelegate
                    {
                        text: textSource
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                        width: parent.width

                        onClicked:
                        {
                            policyTypeChanged(index);
                        }
                    }
                    font.pixelSize: GlobalConfig.timelineFontPixelSize
                }

            }

            Rectangle
            {
                id: param04_languageType
                color: "transparent"
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.minimumWidth: parent.width
                Layout.maximumWidth: parent.width
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40

                ComboBox {
                    id: languageTypeCombo
                    anchors.centerIn: parent
                    width: parent.width * 0.95
                    height: parent.height * 0.95

                    enabled: false
                    currentIndex: 0
                    model: ListModel {
                        id: languageItems
                        ListElement { textSource: "English"; }
                    }
                    delegate: ItemDelegate
                    {
                        text: textSource
                        font.pixelSize: GlobalConfig.timelineFontPixelSize
                        width: parent.width

                        onClicked:
                        {
                            languageTypeChanged(index);
                        }
                    }
                    font.pixelSize: GlobalConfig.timelineFontPixelSize
                }

            }

        }




    }



}



