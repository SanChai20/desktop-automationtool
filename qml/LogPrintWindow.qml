import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Layouts 1.3

Rectangle {

    id: root
    property alias logList : logListModel
    property alias logCurrentIndex: listView.currentIndex

    property int edgeWidth: 2
    property string edgeColor: "red"
    property string mainColor: "green"
    property string fontColor: "black"
    property string fontSelectionBackColor: "grey"

    color: edgeColor

    Rectangle
    {
        anchors
        {
            top: parent.top
            topMargin: edgeWidth

            bottom: parent.bottom
            bottomMargin: edgeWidth

            left: parent.left
            leftMargin: edgeWidth

            right: parent.right
            rightMargin: edgeWidth

        }

        color: mainColor

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

                    font.pixelSize: 12
                    text: logInfo
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true


                    color: fontColor
                    selectionColor : fontSelectionBackColor

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
