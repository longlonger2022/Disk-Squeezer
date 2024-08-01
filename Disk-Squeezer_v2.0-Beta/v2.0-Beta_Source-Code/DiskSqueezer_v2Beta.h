#pragma once

#include <QtWidgets/QWidget>
#include "ui_DiskSqueezer_v2Beta.h"
#include <QPushButton>
#include <QProgressBar>
#include <QGroupBox>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QTimer>
#include <QLineEdit>
#include <QRadioButton>


class DiskSqueezer_v2Beta : public QWidget
{
    Q_OBJECT

public:
    DiskSqueezer_v2Beta(QWidget *parent = nullptr);
    ~DiskSqueezer_v2Beta();
    

private:
    Ui::DiskSqueezer_v2BetaClass ui;
    //����
    QString squeezepathtofunc;
    //��ť
    QPushButton* btn_selectpath;
    QPushButton* btn_startsqueeze;
    QPushButton* btn_about;
    QPushButton* btn_exit;
    //������
    QProgressBar* pb_squeezer;
    //�����
    QGroupBox* gb_path;
    QGroupBox* gb_mode;
    QGroupBox* gb_completeevent;
    //�����
    QLineEdit* le_path;
    //��ѡ��
    QRadioButton* rb_wt;
    QRadioButton* rb_cmd;
    QRadioButton* rb_autobluescreen;
    QRadioButton* rb_autoshutdown;
    QRadioButton* rb_autoexit;
    QRadioButton* rb_none;


private slots:
    void wt(QString squeezepath, int cpevent);
    void cmd(QString squeezepath, int cpevent);
    void updatepb(QString squeezepath);
    void recoverWidget();
    void coverWidget();
    
};

