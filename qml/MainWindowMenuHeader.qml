import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../js/globalConfiguration.js" as GlobalConfig

Rectangle {

    id: root

    RowLayout
    {
        width: root.width
        height: root.height
        spacing: 5

        Rectangle
        {
            id: stateInfoTextHolder
            Layout.minimumWidth: 70
            Layout.maximumWidth: 100
            Layout.preferredWidth: 80
            Layout.preferredHeight: root.height
            color: "blue"
            Text
            {
                id: stateInfoText
                color: "green"
                text: "aaaaaaaaaaaaa"
                anchors.centerIn: parent
            }
        }

        Rectangle
        {
            id: emptyRect
            Layout.fillWidth: true
        }


        Rectangle
        {
            id: closeBtnHolder
            Layout.minimumWidth: root.height
            Layout.maximumWidth: root.height
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
            color: GlobalConfig.mainBackGroundColor

            ButtonStyle03
            {
                id: closeBtn
                anchors.fill: parent
                btnIconCode: GlobalConfig.closeMenuIcon
                btnIconFamily: GlobalConfig.solidFontFamily
                btnIconTip: GlobalConfig.closeMenuIconTip
                btnIconPixelSize: GlobalConfig.btnMenuSize

                onClicked:
                {

                }
            }

        }




        //
        //Rectangle
        //{
        //    id: closeBtnHolder
        //    Layout.minimumWidth: 70
        //    Layout.maximumWidth: 100
        //    Layout.preferredWidth: 80
        //    Layout.preferredHeight: parent.height
        //    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        //    color: "blue"
        //
        //    ButtonStyle03
        //    {
        //        id: closeBtn
        //        anchors.fill: parent
        //        buttonTextDownColor: "black"
        //        btnIconCode: GlobalConfig.commandPointIcon
        //        btnIconTip: GlobalConfig.commandPointIconTip
        //        btnIconPixelSize: GlobalConfig.commandButtonFontPixelSize
        //        onClicked:
        //        {
        //
        //        }
        //    }
        //}


    }

}
