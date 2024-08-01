#include "DiskSqueezer_v2Beta.h"
#include<bits/stdc++.h>
#include<Windows.h>
#include<codecvt>
#include<functional>
#include<QPushButton>
#include<QCheckbox>
#include<QRadioButton>
#include<QGroupBox>
#include<QProgressBar>
#include<QLineEdit>
#include<QFileDialog>
#include<QValidator>
#include<QRegExp>
#include<QWidget>
#include<QLabel>
#include<QDesktopServices>
#include<QMainWindow>
#include<QFile>
#include<QTextStream>
#include<QThread>
#include<QObject>
#include<QTimer>

LPCSTR StdStringToLPCSTR(const std::string& str);
const std::string& WstringToConstStringRef(const std::wstring& wstr);



int space = 0, percentage = 0;
bool issqueezedry = false;
bool issqueezeerror = false;


DiskSqueezer_v2Beta::DiskSqueezer_v2Beta(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //����
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Disk-Squeezer");
    this->setWindowIcon(QIcon("images/icon/Disk-Squeezer.256px.ico"));
    this->setFont(QFont("Microsoft YaHei UI"));
    this->setFixedSize(800, 360);
    this->setGeometry(590, 360, 800, 360);

    //�������
    bool mode = 1;
    bool ifclickedrbmode = 0, ifclickedrbcompleteevent = 0;
    int completeevent = 3;
    squeezepathtofunc = "C:\\";

    //�����
    gb_path = new QGroupBox(this);
    gb_path->setGeometry(40, 40, 355, 100);
    gb_path->setTitle(QString::fromLocal8Bit("·��"));
    gb_path->show();
    gb_mode = new QGroupBox(this);
    gb_mode->setGeometry(40, 150, 355, 100);
    gb_mode->setTitle(QString::fromLocal8Bit("ģʽ"));
    gb_mode->show();
    gb_completeevent = new QGroupBox(this);
    gb_completeevent->setGeometry(405, 40, 355, 210);
    gb_completeevent->setTitle(QString::fromLocal8Bit("����¼�"));
    gb_completeevent->show();

    //�����
    le_path = new QLineEdit(gb_path);
    le_path->setGeometry(10, 20, 335, 30);
    le_path->setPlaceholderText(QString::fromLocal8Bit("������·��"));
    le_path->show();
    le_path->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9:\\\\._-]+"), le_path));

    //��ѡ��
    //ģʽ��ѡ��
    rb_wt = new QRadioButton(gb_mode);
    rb_wt->setGeometry(10, 25, 335, 30);
    rb_wt->setText(QString::fromLocal8Bit("д��ģʽ(�ٶ��������ɶ�Ӳ�������Ч�˺�)"));
    rb_wt->show();
    rb_cmd = new QRadioButton(gb_mode);
    rb_cmd->setGeometry(10, 55, 335, 30);
    rb_cmd->setText(QString::fromLocal8Bit("����ģʽ(�ٶȿ죬���ɶ�Ӳ�������Ч�˺�)"));
    rb_cmd->show();
    //�¼���ѡ��
    rb_autobluescreen = new QRadioButton(gb_completeevent);
    rb_autobluescreen->setGeometry(10, 30, 335, 30);
    rb_autobluescreen->setText(QString::fromLocal8Bit("ե�ɺ��Զ�����(��Ҫ�Թ���Ա�������)"));
    rb_autobluescreen->show();
    rb_autoshutdown = new QRadioButton(gb_completeevent);
    rb_autoshutdown->setGeometry(10, 70, 335, 30);
    rb_autoshutdown->setText(QString::fromLocal8Bit("ե�ɺ��Զ��ػ�"));
    rb_autoshutdown->show();
    rb_autoexit = new QRadioButton(gb_completeevent);
    rb_autoexit->setGeometry(10, 110, 335, 30);
    rb_autoexit->setText(QString::fromLocal8Bit("ե�ɺ��Զ��˳�"));
    rb_autoexit->show();
    rb_none = new QRadioButton(gb_completeevent);
    rb_none->setGeometry(10, 150, 335, 30);
    rb_none->setText(QString::fromLocal8Bit("��"));
    rb_none->show();

    //������
    pb_squeezer = new QProgressBar(this);
    pb_squeezer->setGeometry(40, 270, 720, 30);
    pb_squeezer->setRange(0, 100);
    pb_squeezer->setValue(0);
    pb_squeezer->hide();

    //��ť
    btn_selectpath = new QPushButton(gb_path);
    btn_selectpath->setGeometry(10, 60, 200, 30);
    btn_selectpath->setText(QString::fromLocal8Bit("ѡ��·��(�ݲ�����)"));
    btn_selectpath->show();
    btn_startsqueeze = new QPushButton(this);
    btn_startsqueeze->setGeometry(150, 280, 100, 40);
    btn_startsqueeze->setText(QString::fromLocal8Bit("��ʼե��"));
    btn_startsqueeze->show();
    btn_about = new QPushButton(this);
    btn_about->setGeometry(350, 280, 100, 40);
    btn_about->setText(QString::fromLocal8Bit("����"));
    btn_about->show();
    btn_exit = new QPushButton(this);
    btn_exit->setGeometry(550, 280, 100, 40);
    btn_exit->setText(QString::fromLocal8Bit("�˳�"));
    btn_exit->show();

    //��ť����
    //ѡ��·��(δ���)
    QObject::connect(btn_selectpath, &QPushButton::clicked, this, [&] {
        
        });
    //����
    QObject::connect(btn_about, &QPushButton::clicked, this, [&] {
        //����
        QWidget* aboutWidget = new QWidget();
        aboutWidget->setWindowModality(Qt::ApplicationModal);
        aboutWidget->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        aboutWidget->setWindowTitle(QString::fromLocal8Bit("����"));
        aboutWidget->setWindowIcon(QIcon("images/icon/Disk-Squeezer.256px.ico"));
        aboutWidget->setFont(QFont("Microsoft YaHei UI"));
        aboutWidget->setFixedSize(600, 700);
        aboutWidget->setGeometry(690, 190, 600, 700);
        aboutWidget->show();
        //�ı���ǩ
        QLabel* aboutLabel_title = new QLabel(aboutWidget);
        aboutLabel_title->setGeometry(20, 290, 560, 30);
        aboutLabel_title->setFont(QFont("Microsoft YaHei UI", 15));
        aboutLabel_title->setText(QString::fromLocal8Bit("���� Disk-Squeezer 2.0-Beta"));
        aboutLabel_title->setAlignment(Qt::AlignCenter);
        aboutLabel_title->show();
        QLabel* aboutLabel_body = new QLabel(aboutWidget);
        aboutLabel_body->setGeometry(20, 320, 560, 140);
        aboutLabel_body->setFont(QFont("Microsoft YaHei UI", 10));
        aboutLabel_body->setText(QString::fromLocal8Bit("��������Ӳ��ե����\nEnglish Name: Disk-Squeezer\n�汾��2.0 Beta\n���ߣ���ger_longer(��gerLonger, longer_longer, longlonger2022)\n�ó����ե������Ӳ�̣���С��ʹ��\n��Դ�����Ͻ�һ��δ�������ӯ����Ϊ(�磺��������ȡ��)"));
        aboutLabel_body->setAlignment(Qt::AlignCenter);
        aboutLabel_body->show();
        //ͼƬ��ǩ
        QLabel* aboutLabel_icon = new QLabel(aboutWidget);
        aboutLabel_icon->setGeometry(172, 20, 256, 256);
        aboutLabel_icon->setPixmap(QPixmap("images/icon/Disk-Squeezer.256px.ico"));
        aboutLabel_icon->setScaledContents(true);
        aboutLabel_icon->show();
        //��ť
        QPushButton* aboutbtn_GithubWebsite = new QPushButton(aboutWidget);
        aboutbtn_GithubWebsite->setGeometry(20, 470, 180, 30);
        aboutbtn_GithubWebsite->setText(QString::fromLocal8Bit("GitHub.io��վ"));
        aboutbtn_GithubWebsite->setIcon(QPixmap("images/icon/Disk-Squeezer.256px.ico"));
        aboutbtn_GithubWebsite->show();
        QPushButton* aboutbtn_GithubProfile = new QPushButton(aboutWidget);
        aboutbtn_GithubProfile->setGeometry(210, 470, 180, 30);
        aboutbtn_GithubProfile->setText(QString::fromLocal8Bit("GitHub������ҳ"));
        aboutbtn_GithubProfile->setIcon(QPixmap("images/btnimg/Github-Dark.png"));
        aboutbtn_GithubProfile->show();
        QPushButton* aboutbtn_GithubRepositorie = new QPushButton(aboutWidget);
        aboutbtn_GithubRepositorie->setGeometry(400, 470, 180, 30);
        aboutbtn_GithubRepositorie->setText(QString::fromLocal8Bit("GitHub�ֿ�"));
        aboutbtn_GithubRepositorie->setIcon(QPixmap("images/btnimg/Github-Dark.png"));
        aboutbtn_GithubRepositorie->show();
        QPushButton* aboutbtn_BilibiliSpace = new QPushButton(aboutWidget);
        aboutbtn_BilibiliSpace->setGeometry(20, 505, 275, 30);
        aboutbtn_BilibiliSpace->setText(QString::fromLocal8Bit("Bilibili�ռ�"));
        aboutbtn_BilibiliSpace->setIcon(QPixmap("images/btnimg/bilibili-icon.jpg"));
        aboutbtn_BilibiliSpace->show();
        QPushButton* aboutbtn_BilibiliCollection = new QPushButton(aboutWidget);
        aboutbtn_BilibiliCollection->setGeometry(305, 505, 275, 30);
        aboutbtn_BilibiliCollection->setText(QString::fromLocal8Bit("Bilibili�ϼ�"));
        aboutbtn_BilibiliCollection->setIcon(QPixmap("images/btnimg/bilibili-icon.jpg"));
        aboutbtn_BilibiliCollection->show();
        QPushButton* aboutbtn_DouyinProfile = new QPushButton(aboutWidget);
        aboutbtn_DouyinProfile->setGeometry(20, 540, 132, 30);
        aboutbtn_DouyinProfile->setText(QString::fromLocal8Bit("����"));
        aboutbtn_DouyinProfile->setIcon(QPixmap("images/btnimg/douyin-icon.png"));
        aboutbtn_DouyinProfile->show();
        QPushButton* aboutbtn_XiguaProfile = new QPushButton(aboutWidget);
        aboutbtn_XiguaProfile->setGeometry(162, 540, 133, 30);
        aboutbtn_XiguaProfile->setText(QString::fromLocal8Bit("������Ƶ"));
        aboutbtn_XiguaProfile->setIcon(QPixmap("images/btnimg/xigua-icon.jpg"));
        aboutbtn_XiguaProfile->show();
        QPushButton* aboutbtn_ToutiaoProfile = new QPushButton(aboutWidget);
        aboutbtn_ToutiaoProfile->setGeometry(305, 540, 133, 30);
        aboutbtn_ToutiaoProfile->setText(QString::fromLocal8Bit("����ͷ��"));
        aboutbtn_ToutiaoProfile->setIcon(QPixmap("images/btnimg/toutiao-icon.png"));
        aboutbtn_ToutiaoProfile->show();
        QPushButton* aboutbtn_KuaishouProfile = new QPushButton(aboutWidget);
        aboutbtn_KuaishouProfile->setGeometry(448, 540, 132, 30);
        aboutbtn_KuaishouProfile->setText(QString::fromLocal8Bit("����"));
        aboutbtn_KuaishouProfile->setIcon(QPixmap("images/btnimg/kuaishou-icon.png"));
        aboutbtn_KuaishouProfile->show();
        QPushButton* aboutbtn_ZCZOfficial = new QPushButton(aboutWidget);
        aboutbtn_ZCZOfficial->setGeometry(20, 575, 275, 30);
        aboutbtn_ZCZOfficial->setText(QString::fromLocal8Bit("ZCZ�����ҹ���"));
        aboutbtn_ZCZOfficial->setIcon(QPixmap("images/btnimg/zczblog-icon.png"));
        aboutbtn_ZCZOfficial->show();
        QPushButton* aboutbtn_ZCZBlog = new QPushButton(aboutWidget);
        aboutbtn_ZCZBlog->setGeometry(305, 575, 275, 30);
        aboutbtn_ZCZBlog->setText(QString::fromLocal8Bit("ZCZ����"));
        aboutbtn_ZCZBlog->setIcon(QPixmap("images/btnimg/zczblog-icon.png"));
        aboutbtn_ZCZBlog->show();
        QPushButton* aboutbtn_ok = new QPushButton(aboutWidget);
        aboutbtn_ok->setGeometry(250, 640, 100, 30);
        aboutbtn_ok->setText(QString::fromLocal8Bit("ȷ��"));
        aboutbtn_ok->show();
        //��ť����
        QObject::connect(aboutbtn_GithubWebsite, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://longlonger2022.github.io"));
            });
        QObject::connect(aboutbtn_GithubProfile, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://github.com/longlonger2022"));
            });
        QObject::connect(aboutbtn_GithubRepositorie, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://github.com/longlonger2022/Disk-Squeezer"));
            });
        QObject::connect(aboutbtn_BilibiliSpace, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3493110082439389"));
            });
        QObject::connect(aboutbtn_BilibiliCollection, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3493110082439389/channel/series"));
            });
        QObject::connect(aboutbtn_DouyinProfile, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://www.douyin.com/user/MS4wLjABAAAALLOTkiJC-CS7B7O2qETspuEiTEy9gg4DMLvd7dKxdIs"));
            });
        QObject::connect(aboutbtn_XiguaProfile, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://www.ixigua.com/home/308276834862408"));
            });
        QObject::connect(aboutbtn_ToutiaoProfile, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://www.toutiao.com/c/user/token/MS4wLjABAAAAxkLNY3Z7gbYJdfK0zdDK7uKG-jKFtMxmFeBk_M_yl3w"));
            });
        QObject::connect(aboutbtn_KuaishouProfile, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://www.kuaishou.com/profile/3x2kzmpqxkh9ngk"));
            });
        QObject::connect(aboutbtn_ZCZOfficial, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://www.imzcz.cn"));
            });
        QObject::connect(aboutbtn_ZCZBlog, &QPushButton::clicked, this, [&] {
            QDesktopServices::openUrl(QUrl("https://blog.imzcz.cn"));
            });
        QObject::connect(aboutbtn_ok, &QPushButton::clicked, aboutWidget, &QWidget::close);
        });
    //��ʼե��
    connect(btn_startsqueeze, &QPushButton::clicked, this, [&] {
        squeezepathtofunc = le_path->text();
        
        if (ifclickedrbmode == 1 && ifclickedrbcompleteevent == 1) {
            if (squeezepathtofunc != "") {
                if (squeezepathtofunc.contains(":") && squeezepathtofunc.contains("\\") && squeezepathtofunc.back()=="\\") {
                    if (mode==1){
                        btn_startsqueeze->setDisabled(true);
                        btn_selectpath->setDisabled(true);
                        le_path->setDisabled(true);
                        rb_wt->setDisabled(true);
                        rb_cmd->setDisabled(true);
                        rb_autobluescreen->setDisabled(true);
                        rb_autoshutdown->setDisabled(true);
                        rb_autoexit->setDisabled(true);
                        rb_none->setDisabled(true);
                        QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), squeezepathtofunc, QMessageBox::Ok);
                        cmd(squeezepathtofunc, completeevent);
                    }
                    else if(mode==0){
                        coverWidget();
                        std::thread* wtthread = new std::thread([&]() {
                            wt(squeezepathtofunc, completeevent);
                            });
                        wtthread->detach();
                        //updatepb(squeezepathtofunc);(δ����)
                    }
                }
                else QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��������Ч·��"), QMessageBox::Ok);
            }
            else QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��������Ч·��"), QMessageBox::Ok);
        }
        else QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ��ģʽ������¼�"), QMessageBox::Ok);
        
        });
    //�˳�
    QObject::connect(btn_exit, &QPushButton::clicked, qApp, &qApp->quit);
    //��ѡ��ѡ��
    QObject::connect(rb_wt, &QRadioButton::clicked, this, [&] {
        mode = 0;
        ifclickedrbmode = 1;
        this->setFixedSize(800, 400);
        pb_squeezer->show();
        btn_startsqueeze->setGeometry(150, 330, 100, 40);
        btn_about->setGeometry(350, 330, 100, 40);
        btn_exit->setGeometry(550, 330, 100, 40);
        });
    QObject::connect(rb_cmd, &QRadioButton::clicked, this, [&] {
        mode = 1;
        ifclickedrbmode = 1;
        this->setFixedSize(800, 360);
        pb_squeezer->hide();
        btn_startsqueeze->setGeometry(150, 280, 100, 40);
        btn_about->setGeometry(350, 280, 100, 40);
        btn_exit->setGeometry(550, 280, 100, 40);
        });
    QObject::connect(rb_autobluescreen, &QRadioButton::clicked, this, [&] {
        completeevent = 1;
        ifclickedrbcompleteevent = 1;
        });
    QObject::connect(rb_autoshutdown, &QRadioButton::clicked, this, [&] {
        completeevent = 2;
        ifclickedrbcompleteevent = 1;
        });
    QObject::connect(rb_autoexit, &QRadioButton::clicked, this, [&] {
        completeevent = 3;
        ifclickedrbcompleteevent = 1;
        });
    QObject::connect(rb_none, &QRadioButton::clicked, this, [&] {
        completeevent = 0;
        ifclickedrbcompleteevent = 1;
        });
}

DiskSqueezer_v2Beta::~DiskSqueezer_v2Beta()
{}

void DiskSqueezer_v2Beta::coverWidget() {
    btn_startsqueeze->setDisabled(true);
    btn_selectpath->setDisabled(true);
    rb_wt->setDisabled(true);
    rb_cmd->setDisabled(true);
    rb_autobluescreen->setDisabled(true);
    rb_autoshutdown->setDisabled(true);
    rb_autoexit->setDisabled(true);
    rb_none->setDisabled(true);
}
void DiskSqueezer_v2Beta::recoverWidget() {
    btn_startsqueeze->setDisabled(false);
    btn_selectpath->setDisabled(false);
    rb_wt->setDisabled(false);
    rb_cmd->setDisabled(false);
    rb_autobluescreen->setDisabled(false);
    rb_autoshutdown->setDisabled(false);
    rb_autoexit->setDisabled(false);
    rb_none->setDisabled(false);
}


void DiskSqueezer_v2Beta::updatepb(QString squeezepath) {
    DWORD64 qwFreeBytesToCaller = 0;
    DWORD64        qwTotalBytes = 0;
    DWORD64         qwFreeBytes = 0;
    BOOL                bResult = GetDiskFreeSpaceExA(StdStringToLPCSTR(squeezepath.left(2).toStdString()), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
    DWORD64           freebytes = qwFreeBytes;
    while (issqueezedry == false) {
        bResult = GetDiskFreeSpaceExA(StdStringToLPCSTR(squeezepath.left(2).toStdString()), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
        percentage = 100 - ((freebytes * 1) / (qwFreeBytes * 1) * 100);
        pb_squeezer->setValue(percentage);
    }
    recoverWidget();
}


//д��ģʽ����
void DiskSqueezer_v2Beta::wt(QString squeezepath, int cpevent)
{
    issqueezedry = false;
    qint64 bytesWritten;
    DWORD64 qwFreeBytesToCaller = 0;
    DWORD64 qwTotalBytes        = 0;
    DWORD64 qwFreeBytes         = 0;
    BOOL    bResult             = GetDiskFreeSpaceExA(StdStringToLPCSTR(squeezepath.left(2).toStdString()), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
    QDir squeezediskdir(squeezepath + "squeezedisk");
    if (!squeezediskdir.exists()) {
        if (squeezediskdir.mkpath(squeezepath + "squeezedisk")) {
            QFile wtmodefile(squeezepath + "squeezedisk\\squeezedisk.dll");
            if (wtmodefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QByteArray data = " ";
                while (true) {
                    bytesWritten = wtmodefile.write(data);
                    
                    if (bytesWritten == -1) {
                        wtmodefile.close();
                        issqueezedry = true;
                        if (cpevent == 1) {
                            system("wininit");
                        }
                        else if (cpevent == 2) {
                            system("shutdown -s -t 0");
                        }
                        else if (cpevent == 3) {
                            qApp->quit();
                        }
                        else if (cpevent == 4) {
                            QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ե�����"));
                        }
                        break;
                    }
                    space += 2;
                }
                
            }
            else {
                QMessageBox::critical(NULL, "ERROR", "Cannot to write to write mode file.");
            }
        }
        else {
            QMessageBox::critical(NULL, "ERROR", "Failed to create folder.");
        }
    }
    else {
        QMessageBox::critical(NULL, "ERROR", "Folder already exist.");
    }
    
}

//����ģʽ����
void DiskSqueezer_v2Beta::cmd(QString squeezepath, int cpevent)
{
    
    QFile cmdmodefile("cmdmode.bat");
    if (cmdmodefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QByteArray data;
        data += "set /a size=1342177280\r\n";
        data += ":disk\r\n";
        data += "md " + QByteArray(squeezepath.toUtf8()) + "squeezedisk\r\n";
        data += "attrib +s +h " + QByteArray(squeezepath.toUtf8()) + "squeezedisk\r\n";
        data += ":start\r\n";
        data += "set /a file=%file%+1\r\n";
        data += "fsutil file createnew " + QByteArray(squeezepath.toUtf8()) + "\\squeezedisk\\%file% %size%\r\n";
        data += "if not exist " + QByteArray(squeezepath.toUtf8()) + "\\squeezedisk\\%file% (goto size)\r\n";
        data += "goto start\r\n";
        data += ":size\r\n";
        data += "if /i \"%size%\"==\"1\" goto exit\r\n";
        data += "if /i \"%size%\"==\"10\" set size=1\r\n";
        data += "if /i \"%size%\"==\"102\" set size=10\r\n";
        data += "if /i \"%size%\"==\"1024\" set size=102\r\n";
        data += "if /i \"%size%\"==\"10240\" set size=1024\r\n";
        data += "if /i \"%size%\"==\"102400\" set size=10240\r\n";
        data += "if /i \"%size%\"==\"131072\" set size=102400\r\n";
        data += "if /i \"%size%\"==\"1310720\" set size=131072\r\n";
        data += "if /i \"%size%\"==\"13107200\" set size=1310720\r\n";
        data += "if /i \"%size%\"==\"134217728\" set size=13107200\r\n";
        data += "if /i \"%size%\"==\"1342177280\" set size=134217728\r\n";
        data += "goto start\r\n";
        data += ":exit\r\n";
        data += "cls\r\n";
        data += "echo ե�����\r\n";
        data += "echo.\r\n";
        data += "timeout /t 1 /nobreak\r\n";

        cmdmodefile.write(data);
        cmdmodefile.close();
        system("cmdmode.bat");
        remove("cmdmode.bat");
        btn_startsqueeze->setDisabled(false);
        btn_selectpath->setDisabled(false);
        le_path->setDisabled(false);
        rb_wt->setDisabled(false);
        rb_cmd->setDisabled(false);
        rb_autobluescreen->setDisabled(false);
        rb_autoshutdown->setDisabled(false);
        rb_autoexit->setDisabled(false);
        rb_none->setDisabled(false);
        if (cpevent == 1) {
            system("wininit");
        }
        else if (cpevent == 2) {
            system("shutdown -s -t 0");
        }
        else if (cpevent == 3) {
            qApp->quit();
        }
        else if (cpevent == 0) {
            QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ե�����"));
        }
    }
    else {
        QMessageBox::critical(this, "ERROR", "Cannot to write to cmd mode file.");
        btn_startsqueeze->setDisabled(false);
        btn_selectpath->setDisabled(false);
        le_path->setDisabled(false);
        rb_wt->setDisabled(false);
        rb_cmd->setDisabled(false);
        rb_autobluescreen->setDisabled(false);
        rb_autoshutdown->setDisabled(false);
        rb_autoexit->setDisabled(false);
        rb_none->setDisabled(false);
    }
}

LPCSTR StdStringToLPCSTR(const std::string& str) {
    // Allocate a buffer to hold the null-terminated string
    static char buffer[512]; // Assuming a maximum size here

    // Copy the content of std::string to the buffer
    strcpy_s(buffer, sizeof(buffer), str.c_str());

    // Return LPCSTR (const char*)
    return buffer;
}
const std::string& WstringToConstStringRef(const std::wstring& wstr) {
    // Using std::wstring_convert for UTF-16 to UTF-8 conversion
    static std::string convertedString;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    convertedString = converter.to_bytes(wstr);
    return convertedString;
}
