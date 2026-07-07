import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: mainWindow
    width: Screen.width * 2 / 3
    height: Screen.height * 2 / 3
    visible: true
    title: qsTr("硬盘榨干器")
    font.family: "Maple Mono NF CN"

    property bool inTask: false
    property bool paused: false
    property bool autoTaskExists: autoTaskCreator.hasAutoTask()


    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 50
        spacing: 10

        RowLayout {
            id: contentLayout
            Layout.bottomMargin: 30
            spacing: 10

            ColumnLayout {
                id: leftLayout
                Layout.rightMargin: 20
                spacing: 10

                RowLayout {
                    id: pathLayout
                    spacing: 10

                    TextField {
                        id: pathInput
                        Layout.fillWidth: true
                        enabled: !inTask
                        placeholderText: qsTr("榨干路径")

                        // 实时反馈输入是否合法
                        onTextChanged: {
                            checkPathValidity();
                        }

                        function checkPathValidity() {
                            var fullPath = pathInput.text;
                            if (fullPath === "") {
                                pathStatusText.text = qsTr("请输入路径");
                                pathStatusText.color = "gray";
                                startButton.enabled = false;
                                createAutoTaskButton.enabled = false;
                                return;
                            }

                            // 检查目录是否存在
                            if (!fileSystemChecker.check(fullPath)) {
                                pathStatusText.text = qsTr("× 父目录 ") + fileSystemChecker.getDirectory(fullPath) + qsTr(" 不存在、无法访问或 ") + fullPath + qsTr(" 不是一个文件");
                                pathStatusText.color = "red";
                                startButton.enabled = false;
                                createAutoTaskButton.enabled = false;
                                return;
                            }

                            // 全部验证通过
                            pathStatusText.text = qsTr("✓ 有效路径");
                            pathStatusText.color = "green";
                            startButton.enabled = true;
                            createAutoTaskButton.enabled = true;
                        }
                    }

                    Button {
                        id: browseButton
                        enabled: !inTask
                        text: qsTr("浏览")

                        onClicked: {
                            fileDialog.open();
                        }
                    }
                }

                Text {
                    id: pathStatusText
                    Layout.fillWidth: true
                    text: qsTr("请输入路径")
                    wrapMode: Text.WordWrap
                    color: "gray"
                }

                ComboBox {
                    id: modeComboBox
                    Layout.fillWidth: true
                    enabled: !inTask
                    model: [qsTr("写入模式"), qsTr("命令模式"), qsTr("划区模式")]
                    currentIndex: 2
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 10

                    // 日志输出
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 2

                        Label {
                            text: qsTr("Qt 日志")
                        }
                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            TextArea {
                                id: logTextArea
                                verticalAlignment: TextArea.AlignTop
                                readOnly: true
                            }
                        }
                    }

                    // 命令模式输出
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 2

                        Label {
                            text: qsTr("命令模式输出")
                        }
                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            TextArea {
                                id: cmdOutputTextArea
                                verticalAlignment: TextArea.AlignTop
                                text: "> "
                                readOnly: true
                                enabled: modeComboBox.currentIndex === 1
                            }
                        }
                    }
                }

                Button {
                    id: githubButton
                    Layout.fillWidth: true
                    icon.source: "qrc:/assets/images/github.svg"
                    text: qsTr("GitHub：@0xlonger")

                    onClicked: {
                        Qt.openUrlExternally("https://github.com/0xlonger");
                    }
                }

                Button {
                    id: bilibiliButton
                    Layout.fillWidth: true
                    icon.source: "qrc:/assets/images/bilibili.svg"
                    text: qsTr("哔哩哔哩：@龙ger_longer")

                    onClicked: {
                        Qt.openUrlExternally("https://space.bilibili.com/3493110082439389");
                    }
                }
            }

            ScrollView {
                id: aboutView
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    id: aboutInput
                    verticalAlignment: TextArea.AlignTop
                    text: qsTr(
`程序名：硬盘榨干器(以下简称“该程序”或“本程序”)\n
版本：3.0.0\n
作者：龙ger_longer (也称：龙gerLonger, longer_longer, 0xlonger(GitHub现用名), longlonger2022(GitHub曾用名))\n
该程序可以用于榨干您的硬盘，请谨慎使用。榨干后具体恢复方法请查看UP主(作者)的B站(或其他平台)视频，或者私信UP主(作者)以获取帮助。\n
请勿将本程序恶意传播，否则后果自负。\n
请谨慎使用本程序，UP主(作者)仅提供已知问题的解决方法，造成的其他任何后果UP主(作者)概不负责。\n
若有任何问题请私信询问UP主，一段时间内可能得不到回复，请耐心等待。B站私信可较快获得回复，其他平台时间较长。想要更快地获得回复，可加入UP主(作者)的QQ交流群(群号：975059383，仅供交流与学习使用)以获取帮助。如果长时间未获得回复，可能的原因：UP主(作者)可能较忙、学业繁忙、突发情况等。\n
本程序为开源程序，遵守GNU GPLv3开源协议，若需在本程序的基础上再加以制作，请遵守此协议。协议请点击右侧按钮查看\n
本程序完全免费，严禁一切未经许可的盈利行为(如：倒卖，盗取等)\n
QQ交流群(仅供学习和交流使用，请遵守群规)：975059383\n
作者 龙ger_longer 的个人网站由 GitHub Pages 和 Cloudflare Pages 代理，欢迎访问：
GitHub Pages：https://0xlonger.github.io/
Cloudflare Pages：https://lgr.pages.dev/
（二者的内容是相同的）`
                              )
                    wrapMode: TextArea.WordWrap
                    readOnly: true
                    selectByMouse: true
                }
            }
        }


        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0
            to: 100
            value: 0
        }

        RowLayout {
            id: infoLayout

            Text {
                text: qsTr("榨干进度")
            }

            Text {
                id: statusText
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: "0/0 0%"
            }
        }

        RowLayout {
            id: buttonLayout
            Layout.alignment: Qt.AlignCenter
            spacing: 30

            Button {
                id: startButton
                enabled: !inTask
                visible: enabled
                text: qsTr("开始")

                onClicked: {
                    var path = pathInput.text.trim();

                    var dir = fileSystemChecker.getDirectory(path);

                    var free = fileSystemChecker.getFreeSpace(dir);
                    if (free < 0) {
                        statusText.text = qsTr("错误：无法获取磁盘剩余空间");
                        statusText.color = "red";
                        return;
                    }
                    if (free === 0) {
                        statusText.text = qsTr("错误：磁盘已无剩余空间");
                        statusText.color = "red";
                        return;
                    }

                    // 选择文件创建器
                    var creator = null;
                    if (modeComboBox.currentIndex === 0) {
                        creator = writeModeCreator;
                    } else if (modeComboBox.currentIndex === 1) {
                        creator = commandModeCreator;
                    } else if (modeComboBox.currentIndex === 2) {
                        creator = allocateModeCreator;
                    }

                    // 清空旧进度，准备开始
                    statusText.text = "0/0 0%";
                    statusText.color = "green";

                    // 启动定时器轮询文件大小和剩余空间
                    progressTimer.targetSize = free;
                    progressTimer.filePath = path;
                    progressTimer.dirPath = dir;

                    inTask = true;
                    paused = false;

                    startButton.enabled = false;

                    creator.create(path, free);
                }
            }

            Button {
                id: pauseButton
                text: paused ? qsTr("继续") : qsTr("暂停")
                enabled: inTask && modeComboBox.currentIndex === 0
                visible: enabled

                onClicked: {
                    if (paused) {
                        writeModeCreator.resume();
                        paused = false;
                    } else {
                        writeModeCreator.pause();
                        paused = true;
                    }
                }
            }

            Button {
                id: cancelButton
                text: qsTr("取消")
                enabled: inTask && modeComboBox.currentIndex === 0
                visible: enabled

                onClicked: {
                    writeModeCreator.cancel();
                    inTask = false;
                    startButton.enabled = true;
                    progressBar.value = 0;
                }
            }

            Button {
                id: createAutoTaskButton
                text: qsTr("下次启动时自动静默榨干")

                onClicked: {
                    var path = pathInput.text.trim();
                    var mode = modeComboBox.currentIndex;

                    autoTaskCreator.createAutoTask(path, mode);

                    statusText.text = qsTr("已创建静默榨干任务，下次启动时将自动榨干");
                    statusText.color = "green";

                    autoTaskExists = autoTaskCreator.hasAutoTask();
                }
            }

            Button {
                id: deleteAutoTaskButton
                text: qsTr("取消静默榨干任务")
                enabled: autoTaskExists
                visible: enabled

                onClicked: {
                    autoTaskCreator.deleteAutoTask();

                    statusText.text = qsTr("已取消静默榨干任务");
                    statusText.color = "green";

                    autoTaskExists = autoTaskCreator.hasAutoTask();
                }
            }

            Button {
                id: exitButton
                text: qsTr("退出")

                onClicked: {
                    Qt.quit();
                }
            }
        }

    }

    FileDialog {
        id: fileDialog
        title: qsTr("选择文件路径")

        fileMode: FileDialog.SaveFile

        onAccepted: {
            var fileUrl = selectedFile.toString();
            if (fileUrl.startsWith("file://")) {
                pathInput.text = fileUrl.substring(8);
            } else {
                pathInput.text = fileUrl;
            }

            // 自动触发验证
            pathInput.checkPathValidity();
        }
    }

    Timer {
        id: progressTimer
        interval: 10
        repeat: true
        property double targetSize: 0
        property string filePath: ""
        property string dirPath: ""
        running: inTask && !paused

        onTriggered: {
            // 获取当前文件大小
            var currentSize = fileSystemChecker.fileSize(filePath);
            var freeSpace = fileSystemChecker.getFreeSpace(dirPath);

            if (currentSize < 0) {
                // 文件可能尚未创建，显示 0
                currentSize = 0;
            }

            var percent = targetSize > 0 ? (currentSize / targetSize * 100) : 0;
            // 显示已榨干大小（MiB）和剩余空间（MiB）
            var currentMiB = (currentSize / (1024*1024)).toFixed(2);
            var freeMiB = (freeSpace / (1024*1024)).toFixed(2);

            progressBar.value = percent;
            statusText.text = currentMiB + " MiB / " + freeMiB + " MiB  " + percent.toFixed(1) + "%";
        }
    }

    Connections {
        target: logHandler
        function onLogMessage(message) {
            logTextArea.append(message);
            // 自动滚动到底部
            logTextArea.cursorPosition = logTextArea.length;
        }
    }

    Connections {
        target: commandModeCreator
        function onCommandStarted(cmdLine) {
            cmdOutputTextArea.text += cmdLine;
        }
        function onCommandOutput(output) {
            if (output.trim() !== "") {
                cmdOutputTextArea.append(output + "\n");
            } else {
                cmdOutputTextArea.append("");
            }
            cmdOutputTextArea.text += "\n> ";
        }
    }

    Connections {
        target: writeModeCreator
        function onFinished(success, message) {
            inTask = false;
            startButton.enabled = true;
            statusText.text = message;
            statusText.color = success ? "green" : "red";
            if (success) {
                progressBar.value = 100;
            }
        }
    }
    Connections {
        target: commandModeCreator
        function onFinished(success, message) {
            inTask = false;
            startButton.enabled = true;
            statusText.text = message;
            statusText.color = success ? "green" : "red";
            if (success) {
                progressBar.value = 100;
            }
        }
    }
    Connections {
        target: allocateModeCreator
        function onFinished(success, message) {
            inTask = false;
            startButton.enabled = true;
            statusText.text = message;
            statusText.color = success ? "green" : "red";
            if (success) {
                progressBar.value = 100;
            }
        }
    }

}
