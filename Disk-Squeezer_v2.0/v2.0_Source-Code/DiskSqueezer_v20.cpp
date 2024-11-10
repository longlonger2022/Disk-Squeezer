#include "DiskSqueezer_v20.h"
#include <QDesktopWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QGroupBox>
#include <QFontDataBase>
#include <QStringList>
#include <QFont>
#include <QFontDatabase>
#include <QRegExp>
#include <QValidator>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QGraphicsEffect>
#include <QButtonGroup>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QByteArray>

DiskSqueezer_v20::DiskSqueezer_v20(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->setWindowTitle(QString::fromLocal8Bit("硬盘榨干器")); //窗口标题
    this->setWindowIcon(QIcon("img/Disk-Squeezer_v2.0_icon.1024px.png")); //窗口图标
    this->setStyleSheet("QWidget {"
                        "   font: Microsoft Yahei UI, Microsoft Yahei, -apple-system, BlinkMacSystemFont, Segoe UI, Roboto, Ubuntu, Helvetica Neue, Helvetica, Arial,PingFang SC, Hiragino Sans GB, Microsoft YaHei UI, Microsoft YaHei, Source Han Sans CN, sans-serif;"
                        "}"
                        "QPushButton {"
                        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #0095FF, stop: 1 #0065FF);"
                        "   border: 3px solid rgb(210, 210, 210);"
                        "}"
                        "QPushButton:hover {"
                        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #0065FF, stop: 1 #0095FF);"
                        "}"
                        "QPushButton:pressed {"
                        "   background-color: #0095FF;"
                        "}"
                        "QGroupBox {"
                        "   border: 2px solid gray;"
                        "   margin-top: 2ex;"
                        "}"
                        "QGroupBox::title {"
                        "   subcontrol-origin: margin;"
                        "   subcontrol-position: top center;"
                        "   padding: 0 3px;"
                        "}"
                        "QLineEdit {"
                        "   border: 2px solid gray;"
                        "   selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #F902FF, stop: 1 #00DBDE);"
                        "}"
                        "QLineEdit:focus {"
                        "   border-bottom-color: #0095FF;"
                        "}"
                        "QTextEdit {"
                        "   border: 1px solid rgba(120, 120, 120, 0.5);"
                        "   selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #F902FF, stop: 1 #00DBDE);"
                        "}"
                        "QMessageBox {"
                        "   background-color: qlineargradient(x1: 0, y1: 1, x2: 1, y2: 0, stop: 0 white, stop: 1 rgb(230, 230, 230));"
                        "}"
                        "QMessageBox QPushButton {"
                        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop : 0 white, stop : 1 rgb(240, 240, 240));"
                        "   border-radius: 8px;"
                        "}"
                        "QMessageBox QPushButton:hover {"
                        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 rgb(240, 240, 240), stop: 1 white);"
                        "}"
                        "QMessageBox QPushButton:pressed {"
                        "   background-color: white;"
                        "}"
                       );
    const QRect DesktopWidget = QApplication::desktop()->geometry(); //获取屏幕大小
    this->setFixedSize(DesktopWidget.width() * 0.4, DesktopWidget.height() * 0.4); //窗口大小
    this->setGeometry((DesktopWidget.width() - this->width()) / 2, (DesktopWidget.height() - this->height()) / 2, this->width(), this->height()); //窗口位置
    //this->setWindowOpacity(0.925);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setBackgroundColor();

    HWND hWnd = HWND(this->winId());
    HMODULE hUser = GetModuleHandle(L"user32.dll");
    if (hUser)
    {


        pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute)
        {
            ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hWnd, &data);
        }
    }
    bgColor = new QColor(255, 255, 255, 200);


    // 窗口启动动画
    StartWindowOpacityAnimation->setDuration(1000);
    StartWindowOpacityAnimation->setStartValue(0);
    StartWindowOpacityAnimation->setEndValue(1);
    StartWindowOpacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartWindowPosAnimation->setDuration(1000);
    StartWindowPosAnimation->setStartValue(QPoint((DesktopWidget.width() - this->width()) / 2, (DesktopWidget.height() - this->height()) * 0.6));
    StartWindowPosAnimation->setEndValue(QPoint((DesktopWidget.width() - this->width()) / 2, (DesktopWidget.height() - this->height()) / 2));
    StartWindowPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartWindowAnimationGroup->addAnimation(StartWindowOpacityAnimation);
    StartWindowAnimationGroup->addAnimation(StartWindowPosAnimation);
    StartWindowAnimationGroup->start();
    // 等待1秒后播放动画
    QTimer::singleShot(1000, this, SLOT(StartAnimation()));
    // 字体
    QFont font;
    // 圆角变量
    QString borderRadius;

    // 按钮
    font.setPointSizeF(this->width() * 0.015);
    StartBtn->setGeometry((this->width() - this->width() * 0.15) * 0.2, (this->height() - this->height() * 0.1) * 0.9, this->width() * 0.15, this->height() * 0.1);
    AboutBtn->setGeometry((this->width() - this->width() * 0.15) / 2, (this->height() - this->height() * 0.1) * 0.9, this->width() * 0.15, this->height() * 0.1);
    ExitBtn->setGeometry((this->width() - this->width() * 0.15) * 0.8, (this->height() - this->height() * 0.1) * 0.9, this->width() * 0.15, this->height() * 0.1);
    BilibiliBtn->setGeometry(this->width() * 0.075, this->height() * 0.55, this->width() * 0.4, this->height() * 0.15);
    BilibiliBtn->setIcon(QIcon("img/bilibili-favicon.ico"));
    borderRadius = QString::number(StartBtn->height() * 0.495) + "px;";
    StartBtn->setStyleSheet("QPushButton {"
                            "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 red, stop: 1 #DF0000);"
                            "   border-radius: " + borderRadius + 
                            "}"
                            "QPushButton:hover {"
                            "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #DF0000, stop: 1 red);"
                            "}"
                            "QPushButton:pressed {"
                            "   background-color: red;"
                            "}"
                           );
    AboutBtn->setStyleSheet("border-radius: " + borderRadius);
    ExitBtn->setStyleSheet("QPushButton {"
                           "    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 white, stop: 1 rgb(240, 240, 240));"
                           "    border-radius: " + borderRadius + 
                           "}"
                           "QPushButton:hover {"
                           "    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 rgb(240, 240, 240), stop: 1 white);"
                           "}"
                           "QPushButton:pressed {"
                           "    background-color: white;"
                           "}"
                          );
    borderRadius = QString::number(BilibiliBtn->height() * 0.495) + "px;";
    BilibiliBtn->setStyleSheet("QPushButton {"
                               "    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #FF6699, stop: 1 #DF6699);"
                               "    border-radius: " + borderRadius + 
                               "}"
                               "QPushButton:hover {"
                               "    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 #DF6699, stop: 1 #FF6699);"
                               "}"
                               "QPushButton:pressed {"
                               "background-color: #FF6699;"
                               "}"
                              );
    AboutLICENSEBtn->setGeometry(this->width() * 0.9, this->height() * 0.2, this->width() * 0.05, this->height() * 0.5);
    StartBtn->setFont(font);
    AboutBtn->setFont(font);
    ExitBtn->setFont(font);
    BilibiliBtn->setFont(font);
    AboutLICENSEBtn->setFont(font);
    BackAboutBtn->setGeometry((this->width() - this->width() * 0.1) / 2, this->height() * 0.75, this->width() * 0.1, this->width() * 0.1);
    borderRadius = QString::number(BackAboutBtn->height() * 0.495) + "px;";
    BackAboutBtn->setStyleSheet("QPushButton {"
                                "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 white, stop: 1 rgb(240, 240, 240));"
                                "   border-radius: " + borderRadius + 
                                "}"
                                "QPushButton:hover {"
                                "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 rgb(240, 240, 240), stop: 1 white);"
                                "}"
                                "QPushButton:pressed {"
                                "   background-color: white;"
                                "}"
                               );
    font.setPointSizeF(BackAboutBtn->width() * 0.2);
    BackAboutBtn->setFont(font);
    borderRadius = QString::number(AboutLICENSEBtn->width() * 0.495) + "px;";
    AboutLICENSEBtn->setStyleSheet("QPushButton {"
                                   "    background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 white, stop: 1 rgb(240, 240, 240));"
                                   "    border-top-right-radius: " + borderRadius + 
                                   "    border-bottom-right-radius: " + borderRadius + 
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: -0, stop: 0 rgb(240, 240, 240), stop: 1 white);"
                                   "}"
                                   "QPushButton:pressed {"
                                   "background-color: white;"
                                   "}"
                                  );
    StartBtn->hide();
    AboutBtn->hide();
    ExitBtn->hide();
    BilibiliBtn->hide();
    AboutLICENSEBtn->hide();
    BackAboutBtn->hide();
    BackAboutBtn->setEnabled(false);

    // 标签
    StartPicLb->setPixmap(QPixmap("img/Disk-Squeezer_v2.0_icon.1024px.png"));
    StartPicLb->setGeometry((this->width() - this->height() * 0.5) / 2, (this->height() - this->height() * 0.5) / 2, this->height() * 0.5, this->height() * 0.5);
    StartPicLb->setScaledContents(true);
    StartPicLb->show();
    AboutTitleLb->setGeometry(this->width() * 0.1, this->height() * 0.05, this->width() * 0.8, this->height() * 0.1);
    AboutTitleLb->setAlignment(Qt::AlignCenter);
    AboutTitleLb->hide();
    font.setPixelSize(AboutTitleLb->height() * 0.8);
    AboutTitleLb->setFont(font);

    // 分组框
    borderRadius = QString::number(PathGb->height() * 0.945) + "px;";
    PathGb->setGeometry(this->width() * 0.075, this->height() * 0.05, this->width() * 0.4, this->height() * 0.15);
    ModeGb->setGeometry(this->width() * 0.075, this->height() * 0.25, this->width() * 0.4, this->height() * 0.3);
    CompleteEventGb->setGeometry(this->width() * 0.525, this->height() * 0.05, this->width() * 0.4, this->height() * 0.65);
    PathGb->setAlignment(Qt::AlignCenter);
    ModeGb->setAlignment(Qt::AlignCenter);
    CompleteEventGb->setAlignment(Qt::AlignCenter);
    font.setPixelSize(this->height() * 0.04);
    PathGb->setFont(font);
    ModeGb->setFont(font);
    CompleteEventGb->setFont(font);
    PathGb->setStyleSheet("border-radius:" + borderRadius);
    ModeGb->setStyleSheet("border-radius:" + borderRadius);
    CompleteEventGb->setStyleSheet("border-radius:" + borderRadius);
    PathGb->hide();
    ModeGb->hide();
    CompleteEventGb->hide();

    // 文本框
    AboutTxt->setGeometry(this->width() * 0.45, this->height() * 0.2, this->width() * 0.45, this->height() * 0.5);
    AboutTxt->setText(QString::fromLocal8Bit("程序名：硬盘榨干器(以下简称“该程序”或“本程序”)\n"
                                             "版本：2.0\n"
                                             "作者：龙ger_longer (也称：龙gerLonger, longer_longer, longlonger2022)\n"
                                             "该程序可以用于榨干您的硬盘，请谨慎使用。榨干后具体恢复方法请查看UP主(作者)的B站(或其他平台)视频，或者私信UP主(作者)以获取帮助。\n"
                                             "请勿将本程序恶意传播，否则后果自负。\n"
                                             "请谨慎使用本程序，UP主(作者)仅提供已知问题的解决方法，造成的其他任何后果UP主(作者)概不负责。\n"
                                             "若有任何问题请私信询问UP主，一段时间内可能得不到回复，请耐心等待。B站私信可较快获得回复，其他平台时间较长。想要更快地获得回复，可加入UP主(作者)的QQ交流群(群号：975059383，仅供交流与学习使用)以获取帮助。如果长时间未获得回复，可能的原因：UP主(作者)可能较忙、学业繁忙、突发情况等。\n"
                                             "本程序为开源程序，遵守GNU GPLv3开源协议，若需在本程序的基础上再加以制作，请遵守此协议。协议请点击右侧按钮查看\n"
                                             "本程序完全免费，严禁一切未经许可的盈利行为(如：倒卖，盗取等)\n"
                                             "QQ交流群(仅供学习和交流使用，请遵守群规)：975059383\n"
                                             "GitHub Pages网站：https://longlonger2022.github.io"
                                            )
                     );
    AboutTxt->setReadOnly(true);
    AboutTxt->hide();

    // 输入框
    PathEdt->setPlaceholderText(QString::fromLocal8Bit("请输入路径"));
    PathEdt->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9:\\\\._-]+"), PathEdt)); //限制输入
    PathEdt->setGeometry(PathGb->width() * 0.05, (PathGb->height() - PathGb->height() * 0.4) * 0.6, PathGb->width() * 0.9, PathGb->height() * 0.4);
    font.setPixelSize(PathEdt->height() * 0.7);
    PathEdt->setFont(font);
    borderRadius = QString::number(PathEdt->height() * 0.495) + "px;";
    PathEdt->setStyleSheet("border-radius:" + borderRadius);

    // 单选按钮
    WriteModeRb->setGeometry(ModeGb->width() * 0.05, (ModeGb->height() - ModeGb->height() * 0.4) * 0.3, ModeGb->width() * 0.9, ModeGb->height() * 0.4);
    CmdModeRb->setGeometry(ModeGb->width() * 0.05, (ModeGb->height() - ModeGb->height() * 0.4) * 0.8, ModeGb->width() * 0.9, ModeGb->height() * 0.4);
    CmdModeRb->setChecked(true);
    font.setPixelSize(ModeGb->width() * 0.06);
    WriteModeRb->setFont(font);
    CmdModeRb->setFont(font);
    AutoBlueScreenRb->setGeometry(CompleteEventGb->width() * 0.05, CompleteEventGb->height() * 0.15, CompleteEventGb->width() * 0.9, CompleteEventGb->height() * 0.1);
    AutoShutdownRb->setGeometry(CompleteEventGb->width() * 0.05, CompleteEventGb->height() * 0.35, CompleteEventGb->width() * 0.9, CompleteEventGb->height() * 0.1);
    AutoExitRb->setGeometry(CompleteEventGb->width() * 0.05, CompleteEventGb->height() * 0.55, CompleteEventGb->width() * 0.9, CompleteEventGb->height() * 0.1);
    NoEventRb->setGeometry(CompleteEventGb->width() * 0.05, CompleteEventGb->height() * 0.75, CompleteEventGb->width() * 0.9, CompleteEventGb->height() * 0.1);
    AutoExitRb->setChecked(true);
    font.setPixelSize(CompleteEventGb->width() * 0.06);
    AutoBlueScreenRb->setFont(font);
    AutoShutdownRb->setFont(font);
    AutoExitRb->setFont(font);
    NoEventRb->setFont(font);

    SimpleChineseRb->setGeometry(this->width() * 0.25, this->height() * 0.925, this->width() * 0.25, this->height() * 0.05);
    TraditionalChineseRb->setGeometry(this->width() * 0.45, this->height() * 0.925, this->width() * 0.25, this->height() * 0.05);
    EnglishRb->setGeometry(this->width() * 0.65, this->height() * 0.925, this->width() * 0.25, this->height() * 0.05);
    // 使语言单选按钮互斥
    QButtonGroup *LanguageGroup = new QButtonGroup(this);
    LanguageGroup->addButton(SimpleChineseRb);
    LanguageGroup->addButton(TraditionalChineseRb);
    LanguageGroup->addButton(EnglishRb);
    SimpleChineseRb->setChecked(true);

    // 系统托盘
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("img/Disk-Squeezer_v2.0_icon.1024px.png"));
    trayIcon->setToolTip(QString::fromLocal8Bit("硬盘榨干器"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(stTrayIconActive(QSystemTrayIcon::ActivationReason)));
    QMenu* menu = new QMenu();
    QAction* actionExit = new QAction(QString::fromLocal8Bit("退出"), menu);
    menu->addAction(actionExit);
    trayIcon->setContextMenu(menu);
    connect(actionExit, &QAction::triggered, qApp->quit);
    trayIcon->show();
    //trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("欢迎使用硬盘榨干器！"), QSystemTrayIcon::Information);


    // 按钮控制
    connect(ExitBtn, &QPushButton::clicked, qApp->quit);
    connect(BilibiliBtn, &QPushButton::clicked, [&] {QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3493110082439389")); });
    connect(AboutBtn, &QPushButton::clicked, this, &DiskSqueezer_v20::StartAboutAnimation);
    connect(BackAboutBtn, &QPushButton::clicked, this, &DiskSqueezer_v20::BackAboutAnimation);
    connect(AboutLICENSEBtn, &QPushButton::clicked, [&] {QDesktopServices::openUrl(QUrl("https://www.gnu.org/licenses/gpl-3.0.html#license-text")); });

    connect(CmdModeRb, &QPushButton::clicked, [&] { Mode = 1; });
    connect(WriteModeRb, &QPushButton::clicked, [&] { Mode = 0; });
    connect(AutoBlueScreenRb, &QPushButton::clicked, [&] { CompleteEvent = 4; });
    connect(AutoShutdownRb, &QPushButton::clicked, [&] { CompleteEvent = 3; });
    connect(AutoExitRb, &QPushButton::clicked, [&] { CompleteEvent = 2; });
    connect(NoEventRb, &QPushButton::clicked, [&] { CompleteEvent = 1; });
    
    connect(SimpleChineseRb, &QRadioButton::clicked, this, [=] {
        Lang = "zh_CN";
        this->setWindowTitle(QString::fromLocal8Bit("硬盘榨干器"));
        AboutTxt->setText(QString::fromLocal8Bit("程序名：硬盘榨干器(以下简称“该程序”或“本程序”)\n"
                                                 "版本：2.0\n"
                                                 "作者：龙ger_longer (也称：龙gerLonger, longer_longer, longlonger2022)\n"
                                                 "该程序可以用于榨干您的硬盘，请谨慎使用。榨干后具体恢复方法请查看UP主(作者)的B站(或其他平台)视频，或者私信UP主(作者)以获取帮助。\n"
                                                 "请勿将本程序恶意传播，否则后果自负。\n"
                                                 "请谨慎使用本程序，UP主(作者)仅提供已知问题的解决方法，造成的其他任何后果UP主(作者)概不负责。\n"
                                                 "若有任何问题请私信询问UP主，一段时间内可能得不到回复，请耐心等待。B站私信可较快获得回复，其他平台时间较长。想要更快地获得回复，可加入UP主(作者)的QQ交流群(群号：975059383，仅供交流与学习使用)以获取帮助。如果长时间未获得回复，可能的原因：UP主(作者)可能较忙、学业繁忙、突发情况等。\n"
                                                 "本程序为开源程序，遵守GNU GPLv3开源协议，若需在本程序的基础上再加以制作，请遵守此协议。协议请点击右侧按钮查看\n"
                                                 "本程序完全免费，严禁一切未经许可的盈利行为(如：倒卖，盗取等)\n"
                                                 "QQ交流群(仅供学习和交流使用，请遵守群规)：975059383\n"
                                                 "GitHub Pages网站：https://longlonger2022.github.io"
                                                )
                         );
        PathGb->setTitle(QString::fromLocal8Bit("路径"));
        ModeGb->setTitle(QString::fromLocal8Bit("模式"));
        CompleteEventGb->setTitle(QString::fromLocal8Bit("完成事件"));
        StartBtn->setText(QString::fromLocal8Bit("开始榨干"));
        AboutBtn->setText(QString::fromLocal8Bit("关于"));
        ExitBtn->setText(QString::fromLocal8Bit("退出"));
        BilibiliBtn->setText(QString::fromLocal8Bit("哔哩哔哩：龙ger_longer"));
        AutoBlueScreenRb->setText(QString::fromLocal8Bit("榨干后自动蓝屏(管理员)"));
        AutoShutdownRb->setText(QString::fromLocal8Bit("榨干后自动关机"));
        AutoExitRb->setText(QString::fromLocal8Bit("榨干后自动退出"));
        NoEventRb->setText(QString::fromLocal8Bit("无"));
        CmdModeRb->setText(QString::fromLocal8Bit("命令模式(高速)"));
        WriteModeRb->setText(QString::fromLocal8Bit("写入模式(低速)"));
        AboutLICENSEBtn->setText(QString::fromLocal8Bit("开\n源\n协\n议"));
        AboutTitleLb->setText(QString::fromLocal8Bit("关于"));
        trayIcon->setToolTip(QString::fromLocal8Bit("硬盘榨干器"));
        actionExit->setText(QString::fromLocal8Bit("退出"));
        StartNoteTitle = QString::fromLocal8Bit("提示");
        StartNoteText = QString::fromLocal8Bit("确定开始吗？\n"
                                               "点击确定即代表同意“关于”界面中文本框的内容，造成任何损失自负。\n"
                                               "路径：---\n"
                                               "模式：---\n"
                                               "完成事件：---"
                                              );
        });
    connect(TraditionalChineseRb, &QRadioButton::clicked, this, [=] {
        Lang = "zh_TW";
        this->setWindowTitle(QString::fromLocal8Bit("硬碟榨乾器"));
        AboutTxt->setText(QString::fromLocal8Bit("程式名：硬碟榨乾器（以下簡稱“該程式”或“本程式”）\n"
                                                 "版本：2.0\n作者：龙ger_longer（也稱：龙gerLonger， longer_longer， longlonger2022）\n"
                                                 "該程式可以用於榨乾您的硬碟，請謹慎使用。 榨乾后具體恢復方法請查看UP主（作者）的B站（或其他平臺）視頻，或者私信UP主（作者）以獲取説明。\n"
                                                 "請勿將本程序惡意傳播，否則後果自負。 \n"
                                                 "請謹慎使用本程式，UP主（作者）僅提供已知問題的解決方法，造成的其他任何後果UP主（作者）概不負責。\n"
                                                 "若有任何問題請私信詢問UP主，一段時間內可能得不到回復，請耐心等待。 B站私信可較快獲得回復，其他平台時間較長。 想要更快地獲得回復，可加入UP主（作者）的QQ交流群（群號：975059383，僅供交流與學習使用）以獲取説明。 如果長時間未獲得回復，可能的原因：UP主（作者）可能較忙、學業繁忙、突發情況等。\n"
                                                 "本程式為開源程式，遵守GNU GPLv3開源協定，若需在本程序的基礎上再加以製作，請遵守此協定。協定請點擊右側按鈕查看n本程式完全免費，嚴禁一切未經許可的盈利行為（如：倒賣，盜取等）\n"
                                                 "QQ交流群（僅供學習和交流使用，請遵守群規）：975059383\n"
                                                 "GitHub Pages網站：https://longlonger2022.github.io"
                                                )
                         );
        PathGb->setTitle(QString::fromLocal8Bit("路徑"));
        ModeGb->setTitle(QString::fromLocal8Bit("模式"));
        CompleteEventGb->setTitle(QString::fromLocal8Bit("完成事件"));
        StartBtn->setText(QString::fromLocal8Bit("開始榨乾"));
        AboutBtn->setText(QString::fromLocal8Bit("關於"));
        ExitBtn->setText(QString::fromLocal8Bit("退出"));
        BilibiliBtn->setText(QString::fromLocal8Bit("哔哩哔哩：龙ger_longer"));
        AutoBlueScreenRb->setText(QString::fromLocal8Bit("榨乾後自動藍屏(管理員)"));
        AutoShutdownRb->setText(QString::fromLocal8Bit("榨乾後自動關機"));
        AutoExitRb->setText(QString::fromLocal8Bit("榨乾後自動退出"));
        NoEventRb->setText(QString::fromLocal8Bit("無"));
        CmdModeRb->setText(QString::fromLocal8Bit("命令模式(高速)"));
        WriteModeRb->setText(QString::fromLocal8Bit("寫入模式(低速)"));
        AboutLICENSEBtn->setText(QString::fromLocal8Bit("開\n源\n協\n議"));
        AboutTitleLb->setText(QString::fromLocal8Bit("關於"));
        trayIcon->setToolTip(QString::fromLocal8Bit("硬碟榨乾器"));
        actionExit->setText(QString::fromLocal8Bit("退出"));
        StartNoteTitle = QString::fromLocal8Bit("提示");
        StartNoteText = QString::fromLocal8Bit("確定開始嗎？\n"
                                               "點擊確定即代表同意“關於”界面中文本框的內容，造成任何損失自負。\n"
                                               "路徑：---\n"
                                               "模式：---\n"
                                               "事件：---"
                                              );
        });
    connect(EnglishRb, &QRadioButton::clicked, this, [=] {
        Lang = "en";
        this->setWindowTitle("Disk-Squeezer");
        AboutTxt->setText(QString::fromLocal8Bit("Program Name: Disk-Squeezer (hereinafter referred to as \"the Program\" or \"this Program\")\n"
                                                 "Version: 2.0\nAuthor: 龙ger_longer (also known as: 龙gerLonger, longer_longer, longlonger2022)\n"
                                                 "This program can be used to make your disk space reduce, please use it with caution. For the specific recovery method after using, please check the content creator (Author)'s Bilibili (or other platforms) video, or send a private message to the content creator (Author) for help. \n"
                                                 "Please do not spread this program maliciously, otherwise you will bear the consequences. \n"
                                                 "Please use this program with caution, the content creator (Author) only provides solutions to known problems, and the content creator (Author) is not responsible for any other consequences caused. \n"
                                                 "If you have any questions, please ask the content creator (Author) by private message, you may not get a reply for a period of time, please be patient. Bilibili's private messages can be replied quickly, and other platforms take a long time. If you want to get a reply faster, you can join the QQ communication group (group number: 975059383, for communication and learning only) of the content creator (Author) for help. If you do not get a reply for a long time, the possible reasons: the content creator (Author) may be busy, busy with studies, unexpected situations, etc. \n"
                                                 "This program is open source and is licensed under the GNU GPLv3 open source license, and if you want to build on this program, please do so. Please click the button on the right to view. \n"
                                                 "This program is completely free, and all unauthorized profit-making behaviors (such as: reselling, stealing, etc.) are strictly prohibited\nQQ exchange group (only for learning and communication, please abide by the group rules): 975059383\n"
                                                 "GitHub Pages website: https://longlonger2022.github.io"
                                                )
                         );
        PathGb->setTitle("Path");
        ModeGb->setTitle("Mode");
        CompleteEventGb->setTitle("Complete Event");
        StartBtn->setText("Start");
        AboutBtn->setText("About");
        ExitBtn->setText("Exit");
        BilibiliBtn->setText(QString::fromLocal8Bit("Bilibili: 龙ger_longer"));
        AutoBlueScreenRb->setText("Auto Blue Screen (Admin)");
        AutoShutdownRb->setText("Auto Shutdown");
        AutoExitRb->setText("Auto Exit");
        NoEventRb->setText("None");
        CmdModeRb->setText("Command Mode (Fast)");
        WriteModeRb->setText("Write Mode (Slow)");
        AboutLICENSEBtn->setText("L\nI\nC\nE\nN\nS\nE");
        AboutTitleLb->setText("About");
        trayIcon->setToolTip("Disk-Squeezer");
        actionExit->setText("Exit");
        StartNoteTitle = "Note";
        StartNoteText = "Are you sure to start? \n"
                        "By clicking OK, you agree to the content of the box in Chinese on the \"About\" interface, and you will be responsible for any losses caused. \n"
                        "Path: ---\n"
                        "Mode: ---\n"
                        "Completion Event: ---";
        });

    connect(StartBtn, &QPushButton::clicked, this, [&] {
        SqueezePath = PathEdt->text();
        
        // 判断路径是否为空
        if (SqueezePath != "") {
            // 判断路径是否存在
            if (QDir(SqueezePath).exists()) {
                if (SqueezePath.size() >= 2) {
                    if (SqueezePath.at(SqueezePath.size() - 1) != "\\") {
                        SqueezePath += "\\";
                    }
                }
                StartNote.setWindowTitle(StartNoteTitle);
                StartNote.setIcon(QMessageBox::Icon(QMessageBox::Information));
                StartNote.setStyleSheet("QMessageBox {"
                                        "   background-color: white;"
                                        "   font: Microsoft Yahei UI, Microsoft Yahei, -apple-system, BlinkMacSystemFont, Segoe UI, Roboto, Ubuntu, Helvetica Neue, Helvetica, Arial,PingFang SC, Hiragino Sans GB, Microsoft YaHei UI, Microsoft YaHei, Source Han Sans CN, sans-serif;"
                                        "}"
                );
                StartNote.setWindowOpacity(0.9);
                if (Lang == "zh_CN") {
                    if (Mode == 1) {
                        if (CompleteEvent == 1) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                       "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                       "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                       "模式：命令模式\n"
                                                                                       "完成事件：无"
                                                                                       );
                        else if (CompleteEvent == 2) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自动退出"
                                                                                            );
                        else if (CompleteEvent == 3) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自动关机"
                                                                                            );
                        else if (CompleteEvent == 4) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自动蓝屏(管理员)"
                                                                                            );
                    }
                    else if (Mode == 0) {
                        if (CompleteEvent == 1) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                       "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                       "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                       "模式：写入模式\n"
                                                                                       "完成事件：无"
                                                                                       );
                        else if (CompleteEvent == 2) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：写入模式\n"
                                                                                            "完成事件：自动退出"
                                                                                            );
                        else if (CompleteEvent == 3) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：写入模式\n"
                                                                                            "完成事件：自动关机"
                                                                                            );
                        else if (CompleteEvent == 4) StartNoteText = QString::fromLocal8Bit("确定要开始吗？\n"
                                                                                            "点击确定，即表示您同意“关于”界面中文本框内的内容，并对此过程中产生的任何损失负责。\n"
                                                                                            "路径：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：写入模式\n"
                                                                                            "完成事件：自动蓝屏(管理员)"
                                                                                            );
                    }
                }
                else if (Lang == "zh_TW") {
                    if (Mode == 1) {
                        if (CompleteEvent == 1) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                       "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                       "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                       "模式：命令模式\n"
                                                                                       "完成事件：無"
                                                                                       );
                        else if (CompleteEvent == 2) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自動退出"
                                                                                            );
                        else if (CompleteEvent == 3) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自動關機"
                                                                                            );
                        else if (CompleteEvent == 4) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：命令模式\n"
                                                                                            "完成事件：自動藍屏(管理員)"
                                                                                            );
                    }
                    else if (Mode == 0) {
                        if (CompleteEvent == 1) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                       "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                       "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                       "模式：寫入模式\n"
                                                                                       "完成事件：無"
                                                                                       );
                        else if (CompleteEvent == 2) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：寫入模式\n"
                                                                                            "完成事件：自動退出"
                                                                                            );
                        else if (CompleteEvent == 3) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：寫入模式\n"
                                                                                            "完成事件：自動關機"
                                                                                            );
                        else if (CompleteEvent == 4) StartNoteText = QString::fromLocal8Bit("確定要開始嗎？\n"
                                                                                            "點擊確定，即表示您同意“關於”界面中文本框內的內容，並對此過程中產生的任何損失負責。\n"
                                                                                            "路徑：") + SqueezePath + QString::fromLocal8Bit("\n"
                                                                                            "模式：寫入模式\n"
                                                                                            "完成事件：自動藍屏(管理員)"
                                                                                            );
                    }
                }
                else if (Lang == "en") {
                    if (Mode == 1) {
                        if (CompleteEvent == 1) StartNoteText = "Are you sure you want to start?\n"
                                                                "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                "Path: " + SqueezePath + "\n"
                                                                "Mode: Command Mode\n"
                                                                "Complete Event: None";
                        else if (CompleteEvent == 2) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Command Mode\n"
                                                                     "Complete Event: Auto Exit";
                        else if (CompleteEvent == 3) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Command Mode\n"
                                                                     "Complete Event: Auto Shutdown";
                        else if (CompleteEvent == 4) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Command Mode\n"
                                                                     "Complete Event: Auto Blue Screen(Admin)";
                    }
                    else if (Mode == 0) {
                        if (CompleteEvent == 1) StartNoteText = "Are you sure you want to start?\n"
                                                                "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                "Path: " + SqueezePath + "\n"
                                                                "Mode: Write Mode\n"
                                                                "Complete Event: None";
                        else if (CompleteEvent == 2) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Write Mode\n"
                                                                     "Complete Event: Auto Exit";
                        else if (CompleteEvent == 3) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Write Mode\n"
                                                                     "Complete Event: Auto Shutdown";
                        else if (CompleteEvent == 4) StartNoteText = "Are you sure you want to start?\n"
                                                                     "Click Yes to indicate that you agree with the content in the text box of the \"About\" interface, and are responsible for any losses that may occur during this process.\n"
                                                                     "Path: " + SqueezePath + "\n"
                                                                     "Mode: Write Mode\n"
                                                                     "Complete Event: Auto Blue Screen(Admin)";
                    }
                }
                StartNote.setText(StartNoteText);
                StartNote.exec();
                if (StartNote.clickedButton() == StartNoteYesBtn) {
                    StartBtn->setDisabled(true);
                    AboutBtn->setDisabled(true);
                    ExitBtn->setDisabled(true);
                    PathGb->setDisabled(true);
                    ModeGb->setDisabled(true);
                    CompleteEventGb->setDisabled(true);
                    this->hide();
                    if (Mode == 1) {
                        CmdMode();
                    }
                    else if (Mode == 0) {
                        WriteMode();
                    }
                }
            }
            if (!QDir(SqueezePath).exists()) {
                if (Lang == "zh_CN") {
                    QMessageBox::critical(this, StartNoteTitle, QString::fromLocal8Bit("路径不存在"));
                }
                else if (Lang == "zh_TW") {
                    QMessageBox::critical(this, StartNoteTitle, QString::fromLocal8Bit("路徑不存在"));
                }
                else if (Lang == "en") {
                    QMessageBox::critical(this, StartNoteTitle, "The path does not exist");
                }
            }
        }
        else if (SqueezePath == "") {
            if (Lang == "zh_CN") {
                QMessageBox::critical(this, StartNoteTitle, QString::fromLocal8Bit("请输入路径"));
            }
            else if (Lang == "zh_TW") {
                QMessageBox::critical(this, StartNoteTitle, QString::fromLocal8Bit("請輸入路徑"));
            }
            else if (Lang == "en") {
                QMessageBox::critical(this, StartNoteTitle, "Please enter the path");
            }
        }
        
        });
}

DiskSqueezer_v20::~DiskSqueezer_v20()
{}


void DiskSqueezer_v20::StartAnimation() {
    StartBtn->show();
    AboutBtn->show();
    ExitBtn->show();
    BilibiliBtn->show();
    PathGb->show();
    ModeGb->show();
    CompleteEventGb->show();
    StartPicAnimation->setDuration(1000);
    StartPicAnimation->setStartValue(QPoint((this->width() - this->height() * 0.5) / 2, (this->height() - this->height() * 0.5) / 2));
    StartPicAnimation->setEndValue(QPoint((this->width() - this->height() * 0.5) / 2, ((this->height() - this->height() * 0.5) / 2) + this->height()));
    StartPicAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartStartBtnAnimation->setDuration(1000);
    StartStartBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) * 0.2, (this->height() - this->height() * 0.1) * 0.9 - this->height() - (this->height() - this->height() * 0.1) * 0.9));
    StartStartBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) * 0.2, (this->height() - this->height() * 0.1) * 0.9));
    StartStartBtnAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartAboutBtnAnimation->setDuration(1000);
    StartAboutBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) / 2, (this->height() - this->height() * 0.1) * 0.9 - this->height() - (this->height() - this->height() * 0.1) * 0.9));
    StartAboutBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) / 2, (this->height() - this->height() * 0.1) * 0.9));
    StartAboutBtnAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartExitBtnAnimation->setDuration(1000);
    StartExitBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) * 0.8, (this->height() - this->height() * 0.1) * 0.9 - this->height() - (this->height() - this->height() * 0.1) * 0.9));
    StartExitBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) * 0.8, (this->height() - this->height() * 0.1) * 0.9));
    StartExitBtnAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartBilibiliBtnAnimation->setDuration(1000);
    StartBilibiliBtnAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.55 - this->height() - this->height() * 0.55));
    StartBilibiliBtnAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.55));
    StartBilibiliBtnAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartPathGbAnimation->setDuration(1000);
    StartPathGbAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.05 - this->height() - this->height() * 0.05));
    StartPathGbAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.05));
    StartPathGbAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartModeGbAnimation->setDuration(1000);
    StartModeGbAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.25 - this->height() - this->height() * 0.25));
    StartModeGbAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.25));
    StartModeGbAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartCompleteEventGbAnimation->setDuration(1000);
    StartCompleteEventGbAnimation->setStartValue(QPoint(this->width() * 0.525, this->height() * 0.05 - this->height() - this->height() * 0.05));
    StartCompleteEventGbAnimation->setEndValue(QPoint(this->width() * 0.525, this->height() * 0.05));
    StartCompleteEventGbAnimation->setEasingCurve(QEasingCurve::OutCubic);
    StartAnimationGroup->addAnimation(StartPicAnimation);
    StartAnimationGroup->addAnimation(StartStartBtnAnimation);
    StartAnimationGroup->addAnimation(StartAboutBtnAnimation);
    StartAnimationGroup->addAnimation(StartExitBtnAnimation);
    StartAnimationGroup->addAnimation(StartBilibiliBtnAnimation);
    StartAnimationGroup->addAnimation(StartPathGbAnimation);
    StartAnimationGroup->addAnimation(StartModeGbAnimation);
    StartAnimationGroup->addAnimation(StartCompleteEventGbAnimation);
    StartAnimationGroup->start();
}

void DiskSqueezer_v20::StartAboutAnimation() {
    AboutTitleLb->show();
    AboutTxt->show();
    AboutLICENSEBtn->show();
    BackAboutBtn->show();
    BackAboutBtn->setEnabled(true);
    AboutBtn->setEnabled(false);
    StartAboutPicAnimation->setDuration(1000);
    StartAboutPicAnimation->setStartValue(QPoint((this->width() - this->height() * 0.5) / 2, ((this->height() - this->height() * 0.5) / 2) + this->height()));
    StartAboutPicAnimation->setEndValue(QPoint((this->width() - this->height() * 0.5) * 0.15, (this->height() - this->height() * 0.5) * 0.4));
    StartAboutPicAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutTitleLbAnimation->setDuration(1000);
    StartAboutTitleLbAnimation->setStartValue(QPoint((this->width() * 0.1) + this->width(), this->height() * 0.05));
    StartAboutTitleLbAnimation->setEndValue(QPoint(this->width() * 0.1, this->height() * 0.05));
    StartAboutTitleLbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutTxtAnimation->setDuration(1000);
    StartAboutTxtAnimation->setStartValue(QPoint((this->width() * 0.45) + this->width(), this->height() * 0.2));
    StartAboutTxtAnimation->setEndValue(QPoint(this->width() * 0.45, this->height() * 0.2));
    StartAboutTxtAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutLICENSEBtnAnimation->setDuration(1000);
    StartAboutLICENSEBtnAnimation->setStartValue(QPoint((this->width() * 0.9) + this->width(), this->height() * 0.2));
    StartAboutLICENSEBtnAnimation->setEndValue(QPoint(this->width() * 0.9, this->height() * 0.2));
    StartAboutLICENSEBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutBackAboutBtnAnimation->setDuration(1000);
    StartAboutBackAboutBtnAnimation->setStartValue(QPoint(((this->width() - this->width() * 0.1) / 2) + this->width(), this->height() * 0.75));
    StartAboutBackAboutBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.1) / 2, this->height() * 0.75));
    StartAboutBackAboutBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutStartBtnAnimation->setDuration(1000);
    StartAboutStartBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) * 0.2, (this->height() - this->height() * 0.1) * 0.9));
    StartAboutStartBtnAnimation->setEndValue(QPoint(((this->width() - this->width() * 0.15) * 0.2) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    StartAboutStartBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutAboutBtnAnimation->setDuration(1000);
    StartAboutAboutBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) / 2, (this->height() - this->height() * 0.1) * 0.9));
    StartAboutAboutBtnAnimation->setEndValue(QPoint(((this->width() - this->width() * 0.15) / 2) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    StartAboutAboutBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutExitBtnAnimation->setDuration(1000);
    StartAboutExitBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.15) * 0.8, (this->height() - this->height() * 0.1) * 0.9));
    StartAboutExitBtnAnimation->setEndValue(QPoint(((this->width() - this->width() * 0.15) * 0.8) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    StartAboutExitBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutBilibiliBtnAnimation->setDuration(1000);
    StartAboutBilibiliBtnAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.55));
    StartAboutBilibiliBtnAnimation->setEndValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.55));
    StartAboutBilibiliBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutPathGbAnimation->setDuration(1000);
    StartAboutPathGbAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.05));
    StartAboutPathGbAnimation->setEndValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.05));
    StartAboutPathGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutModeGbAnimation->setDuration(1000);
    StartAboutModeGbAnimation->setStartValue(QPoint(this->width() * 0.075, this->height() * 0.25));
    StartAboutModeGbAnimation->setEndValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.25));
    StartAboutModeGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutCompleteEventGbAnimation->setDuration(1000);
    StartAboutCompleteEventGbAnimation->setStartValue(QPoint(this->width() * 0.525, this->height() * 0.05));
    StartAboutCompleteEventGbAnimation->setEndValue(QPoint((this->width() * 0.525) - this->width(), this->height() * 0.05));
    StartAboutCompleteEventGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    StartAboutAnimationGroup->addAnimation(StartAboutPicAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutTitleLbAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutTxtAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutLICENSEBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutBackAboutBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutStartBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutAboutBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutExitBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutBilibiliBtnAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutPathGbAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutModeGbAnimation);
    StartAboutAnimationGroup->addAnimation(StartAboutCompleteEventGbAnimation);
    StartAboutAnimationGroup->start();
}

void DiskSqueezer_v20::BackAboutAnimation() {
    BackAboutBtn->setEnabled(false);
    AboutBtn->setEnabled(true);
    BackAboutPicAnimation->setDuration(1000);
    BackAboutPicAnimation->setStartValue(QPoint((this->width() - this->height() * 0.5) * 0.15, (this->height() - this->height() * 0.5) * 0.4));
    BackAboutPicAnimation->setEndValue(QPoint((this->width() - this->height() * 0.5) / 2, ((this->height() - this->height() * 0.5) / 2) + this->height()));
    BackAboutPicAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutTitleLbAnimation->setDuration(1000);
    BackAboutTitleLbAnimation->setStartValue(QPoint(this->width() * 0.1, this->height() * 0.05));
    BackAboutTitleLbAnimation->setEndValue(QPoint((this->width() * 0.1) + this->width(), this->height() * 0.05));
    BackAboutTitleLbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutTxtAnimation->setDuration(1000);
    BackAboutTxtAnimation->setStartValue(QPoint(this->width() * 0.45, this->height() * 0.2));
    BackAboutTxtAnimation->setEndValue(QPoint((this->width() * 0.45) + this->width(), this->height() * 0.2));
    BackAboutTxtAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutLICENSEBtnAnimation->setDuration(1000);
    BackAboutLICENSEBtnAnimation->setStartValue(QPoint(this->width() * 0.9, this->height() * 0.2));
    BackAboutLICENSEBtnAnimation->setEndValue(QPoint((this->width() * 0.9) + this->width(), this->height() * 0.2));
    BackAboutLICENSEBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutBackAboutBtnAnimation->setDuration(1000);
    BackAboutBackAboutBtnAnimation->setStartValue(QPoint((this->width() - this->width() * 0.1) / 2, this->height() * 0.75));
    BackAboutBackAboutBtnAnimation->setEndValue(QPoint(((this->width() - this->width() * 0.1) / 2) + this->width(), this->height() * 0.75));
    BackAboutBackAboutBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutStartBtnAnimation->setDuration(1000);
    BackAboutStartBtnAnimation->setStartValue(QPoint(((this->width() - this->width() * 0.15) * 0.2) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    BackAboutStartBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) * 0.2, (this->height() - this->height() * 0.1) * 0.9));
    BackAboutStartBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutAboutBtnAnimation->setDuration(1000);
    BackAboutAboutBtnAnimation->setStartValue(QPoint(((this->width() - this->width() * 0.15) / 2) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    BackAboutAboutBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) / 2, (this->height() - this->height() * 0.1) * 0.9));
    BackAboutAboutBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutExitBtnAnimation->setDuration(1000);
    BackAboutExitBtnAnimation->setStartValue(QPoint(((this->width() - this->width() * 0.15) * 0.8) - this->width(), (this->height() - this->height() * 0.1) * 0.9));
    BackAboutExitBtnAnimation->setEndValue(QPoint((this->width() - this->width() * 0.15) * 0.8, (this->height() - this->height() * 0.1) * 0.9));
    BackAboutExitBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutBilibiliBtnAnimation->setDuration(1000);
    BackAboutBilibiliBtnAnimation->setStartValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.55));
    BackAboutBilibiliBtnAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.55));
    BackAboutBilibiliBtnAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutPathGbAnimation->setDuration(1000);
    BackAboutPathGbAnimation->setStartValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.05));
    BackAboutPathGbAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.05));
    BackAboutPathGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutModeGbAnimation->setDuration(1000);
    BackAboutModeGbAnimation->setStartValue(QPoint((this->width() * 0.075) - this->width(), this->height() * 0.25));
    BackAboutModeGbAnimation->setEndValue(QPoint(this->width() * 0.075, this->height() * 0.25));
    BackAboutModeGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutCompleteEventGbAnimation->setDuration(1000);
    BackAboutCompleteEventGbAnimation->setStartValue(QPoint((this->width() * 0.525) - this->width(), this->height() * 0.05));
    BackAboutCompleteEventGbAnimation->setEndValue(QPoint(this->width() * 0.525, this->height() * 0.05));
    BackAboutCompleteEventGbAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    BackAboutAnimationGroup->addAnimation(BackAboutPicAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutTitleLbAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutTxtAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutLICENSEBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutBackAboutBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutStartBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutAboutBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutExitBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutBilibiliBtnAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutPathGbAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutModeGbAnimation);
    BackAboutAnimationGroup->addAnimation(BackAboutCompleteEventGbAnimation);
    BackAboutAnimationGroup->start();
}


void DiskSqueezer_v20::setBackgroundColor() {
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(245, 245, 245, 250));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}


void DiskSqueezer_v20::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);
    this->startX = event->x();
    this->startY = event->y();
}

void DiskSqueezer_v20::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);
    float disX = event->x() - this->startX;
    float disY = event->y() - this->startY;
    this->move(this->x() + disX, this->y() + disY);
}


void DiskSqueezer_v20::setGlassBackgroundR(int R)
{
    bgColor->setRed(R);


}
void DiskSqueezer_v20::setGlassBackgroundG(int G)
{
    bgColor->setGreen(G);

}
void DiskSqueezer_v20::setGlassBackgroundB(int B)
{
    bgColor->setBlue(B);

}
void DiskSqueezer_v20::setGlassBackgroundA(int A)
{
    bgColor->setAlpha(A);

}


void DiskSqueezer_v20::paintEvent(QPaintEvent* ev)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(*bgColor);
    //painter.drawRoundedRect(rect(), 20, 20);
    painter.drawRect(0, 0, width(), height());



}



// 命令模式函数
void DiskSqueezer_v20::CmdMode() {
    QFile cmdmodefile("cmdmode.bat");
    if (cmdmodefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        SetFileAttributes(cmdmodefile.fileName().toStdWString().c_str(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
        QByteArray data;
        data += "set /a size=1342177280\r\n"
                ":disk\r\n"
                "md " + QByteArray(SqueezePath.toUtf8()) + "squeezedisk\r\n"
                "attrib +s +h " + QByteArray(SqueezePath.toUtf8()) + "squeezedisk\r\n"
                ":start\r\n"
                "set /a file=%file%+1\r\n"
                "fsutil file createnew " + QByteArray(SqueezePath.toUtf8()) + "\\squeezedisk\\%file% %size%\r\n"
                "if not exist " + QByteArray(SqueezePath.toUtf8()) + "\\squeezedisk\\%file% (goto size)\r\n"
                "goto start\r\n"
                ":size\r\n"
                "if /i \"%size%\"==\"1\" goto exit\r\n"
                "if /i \"%size%\"==\"10\" set size=1\r\n"
                "if /i \"%size%\"==\"102\" set size=10\r\n"
                "if /i \"%size%\"==\"1024\" set size=102\r\n"
                "if /i \"%size%\"==\"10240\" set size=1024\r\n"
                "if /i \"%size%\"==\"102400\" set size=10240\r\n"
                "if /i \"%size%\"==\"131072\" set size=102400\r\n"
                "if /i \"%size%\"==\"1310720\" set size=131072\r\n"
                "if /i \"%size%\"==\"13107200\" set size=1310720\r\n"
                "if /i \"%size%\"==\"134217728\" set size=13107200\r\n"
                "if /i \"%size%\"==\"1342177280\" set size=134217728\r\n"
                "goto start\r\n"
                ":exit\r\n"
                "cls\r\n"
                "echo Done\r\n"
                "echo.\r\n";

        cmdmodefile.write(data);
        cmdmodefile.close();
        system("cmdmode.bat");
        remove("cmdmode.bat");
        StartBtn->setDisabled(false);
        AboutBtn->setDisabled(false);
        ExitBtn->setDisabled(false);
        PathGb->setDisabled(false);
        ModeGb->setDisabled(false);
        CompleteEventGb->setDisabled(false);
        if (CompleteEvent == 4) {
            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，即将自动蓝屏\n若未蓝屏，则未以管理员身份运行"), QSystemTrayIcon::Information);
            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，即將自動藍屏\n若未藍屏，則未以管理員身份運行"), QSystemTrayIcon::Information);
            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, about to automatically blue screennIf you are not blue screen, you are not running as administrator"), QSystemTrayIcon::Information);
            system("wininit");
            system("taskkill /f /im wininit.exe");
        }
        else if (CompleteEvent == 3) {
            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，即将自动关机"), QSystemTrayIcon::Information);
            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，即將自動關機"), QSystemTrayIcon::Information);
            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, it's about to shut down automatically"), QSystemTrayIcon::Information);
            system("shutdown -s -t 0");
        }
        else if (CompleteEvent == 2) {
            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，已自动退出"), QSystemTrayIcon::Information);
            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，已自動退出"), QSystemTrayIcon::Information);
            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, automatically exited"), QSystemTrayIcon::Information);
            qApp->quit();
        }
        else if (CompleteEvent == 1) {
            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成"), QSystemTrayIcon::Information);
            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成"), QSystemTrayIcon::Information);
            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done"), QSystemTrayIcon::Information);
            this->show();
        }
    }
    else {
        QMessageBox::critical(this, "ERROR", "Cannot to write to cmd mode file.");
        StartBtn->setDisabled(false);
        AboutBtn->setDisabled(false);
        ExitBtn->setDisabled(false);
        PathGb->setDisabled(false);
        ModeGb->setDisabled(false);
        CompleteEventGb->setDisabled(false);
    }
}

// 写入模式函数
void DiskSqueezer_v20::WriteMode() {
    qint64 bytesWritten;
    QDir squeezediskdir(SqueezePath + "squeezedisk");
    if (!squeezediskdir.exists()) {
        if (squeezediskdir.mkpath(SqueezePath + "squeezedisk")) {
            SetFileAttributes((SqueezePath.toStdWString() + L"squeezedisk").c_str(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
            QFile wtmodefile(SqueezePath + "squeezedisk\\squeezedisk.sqz");
            if (wtmodefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QByteArray data = "你好世界 Hello World ";
                while (true) {
                    bytesWritten = wtmodefile.write(data);
                    qApp->processEvents();
                    if (bytesWritten == -1) {
                        wtmodefile.close();
                        StartBtn->setDisabled(false);
                        AboutBtn->setDisabled(false);
                        ExitBtn->setDisabled(false);
                        PathGb->setDisabled(false);
                        ModeGb->setDisabled(false);
                        CompleteEventGb->setDisabled(false);
                        if (CompleteEvent == 4) {
                            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，即将自动蓝屏\n若未蓝屏，则未以管理员身份运行"), QSystemTrayIcon::Information);
                            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，即將自動藍屏\n若未藍屏，則未以管理員身份運行"), QSystemTrayIcon::Information);
                            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, about to automatically blue screennIf you are not blue screen, you are not running as administrator"), QSystemTrayIcon::Information);
                            system("wininit");
                            system("taskkill /f /im wininit.exe");
                        }
                        else if (CompleteEvent == 3) {
                            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，即将自动关机"), QSystemTrayIcon::Information);
                            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，即將自動關機"), QSystemTrayIcon::Information);
                            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, it's about to shut down automatically"), QSystemTrayIcon::Information);
                            system("shutdown -s -t 0");
                        }
                        else if (CompleteEvent == 2) {
                            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成，已自动退出"), QSystemTrayIcon::Information);
                            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成，已自動退出"), QSystemTrayIcon::Information);
                            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done, automatically exited"), QSystemTrayIcon::Information);
                            qApp->quit();
                        }
                        else if (CompleteEvent == 1) {
                            if (Lang == "zh_CN") trayIcon->showMessage(QString::fromLocal8Bit("硬盘榨干器"), QString::fromLocal8Bit("完成"), QSystemTrayIcon::Information);
                            else if (Lang == "zh_TW") trayIcon->showMessage(QString::fromLocal8Bit("硬碟榨乾器"), QString::fromLocal8Bit("完成"), QSystemTrayIcon::Information);
                            else if (Lang == "en") trayIcon->showMessage(QString::fromLocal8Bit("Disk-Squeezer"), QString::fromLocal8Bit("Done"), QSystemTrayIcon::Information);
                            this->show();
                        }
                        break;
                    }
                }
            }
            else {
                StartBtn->setDisabled(false);
                AboutBtn->setDisabled(false);
                ExitBtn->setDisabled(false);
                PathGb->setDisabled(false);
                ModeGb->setDisabled(false);
                CompleteEventGb->setDisabled(false);
                this->show();
                QMessageBox::critical(NULL, "ERROR", "Cannot to write to write mode file.");
            }
        }
        else {
            StartBtn->setDisabled(false);
            AboutBtn->setDisabled(false);
            ExitBtn->setDisabled(false);
            PathGb->setDisabled(false);
            ModeGb->setDisabled(false);
            CompleteEventGb->setDisabled(false);
            this->show();
            QMessageBox::critical(NULL, "ERROR", "Failed to create folder.");
        }
    }
    else {
        StartBtn->setDisabled(false);
        AboutBtn->setDisabled(false);
        ExitBtn->setDisabled(false);
        PathGb->setDisabled(false);
        ModeGb->setDisabled(false);
        CompleteEventGb->setDisabled(false);
        this->show();
        QMessageBox::critical(NULL, "ERROR", "Folder already exist.");
    }

}
