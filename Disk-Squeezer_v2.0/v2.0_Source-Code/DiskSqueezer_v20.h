#pragma once

#include <QtWidgets/QWidget>
#include "ui_DiskSqueezer_v20.h"
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QRegExp>
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTextEdit>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QtWin>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>

class DiskSqueezer_v20 : public QWidget
{
    Q_OBJECT

public:
    DiskSqueezer_v20(QWidget* parent = nullptr);
    ~DiskSqueezer_v20();

    void setGlassBackgroundR(int R);
    void setGlassBackgroundG(int G);
    void setGlassBackgroundB(int B);
    void setGlassBackgroundA(int A);


    void paintEvent(QPaintEvent* ev);


protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);


private:
    Ui::DiskSqueezer_v20Class ui;

    QColor* bgColor = nullptr;
    void setBackgroundColor();                  // 设置窗口背景颜色

    QString Lang = "zh_CN";
    bool Mode = 1;                          // 模式
    int CompleteEvent = 2;                  // 完成事件
    QString SqueezePath = "C:\\";
    QString StartNoteTitle = QString::fromLocal8Bit("提示");
    QString StartNoteText = QString::fromLocal8Bit("确定开始吗？\n点击确定即代表同意“关于”界面中文本框的内容，造成任何损失自负。\n路径：---\n模式：---\n完成事件：---");
    
    // 提示框
    QMessageBox StartNote;
    QPushButton* StartNoteYesBtn = StartNote.addButton(QString::fromLocal8Bit("确定"), QMessageBox::YesRole);
    QPushButton* StartNoteNoBtn = StartNote.addButton(QString::fromLocal8Bit("取消"), QMessageBox::NoRole);

    // 按钮
    QPushButton* StartBtn = new QPushButton(QString::fromLocal8Bit("开始榨干"), this);
    QPushButton* AboutBtn = new QPushButton(QString::fromLocal8Bit("关于"), this);
    QPushButton* BackAboutBtn = new QPushButton(QString::fromLocal8Bit("<"), this);
    QPushButton* ExitBtn = new QPushButton(QString::fromLocal8Bit("退出"), this);
    QPushButton* BilibiliBtn = new QPushButton(QString::fromLocal8Bit("哔哩哔哩：龙ger_longer"), this);
    QPushButton* AboutLICENSEBtn = new QPushButton(QString::fromLocal8Bit("开\n源\n协\n议"), this);

    // 标签
    QLabel* StartPicLb = new QLabel(this);
    QLabel* AboutTitleLb = new QLabel(QString::fromLocal8Bit("关于"), this);

    // 分组框
    QGroupBox* PathGb = new QGroupBox(QString::fromLocal8Bit("路径"), this);
    QGroupBox* ModeGb = new QGroupBox(QString::fromLocal8Bit("模式"), this);
    QGroupBox* CompleteEventGb = new QGroupBox(QString::fromLocal8Bit("完成事件"), this);

    // 输入框
    QLineEdit* PathEdt = new QLineEdit(QString::fromLocal8Bit("C:\\"), PathGb);

    // 文本框
    QTextEdit* AboutTxt = new QTextEdit(this);

    // 单选按钮
    QRadioButton* WriteModeRb = new QRadioButton(QString::fromLocal8Bit("写入模式(低速)"), ModeGb);
    QRadioButton* CmdModeRb = new QRadioButton(QString::fromLocal8Bit("命令模式(高速)"), ModeGb);
    QRadioButton* AutoBlueScreenRb = new QRadioButton(QString::fromLocal8Bit("榨干后自动蓝屏(管理员)"), CompleteEventGb);
    QRadioButton* AutoShutdownRb = new QRadioButton(QString::fromLocal8Bit("榨干后自动关机"), CompleteEventGb);
    QRadioButton* AutoExitRb = new QRadioButton(QString::fromLocal8Bit("榨干后自动退出"), CompleteEventGb);
    QRadioButton* NoEventRb = new QRadioButton(QString::fromLocal8Bit("无"), CompleteEventGb);

    QRadioButton* SimpleChineseRb = new QRadioButton(QString::fromLocal8Bit("简体中文"), this);
    QRadioButton* TraditionalChineseRb = new QRadioButton(QString::fromLocal8Bit("繁體中文"), this);
    QRadioButton* EnglishRb = new QRadioButton(QString::fromLocal8Bit("English"), this);

    // 系统托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);

    // 动画
    QPropertyAnimation* StartWindowOpacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    QPropertyAnimation* StartWindowPosAnimation = new QPropertyAnimation(this, "pos");
    QPropertyAnimation* StartPicAnimation = new QPropertyAnimation(StartPicLb, "pos");
    QPropertyAnimation* StartStartBtnAnimation = new QPropertyAnimation(StartBtn, "pos");
    QPropertyAnimation* StartAboutBtnAnimation = new QPropertyAnimation(AboutBtn, "pos");
    QPropertyAnimation* StartExitBtnAnimation = new QPropertyAnimation(ExitBtn, "pos");
    QPropertyAnimation* StartBilibiliBtnAnimation = new QPropertyAnimation(BilibiliBtn, "pos");
    QPropertyAnimation* StartPathGbAnimation = new QPropertyAnimation(PathGb, "pos");
    QPropertyAnimation* StartModeGbAnimation = new QPropertyAnimation(ModeGb, "pos");
    QPropertyAnimation* StartCompleteEventGbAnimation = new QPropertyAnimation(CompleteEventGb, "pos");

    QPropertyAnimation* StartAboutPicAnimation = new QPropertyAnimation(StartPicLb, "pos");
    QPropertyAnimation* StartAboutTitleLbAnimation = new QPropertyAnimation(AboutTitleLb, "pos");
    QPropertyAnimation* StartAboutTxtAnimation = new QPropertyAnimation(AboutTxt, "pos");
    QPropertyAnimation* StartAboutLICENSEBtnAnimation = new QPropertyAnimation(AboutLICENSEBtn, "pos");
    QPropertyAnimation* StartAboutBackAboutBtnAnimation = new QPropertyAnimation(BackAboutBtn, "pos");
    QPropertyAnimation* StartAboutStartBtnAnimation = new QPropertyAnimation(StartBtn, "pos");
    QPropertyAnimation* StartAboutAboutBtnAnimation = new QPropertyAnimation(AboutBtn, "pos");
    QPropertyAnimation* StartAboutExitBtnAnimation = new QPropertyAnimation(ExitBtn, "pos");
    QPropertyAnimation* StartAboutBilibiliBtnAnimation = new QPropertyAnimation(BilibiliBtn, "pos");
    QPropertyAnimation* StartAboutPathGbAnimation = new QPropertyAnimation(PathGb, "pos");
    QPropertyAnimation* StartAboutModeGbAnimation = new QPropertyAnimation(ModeGb, "pos");
    QPropertyAnimation* StartAboutCompleteEventGbAnimation = new QPropertyAnimation(CompleteEventGb, "pos");

    QPropertyAnimation* BackAboutPicAnimation = new QPropertyAnimation(StartPicLb, "pos");
    QPropertyAnimation* BackAboutTitleLbAnimation = new QPropertyAnimation(AboutTitleLb, "pos");
    QPropertyAnimation* BackAboutTxtAnimation = new QPropertyAnimation(AboutTxt, "pos");
    QPropertyAnimation* BackAboutLICENSEBtnAnimation = new QPropertyAnimation(AboutLICENSEBtn, "pos");
    QPropertyAnimation* BackAboutBackAboutBtnAnimation = new QPropertyAnimation(BackAboutBtn, "pos");
    QPropertyAnimation* BackAboutStartBtnAnimation = new QPropertyAnimation(StartBtn, "pos");
    QPropertyAnimation* BackAboutAboutBtnAnimation = new QPropertyAnimation(AboutBtn, "pos");
    QPropertyAnimation* BackAboutExitBtnAnimation = new QPropertyAnimation(ExitBtn, "pos");
    QPropertyAnimation* BackAboutBilibiliBtnAnimation = new QPropertyAnimation(BilibiliBtn, "pos");
    QPropertyAnimation* BackAboutPathGbAnimation = new QPropertyAnimation(PathGb, "pos");
    QPropertyAnimation* BackAboutModeGbAnimation = new QPropertyAnimation(ModeGb, "pos");
    QPropertyAnimation* BackAboutCompleteEventGbAnimation = new QPropertyAnimation(CompleteEventGb, "pos");


    // 动画组
    QParallelAnimationGroup* StartWindowAnimationGroup = new QParallelAnimationGroup(this);
    QParallelAnimationGroup* StartAnimationGroup = new QParallelAnimationGroup(this);
    QParallelAnimationGroup* StartAboutAnimationGroup = new QParallelAnimationGroup(this);
    QParallelAnimationGroup* BackAboutAnimationGroup = new QParallelAnimationGroup(this);


private:
    float startX;                               // 这两个变量用来移动窗口
    float startY;

private slots:
    void StartAnimation();
    void StartAboutAnimation();
    void BackAboutAnimation();
    void CmdMode();
    void WriteMode();
};


typedef enum _WINDOWCOMPOSITIONATTRIB
{
    CA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,//
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_LAST = 23
} WINDOWCOMPOSITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef enum _ACCENT_STATE
{
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_INVALID_STATE = 4
} ACCENT_STATE;

typedef struct _ACCENT_POLICY
{
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
} ACCENT_POLICY;

WINUSERAPI
BOOL
WINAPI
GetWindowCompositionAttribute(
    _In_ HWND hWnd,
    _Inout_ WINDOWCOMPOSITIONATTRIBDATA* pAttrData);

typedef BOOL(WINAPI* pfnGetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

WINUSERAPI
BOOL
WINAPI
SetWindowCompositionAttribute(
    _In_ HWND hWnd,
    _Inout_ WINDOWCOMPOSITIONATTRIBDATA* pAttrData);

typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);
