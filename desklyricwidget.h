#ifndef DESKLYRICWIDGET_H
#define DESKLYRICWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class DeskLyricWidget;
}

class DeskLyricWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeskLyricWidget(QWidget *parent = 0);
    ~DeskLyricWidget();
    void showDeskLyric(int index,QString lyric);
    void showLastDeskLyric();
    void setSongName(QString name);
    void clearDeskLyricList(void);
    //设置播放模式
    void setCurrentPlayStatus(QString status);
    void setInitPlayStatus(QString initstatus);
    void setCurrentPlayMode(int m_playMode);

protected:
    ///用于界面移动的相关鼠标事件
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

signals:
    //播放暂停控制
    void siganlPlayStatusChanged(void);
    //播放模式控制
    void signalPlayModeChanged(void);
    //上一曲下一曲控制
    void signalPlayNextSong(void);
    void signalPlayPrevSong(void);


private slots:
    void on_pb_close_clicked();

    void on_pb_prev_clicked();

    void on_pb_next_clicked();

    void on_pb_playMode_clicked();

    void on_pb_play_clicked();

private:
    Ui::DeskLyricWidget *ui;

    ///实现跑马
    void showTicker();
    int m_curIndex;
    QString m_showText;

    ///定义一个QPoint对象用于界面移动
    QPoint m_deskWidgetMove;

};

#endif // DESKLYRICWIDGET_H
