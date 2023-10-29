import QtQuick 2.0
import Qt.labs.platform 1.0
import "globalConfiguration.js" as GlobalConfig

FileDialog {

    signal imgFileSelected(string file, string absolutePath)

    id: root
    acceptLabel: "Select"
    rejectLabel: "Cancel"
    nameFilters: ["(*.png *.bmp *.jpg)"]

    onAccepted: {
        var cleanPath = GlobalConfig.getAbsolutePath(root.file)
        root.imgFileSelected(root.file, cleanPath)
    }
}
