import QtQuick 2.0
import CoverTransparentWindow 1.0

Covertransparentwindow {

    function setMatchRect(x, y, width, height, clickX, clickY)
    {

    }


    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput
    color: "#00E2FF"
    visible: false


    Rectangle{
        id: matchRectOuter
        color: "red"

        Rectangle
        {
            id: matchRectInner
            color: "#00E2FF"
            anchors{
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                left: parent.left

                topMargin: 4
                bottomMargin: 4
                leftMargin: 4
                rightMargin: 4
            }
        }



    }

}
