#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QSettings>
#include "settingwidget.h"
#include <QSystemTrayIcon>
#define INITINDEX 0

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    ///歌词列表显示
    void setWidgetLyricShow(QStringList lyriclist);
    void setCurrentLyricPos(int index);

    ///歌曲列表的显示
    void setWidgetSongNameList(QStringList namelist);

    ///初始音量的显示
    void setInitVolume(int volume);

    ///时间显示与控制
    void setCurrentSongTime(QString time);//显示当前歌曲时间
    void setCurrentSongPosition(qint64 position);//显示当前歌曲位置
    void setCurrentSongMaxPosition(qint64 totalPosition);//调整歌曲最大显示位置
    void setCurrentPlayStatus(QString status);
    void setInitPlayStatus(QString initstatus);
    void setCurrentSongSource(int index,QString name);
    void setCurrentPlayMode(int m_playMode);//设置播放模式图标（文字）
    //清空lw_songNameList
    void clearWidgetSongListAndLyricList(void);

    //初始化index配置文件
    bool writeInit(QString group, QString key, int value);
    bool readInit(QString group, QString key, int &value);
    ///最小化到托盘
    void minimizeToTray();
    void createActions();
    void createMenu();

    QSystemTrayIcon *mSysTrayIcon;//要在逻辑层 退出程序后删除托盘图标  做成public


signals:
   // void signalControlPlay(void);
    void signalMusicSliderPositionChanged(int position);
    void signalVolumeSliderPositionChanged(int position);
    //播放暂停控制
    void siganlPlayStatusChanged(void);
    //播放曲目控制
    void signalControlPlaySongs(int index);
    //播放模式控制
    void signalPlayModeChanged(void);
    //上一曲下一曲控制
    void signalPlayNextSong(void);
    void signalPlayPrevSong(void);
    //SettingWidget 页面的打开和关闭
    void signalOpenSettingWidget(void);
    void signalCloseSettingWidget(void);
    //默认播放曲目设置成功信号
    void signalInitIndexSuccess(int index);
    //DeskLyricWidget 页面的打开和关闭
    void signalOpenDeskLyricWidget(void);
    void signalCloseDeskLyricWidget(void);
    //DesktopPremoye 页面的打开和关闭
    void signalOpenDesktopPremoye(void);
    void signalCloseDesktopPremoye(void);
    //主页面的关闭
    void signalCloseMainWidget(void);
    //欢迎页面的关闭
    void signalCloseWelcomeWidget(void);



private slots:
    void on_pb_close_clicked();

    void on_pb_min_clicked();

    void on_pb_max_clicked();

    void on_hs_position_sliderMoved(int position);

    void on_hs_volume_sliderMoved(int position);


    void on_lw_songNameList_doubleClicked(const QModelIndex &index);

    void on_pb_playMode_clicked();

    void on_pb_prev_clicked();

    void on_pb_next_clicked();

    void on_pb_setting_clicked();
    //开始更改默认播放曲目自定义槽
    void slotInitPlaySongsIndexBegin(void);
    //歌词和歌曲路径发生改变自定义槽
    void slotPathsChanged(void);

    void on_pb_deskLyric_clicked();

    void on_pb_remote_clicked();
    //点击系统托盘图标，显示主界面
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_showMainAction();
    void on_exitAppAction();

protected:
    ///用于界面移动的相关鼠标事件
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    Ui::MainWidget *ui;
    ///实现跑马
    void showTicker();
    int m_curIndex;
    QString m_showText;

    ///定义一个QPoint对象用于界面移动
    QPoint m_widgetMove;
    ///初始化界面布局
    void initWidgetUnits(void);//去除界面边框

    int m_getIniIndex;
    int m_writeIndex;

    //配置文件地址
    QString m_setIndexFileName;


    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
};

#endif // MAINWIDGET_H
