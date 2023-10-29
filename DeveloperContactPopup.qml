import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "globalConfiguration.js" as GlobalConfig
import GameScriptClipBoard 1.0
Popup {

    id: root
    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


    ColumnLayout
    {
        id: generalColumn

        width: parent.width * 0.9
        height: parent.height * 0.5
        anchors.centerIn: parent
        spacing: 5

        Rectangle
        {
            id: hintContentRect
            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            Layout.minimumWidth: parent.width
            Layout.maximumWidth: parent.width
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: hintContent.contentHeight

            Text
            {
                id: hintContent
                anchors.fill: parent
                text: qsTr(GlobalConfig.generalContent)
                font.pixelSize: GlobalConfig.profileContentFontPixelSizeSmall
                color: GlobalConfig.profileContentFontColor
                elide: Text.ElideRight
            }
        }


        Rectangle
        {
            id: mailBoxContactRect
            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            Layout.minimumWidth: parent.width
            Layout.maximumWidth: parent.width
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: mailBoxContact.contentHeight

            RowLayout
            {
                id: contactRowLayout

                width: parent.width
                height: mailBoxContact.contentHeight
                spacing: 5

                Text
                {
                    id: mailBoxContact
                    Layout.alignment: Qt.AlignLeft
                    text: qsTr("Email Address: ") + GlobalConfig.mailboxContent
                    font.pixelSize: GlobalConfig.profileContentFontPixelSizeLarge
                    color: GlobalConfig.profileContentFontSubColor
                }

                ButtonStyle05 {
                    id: mailBoxPasteBtn
                    btnIconCode: GlobalConfig.profileCopyIcon
                    btnIconFamily: GlobalConfig.regularFontFamily
                    //buttonTextColor: GlobalConfig.profileContentFontColor
                    btnIconTip: GlobalConfig.profileCopyIconTip

                    onClicked:
                    {
                        clipboard.paste(GlobalConfig.mailboxContent)
                    }

                }

                Rectangle
                {
                    Layout.fillWidth: true
                }
            }



        }

        Rectangle
        {
            id: empty
            Layout.fillHeight: true
        }


    }


    GameScriptClipBoard
    {
        id: clipboard

    }



}
