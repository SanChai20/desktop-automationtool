.import QtQuick.Controls.Material 2.3 as Sql

var regularFontFamily = "Font Awesome 6 Free Regular"
var solidFontFamily = "Font Awesome 6 Free Solid"
var logWindowEdgeBorderColor = "#6382f2"

var captureImagePreviewSize = 256
var captureImagePrefix = "image://captures/"
var captureWindowMaxCount = 32

//TimelineBar Configuration
var importIcon = "\uf56f"
var importIconTip = "Import file of command set"


var exportIcon = "\uf56e"
var exportIconTip = "Export file of command set"


var stopRunningIcon = "\uf04d"
var stopRunningIconTip = "Stop Execution"


var pauseRunningIcon = "\uf04c"
var pauseRunningIconTip = "Pause Execution"


var beginRunningIcon = "\uf04b"
var beginRunningIconTip = "Start Execution"

var timelineFontPixelSize = 11
var timelineButtonFontPixelSize = 15

var buttonStyle01_IconColor = "#f08b11"
var buttonStyle01_BorderColor = "#f08b11"
var buttonStyle01_IconDownColor = "black"
var buttonStyle01_BorderDownColor = "black"
var buttonStyle01_BackGroundColor = "#303030"
var buttonStyle01_BackGroundDownColor = "#303030"


var buttonStyle02_IconColor = "#6382f2"
var buttonStyle02_BorderColor = "#6382f2"
var buttonStyle02_IconDownColor = "#8198eb"
var buttonStyle02_BorderDownColor = "#8198eb"
var buttonStyle02_BackGroundColor = "#303030"
var buttonStyle02_BackGroundDownColor = "#303030"



var mainBackGroundColor = "#303030"



//CommandStepWindow Configuration
var commandSelectedColor = "#404040"
var commandFontColor = "white"
var commandTextFieldFontColor = "#6382f2"
var commandLineHeight = 32
var commandFontPixelSize = 14
var commandButtonFontPixelSize = 15


var addIcon = "\uf067"
var addIconTip = "Insert one command below"

var trashIcon = "\uf2ed"// "\uf2ed"//
var trashIconTip = "remove current command"

var objectImageTip = "click to select matching image"
var objectImageTip2 = "click to replace matching image"

var switchIcon  = "\uf0c9"//"\uf7a4"
var switchIconTip = "change order by dragging up and down"

var checkBoxTip = "in looping or not"


var commandPointIcon = "\uf25a"
var commandPointIconTip = "select click location"
var commandPointPixelSize = 25

var rightArrowIcon = "\uf30b"
var rightArrowIconTip = "indicating executing current command"

var clickComboWidth = 155


//CaptureLogWindow Configuration


var capturelogwindowLogFontColor = "white"
var capturelogwindowLogSelectedColor = "grey"

var buttonRectangleHeight = 40
var buttonGapMarginRight = 10
var buttonGapMarginBottom = 5
var buttonTextPixelSize = 25

var captureButtonIcon = "\uf065"
var captureButtonIconTip = "capture window by clicking"
var captureButtonIconTip2 = "capture window again"

var cleanButtonIcon = "\uf2ed"
var cleanButtonIconTip = "clean log info record"


var closeDebugCoverIcon = "\uf06e"
var closeDebugCoverIconTip = "close debug window by clicking"

var showDebugCoverIcon = "\uf070"
var showDebugCoverIconTip = "open debug window by clicking"

//Script Main Content Configuration

var addWindowIcon = "\uf5fd"
var addWindowIconTip = "add new window"

var closeWindowIcon = "\uf410"
var closeWindowIconTip = "close current window"

var closeApplicationIcon = "\uf00d"
var closeApplicationIconTip = "shut down application"

var windowHintIcon = "\uf51b"


var addWindowButtonSize = 40
var btnGapSize = 5

var comboButtonPixelSize = 12
var comboButtonHoveredPixelSize = 15
var closeMainButtonSize = 22
var closeSubLevelButtonSize = 25

//main


var applicationWindowWidth = 820
var applicationWindowHeight = 720
var applicationWindowMinimumWidth = 820
var applicationWindowMinimumHeight = 600
var applicationWindowMaximumWidth = 1080
var applicationWindowMaximumHeight = 1080


//MainWindowMenuHeader


var closeMenuIcon = "\uf2d3"
var closeMenuIconTip = "shut down application"

var minimumMenuIcon = "\uf2d1"
var minimumMenuIconTip = "minimized application"

var btnMenuSize = 18

//Advanced Settings

var namePixelSize = 14
var nameFontColor = "white"



// function

function getAbsolutePath(originPath)
{
    var path = originPath.toString()
    // remove prefixed "file:///"
    path = path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/, "")
    // unescape html codes like '%23' for '#'
    return decodeURIComponent(path)
}

//developer info
var profileContent = "a game developer who wants to put life on easy mode"
var generalContent = "Contact developer if u have any suggestions or problems"
var mailboxContent = "sanchai20@163.com"
var wechatContent = "sanchai233"
var profileContentFontPixelSizeLarge = 18
var profileContentFontPixelSizeSmall = 16
var profileContentFontColor = "white"
var profileContentFontSubColor = "orange"


var profileCopyIcon = "\uf0c5"
var profileCopyIconTip = "Copy email address"

