#include "desklyricwidget.h"
#include "ui_desklyricwidget.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QDebug>

DeskLyricWidget::DeskLyricWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeskLyricWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->lw_deskLyric->item(0)->setTextAlignment(Qt::AlignLeft);
    ui->lw_deskLyric->item(1)->setTextAlignment(Qt::AlignRight);
    ui->lw_deskLyric->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->lw_deskLyric->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //歌曲名左跑马
    m_curIndex = 0;//当前角码
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DeskLyricWidget::showTicker);
    timer->start(1000);//定时器

    ///桌面歌词界面显示在屏幕顶端
    //获取可用桌面大小
    QRect deskRect = QApplication::desktop()->availableGeometry();
    //得到应用程序矩形
    QRect applicationRect = this->frameGeometry();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move((deskRect.width() - applicationRect.width())  / 2, 0);

}

DeskLyricWidget::~DeskLyricWidget()
{
    delete ui;
}

void DeskLyricWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        m_deskWidgetMove = ev->globalPos()
                - this->frameGeometry().topLeft();
        ev->accept();
    }
}

void DeskLyricWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        move(ev->globalPos() - m_deskWidgetMove);
        ev->accept();
    }
}

void DeskLyricWidget::showDeskLyric(int index, QString lyric){
    if(index % 2 == 0){
        ui->lw_deskLyric->setCurrentRow(1);
        ui->lw_deskLyric->item(0)->setText(lyric);
    }else {
        ui->lw_deskLyric->setCurrentRow(0);
        ui->lw_deskLyric->item(1)->setText(lyric);
    }
}

void DeskLyricWidget::showLastDeskLyric(){
    ui->lw_deskLyric->setCurrentRow(1);
}

void DeskLyricWidget::setSongName(QString name){
    //ui->lb_songName->setText(name);
    m_showText = name;
}

void DeskLyricWidget::showTicker()
{

    int iSize = m_showText.size();
    if (m_curIndex > iSize)
        m_curIndex = 0;
//    qDebug()<<m_curIndex;
//    qDebug()<<m_showText.mid(m_curIndex++);
    ui->lb_songName->setText(m_showText.mid(m_curIndex++));

    //ui->lb_songName->setText(m_showText.right(iSize - m_curIndex--));
}

void DeskLyricWidget::clearDeskLyricList(void){
    ui->lw_deskLyric->item(0)->setText("");
    ui->lw_deskLyric->item(1)->setText("");
}

void DeskLyricWidget::setCurrentPlayStatus(QString status){
    ui->pb_play->setText(status);
}

void DeskLyricWidget::setInitPlayStatus(QString initstatus){
    ui->pb_play->setText(initstatus);
}

void DeskLyricWidget::setCurrentPlayMode(int m_playMode){
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



void DeskLyricWidget::on_pb_close_clicked()
{
    this->close();
}

void DeskLyricWidget::on_pb_prev_clicked()
{
    emit signalPlayPrevSong();
}

void DeskLyricWidget::on_pb_next_clicked()
{
    emit signalPlayNextSong();
}

void DeskLyricWidget::on_pb_playMode_clicked()
{
    emit signalPlayModeChanged();
}

void DeskLyricWidget::on_pb_play_clicked()
{
    emit siganlPlayStatusChanged();
}
