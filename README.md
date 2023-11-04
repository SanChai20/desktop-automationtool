# desktop-automationtool
A desktop automation tool used to capture windows and imitate mouse click events at arbitrary location. 

This is a [Qt 5.12.12](https://www.qt.io/offline-installers) and [OpenCV 4.7.0](https://opencv.org/releases/) based project that developing environment should be configured on your own.

Windows x64 installer can be found [here](https://github.com/SanChai20/desktop-automationtool/releases)



# Jee Jee Automation Tool User Manual

> Jee-Jee can be used to capture any desktop window, and then perform simple operations according to the given instruction queue sequence, which can save labor costs and is suitable for scenes with high repetition and high mechanical nature.


## Ⅰ. Operation Guide

### ①. Installation

1. Double click `JeeJee Installer.exe` to install desktop application. After the installation is completed, start the application by desktop shortcut or `JeeJee Installer.exe` in the installation directory .

### ②. Window Capture

1. Please **click the green icon within the red box in the screen-shot below**

   ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\1.png)

2. Then **move the mouse cursor to the desktop window to be captured** . The window to be captured will be covered with a light green area to remind you which window is selected.

3. After confirmation, **please click the left mouse button** anywhere on the target window to capture. After the capture is successful, the capture icon mentioned in step 1 will be displayed in red.

### ③. Configure instructions

1. Then look at the middle part of the main interface and you will find an empty command. 

2. ***Configure the first command***

   - Click red box to select matching image which has been captured and saved in advance

     ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\2.png)

   - Click red box to select mouse click event

     ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\3.png)

   - Click red box to select and save click location on image

     ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\4.png)

   - 点击`详解图`中的④处，给定合适的时间间隔，这里暂且给定2秒

     ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\5.png)

3. Click ![](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\add.svg) to add new command and repeat step 2 to configure all rest commands


### ④. Window operation

1. Then click ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\play.svg)the button at the bottom of the main interface to start running. You can click the button ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\eye-slash.svg) to turn on the debugging mode before starting , and the command execution will be visualized on the target window.

### ⑤. Export command file

1. In order to save time for the next time using, we can export the command list after configuring it by clicking ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\export.svg). The export file format is `.command` and this file can be imported in other capture windows by clicking ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\import.svg)

## Ⅱ. Function introduction

> The following will introduce each editor interface module one by one.

### ①. Main interface partition

<img src="F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\6.png" style="zoom:45%;" />

- As shown in the figure above, the main interface can be divided into three parts according to functions.

  - Global Control Area, GCA (red box above)

    > Mainly used for control `Window Captured` and global configuration

  - Visualization Area, VA (orange box above)

    > The visual ribbon contains `Printing Log`, `Thumbnail of Captured Window`and`Funtionality Button Area`
    >
    > **Each capture window in this area is independent of each other**

  - Command Logic Area, CLA (blue box above)

    > Configure `Command List` `Import/Export Command File` `Running Logic`
    >
    > **Each capture window in this area is independent of each other**

### ②. Detailed explanation of icons

> All icons will be explained below. You can refer here when you encounter them.

|                    `Icon`                    | `Explanation`                                                | `Area` |
| :------------------------------------------: | :----------------------------------------------------------- | :----: |
|   ![](D:\JeeJee\JeeJee0822\chalkboard.svg)   | This icon appears to the left of the capture window title and is used to distinguish between different windows |  GCA   |
|    ![](D:\JeeJee\JeeJee0822\close01.svg)     | This icon appears on the far right of each capture window. Click to close this window. This function is not available only when there is only one window remaining or the current window is running. |  GCA   |
|   ![](D:\JeeJee\JeeJee0822\addwindow.svg)    | This icon appears first when opening the drop-down menu on the right side of the GCA and is used to add a new capture window |  GCA   |
|    ![](D:\JeeJee\JeeJee0822\contact.svg)     | This icon appears in the second position when opening the drop-down menu on the right side of the GCA. It is used to open a pop-up window displaying developer contact information. |  GCA   |
|    ![](D:\JeeJee\JeeJee0822\settings.svg)    | This icon appears in the third position when opening the drop-down menu on the right side of the GCA. It is used to open the pop-up window for controlling global settings. |  GCA   |
|      ![](D:\JeeJee\JeeJee0822\copy.svg)      | This icon appears in the pop-up window that appears after clicking ![](D:\JeeJee\JeeJee0822\contact.svg). Click to copy the current developer's contact information. |  GCA   |
| ![](D:\JeeJee\JeeJee0822\trash_cleanlog.svg) | This icon appears on the left side of ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\capture.svg)and is used to clear log information |   VA   |
|      ![](D:\JeeJee\JeeJee0822\eye.svg)       | This icon indicates that the debugging window has been opened. Click to close the debugging window. |   VA   |
|   ![](D:\JeeJee\JeeJee0822\eye-slash.svg)    | This icon indicates that the debugging window has been closed. Click to open the debugging window. |   VA   |
|    ![](D:\JeeJee\JeeJee0822\capture.svg)     | This icon appears in the lower right corner of the VA and is used to capture the window. When it is green, it means it is to be captured, and when it is red, it means it has been captured successfully. |   VA   |
|      ![](D:\JeeJee\JeeJee0822\add.svg)       | This icon appears at the end of each command and is used to insert a new command below the current command. |  CLA   |
|      ![](D:\JeeJee\JeeJee0822\bars.svg)      | This icon appears on the left side of each command. Drag this icon up or down to change the position of the command in the queue. |  CLA   |
|  ![](D:\JeeJee\JeeJee0822\handpointer.svg)   | This icon will appear in both the selected image click position and the debugging interface position display, used to visualize the recorded mouse click position. |  CLA   |
|     ![](D:\JeeJee\JeeJee0822\trash.svg)      | This icon appears on the left side of ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\add.svg) and is used to delete the current instruction. |  CLA   |
|     ![](D:\JeeJee\JeeJee0822\import.svg)     | This icon appears at the bottom of the CLA. Click to select the instruction list file that needs to be imported. |  CLA   |
|     ![](D:\JeeJee\JeeJee0822\export.svg)     | This icon appears on the right side of ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\import.svg). Click to export the command list of the current window to the local command file. |  CLA   |
|      ![](D:\JeeJee\JeeJee0822\play.svg)      | This icon appears at the bottom of the command logic ribbon. Click to run the command task of the current capture window. |  CLA   |
|      ![](D:\JeeJee\JeeJee0822\stop.svg)      | When this icon appears, it indicates that it has entered the command running state. Click to stop running the command task of the current capture window. |  CLA   |
|   ![](D:\JeeJee\JeeJee0822\rightarrow.svg)   | This icon will appear in position of ![img](F:\github-projects\JeeJee_ManualMarkdownFiles\JeeJee\JeeJee0822\bars.svg) when running a command to show which command is currently being processed |  CLA   |

### ③. Global configuration instructions

> **If there is no special need, just keep this global configuration as default**.

- `Priority`

  > - Custom
  >
  >   > `Matching Algorithm`  `Detecting Algorithm`  `kNN Threshold` can be adjusted freely
  >
  > - Accuracy fisrt
  >
  >   > Priority is given to ensuring accuracy, but performance and speed will be sacrificed. `Matching Algorithm` will be fixed as `BRUTE_COMMON`, `Detecting Algorithm`will be fixed as `SIFT`
  >
  > - Speed priority
  >
  >   > Prioritizes running efficiency, but will be accompanied by problems such as incorrect matching or lack of matching. `Matching Algorithm` will be fixed as `FLANNBASED_KNN`, `Detecting Algorithm` will be fixed as `SURF`, `kNN Threshold `will be fixed as `0.7`
  >
  > - False positive rate priority
  >
  >   > Try to keep the number of false matches to a minimum and ensure reasonable operating efficiency. However, there is a possibility that the result cannot be matched.  `Matching Algorithm` will be fixed as `BRUTE_KNN`, `Detecting Algorithm` will be fixed as `SIFT`, `kNN Threshold` will be fixed as `0.15`

- `Matching Algorithm` 

  Only used on feature matching. Template matching has nothing to do with this type, see below `Points to note` for details

  > - BRUTE_KNN
  >
  > - BRUTE_COMMON
  >
  >   > The Brute method will perform violent matching, which is too time-consuming, but has higher accuracy. In scenarios where accuracy is higher than speed, the BRUTE method is preferred.
  >
  > - FLANNBASED_KNN
  >
  > - FLANNBASED_COMMON
  >
  >   > The FLANNBASED method has obvious advantages in operating efficiency when the image to be matched is large. If you do not worry about the misjudgment rate of matching, you can use the FLANNBASED method first.
  >
  >   > The KNN method is used to quickly sort out results with higher probability, and is better than the COMMON method under reasonable `kNN Threshold` given circumstances.

- `Detecting Algorithm`

  > - SURF
  >
  >   > Similar to the algorithmic `SIFT`, but much faster than SIFT.
  >
  > - SIFT
  >
  >   > It is recommended to choose this algorithm when there is a brightness difference or size scaling between the image to be matched and the capture window image.
  >
  > - KAZE
  >
  >   > It is recommended to choose this algorithm when there is a rotation difference or blur difference between the image to be matched and the capture window image and there is no obvious brightness change.

- `[kNN Threshold]`

  > When selected `Matching Algorithm` is `BRUTE_KNN ` or `FLANNBASED_KNN`, this item will pop up for adjustment. Please adjust it within the range of 0.01 to 1.00. The smaller the value, the stricter the match, but at the same time, the probability of not matching the result will increase. The larger the value, the looser the match. But at the same time the probability of incorrect matching will increase.

- `Execution Strategy`

  > The strategy that should be adopted when the target image is not matched after the instruction is executed.
  >
  > - stop command execution after detection failure
  >
  >   > Stop execution when the target image is not matched. This is **the default configuration.**
  >
  > - execute current command in a loop after detection failure
  >
  >   > If the target image is not matched, continue to execute the current instruction. The next instruction will not be executed until a reasonable result is matched.
  >
  > - execute next command after detection failure
  >
  >   > If the target image is not matched, continue to execute the next instruction.

- `Language`

  > Currently only supports English



## Ⅲ. Points to note

> The following points will help you avoid invalid operations as much as possible and improve the efficiency of using this application.

- Jee Jee uses the openCV library for image matching. The current program matching logic is performed  `Template matching` first. If it does not pass, it will be performed `Feature matching` automatically. The performance overhead of these two methods is quite different. Try to give priority to template matching, that is, there is no rotation and scaling between the screenshot to be matched and the captured window image.  If it cannot be avoided, you can adjust `Advanced Settings` to trade off time consumption and accuracy.
- The exported file will only record the path of the image to be matched. Therefore, when the image under the path is deleted or moved, the import failure will be prompted when the `.command` file is re-opened, and the thumbnail of the image to be matched in the command list will be black. **Please pay attention to this to avoid unnecessary loss**.
- Although there is no limits on the number of windows ![](D:\JeeJee\JeeJee0822\chalkboard.svg) in the editor, the current version of the Jee Jee application will **occupy a high amount of CPU resources .** It is best not to exceed the number of  **`6`**  windows . Within this number, it will be allocated to different threads for parallel calculations. If it is exceeded, the overall operating efficiency will be reduced significantly. 
- ![](D:\JeeJee\JeeJee0822\eye-slash.svg)can help you find problems when trying to match the command list, and it will visually show whether the match is successful. If the match is not successful, then you may need to:
  - Appropriately expand the interception range of the image to be matched (contain as many feature points as possible)
  - You can adjust `Matching Algorithm` to `Brute`, `Detecting Algorithm` adjust to `SIFT`, or directly adjust `Priority` to `False positive rate priority`
  - If you use `kNN` categories, you can increase the `kNN Threshold`.



## Ⅳ. Optimizations to do

> After testing, currently when Jee Jee is running with 6 capture windows at the same time, `Intel(R) Core(TM) i7-10700 CPU @ 2.90GHz 2.90 GHz`the occupancy rate will be as high as over 90%.



- Move the feature matching process to the GPU
- Check if memory allocation is frequent
- CPU processing work adopts a rotation mechanism
- ~~Ask another developer :sweat_smile:~~



## Ⅴ. Contact the developer

> If you have any comments or suggestions, you can contact the developer by sending an email to [sanchai20@163.com](mailto:sanchai20@163.com)

