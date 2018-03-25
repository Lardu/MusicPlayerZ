#include "desktopremote.h"
#include "ui_desktopremote.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QTimer>

DesktopRemote::DesktopRemote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesktopRemote)
{
    ui->setupUi(this);

    ///歌曲名左跑马
    m_curIndex = 0;//当前角码
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DesktopRemote::showTicker);
    timer->start(1000);//定时器

    ///使遥控器界面显示在桌面右下角
    //获取可用桌面大小
    QRect deskRect = QApplication::desktop()->availableGeometry();
    //得到应用程序矩形
    QRect applicationRect = this->frameGeometry();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(deskRect.width()-applicationRect.width(),deskRect.height()-applicationRect.height());
}

DesktopRemote::~DesktopRemote()
{
    delete ui;
}

//设置播放状态
void DesktopRemote::setCurrentPlayStatus(QString status){
    ui->pb_play->setText(status);
}

//设置初始化播放按钮
void DesktopRemote::setInitPlayStatus(QString initstatus){
    ui->pb_play->setText(initstatus);
}

//设置播放模式
void DesktopRemote::setCurrentPlayMode(int m_playMode){
    if(m_playMode == 0){
        ui->pb_playMode->setText("COnce");
    }else if(m_playMode == 1){
        ui->pb_playMode->setText("CLoop");
    }else if(m_playMode == 2){
        ui->pb_playMode->setText("Sequential");
    }else if(m_playMode == 3){
        ui->pb_playMode->setText("Loop");
    }else if(m_playMode == 4){
        ui->pb_playMode->setText("Random");
    }
}

//布置歌曲名
void DesktopRemote::setSongName(QString name){
    //ui->lb_songName->setText(name);
    m_showText = name;
}

void DesktopRemote::showTicker()
{

    int iSize = m_showText.size();
    if (m_curIndex > iSize)
        m_curIndex = 0;
//    qDebug()<<m_curIndex;
//    qDebug()<<m_showText.mid(m_curIndex++);
    ui->lb_songName->setText(m_showText.mid(m_curIndex++));

    //ui->lb_songName->setText(m_showText.right(iSize - m_curIndex--));
}

void DesktopRemote::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        m_deskWidgetMove = ev->globalPos()
                - this->frameGeometry().topLeft();
        ev->accept();
    }
}

void DesktopRemote::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        move(ev->globalPos() - m_deskWidgetMove);
        ev->accept();
    }
}

void DesktopRemote::on_pb_prev_clicked()
{
    emit signalPlayPrevSong();
}

void DesktopRemote::on_pb_play_clicked()
{
    emit siganlPlayStatusChanged();
}

void DesktopRemote::on_pb_next_clicked()
{
    emit signalPlayNextSong();
}

void DesktopRemote::on_pb_playMode_clicked()
{
    emit signalPlayModeChanged();
}

void DesktopRemote::on_pb_close_clicked()
{
    this->close();
}
