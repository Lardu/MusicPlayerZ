#ifndef PLAYERLOGIC_H
#define PLAYERLOGIC_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <mainwidget.h>
#include "songsfile.h"
#include "lyricsfile.h"
#include "settingwidget.h"
#include "desklyricwidget.h"
#include "desktopremote.h"
#include "welcomewidget.h"

#define InitVolume 50

class PlayerLogic : public QObject
{
    Q_OBJECT
public:
    explicit PlayerLogic(QObject *parent = 0);
    ~PlayerLogic();

    void showMainWidget(void);


signals:
    void signalInitPlaySongsIndexBegin();
    void signalPathsChanged();

public slots:
    //void slotsControlPlay(void);



private slots:
    //歌曲播放进度自定义槽
    void slotPositionChanged(qint64 position);
    //当前歌曲总时长自定义槽
    void slotDurationChanged(qint64 duration);
    //鼠标拖动改变歌曲播放位置自定义槽
    void slotSliderMusicPositionChanged(int position);
    //鼠标拖动改变音量私有槽
    void slotSliderVolumePositionChanged(int position);
    //播放状态改变自定义槽
    void slotPlayStatusChanged(void);
    //播放曲目改变自定义槽
    void slotControlPlaySongs(int index);
    //播放位置切换自定义槽
    void slotCurrentIndexChanged(int position);
    //播放模式切换自定义槽
    void slotPlayModeChanged(void);
    //上一曲，下一曲切换自定义槽
    void slotPlayNextSong(void);
    void slotPlayPrevSong(void);
    //SettingWidget页面打开和关闭 自定义槽
    void slotOpenSettingWidget(void);
    void slotCloseSettingWidget(void);

    //歌曲路径改变自定义槽
    void slotSongsAndLyricChanged(QString songPath,QString lyricPath);

    //默认播放曲目设置成功自定义槽
    void slotInitIndexSuccess(int index);

    //打开关闭桌面歌词界面自定义槽
    void slotOpenDeskLyricWidget(void);
    void slotCloseDeskLyricWidget();

    //打开关闭DesktopPremoye界面自定义槽
    void slotOpenDesktopPremoye(void);
    void slotCloseDesktopPremoye(void);

    //主界面的关闭
    void slotCloseMainWidget(void);

    //欢迎页面的关闭
    void slotCloseWelcomeWidget(void);


private:
    ///初始化欢迎界面
    WelcomeWidget *m_welcomeWidget;
    void initWelcomeWidget();
    ///初始化桌面遥控器界面
    DesktopRemote *m_desktopRemote;
    void initDesktopRemote();
    ///初始化桌面歌词界面
    DeskLyricWidget *m_deskLyricWidget;
    void initDeskLyricWidget();
    ///初始化设置界面
    SettingWidget *m_settingWidget;
    void initSettingWidget();
    ///播放控制
    int m_playMode;//播放模式
    int m_currentIndex;//当前播放歌曲的下标

    ///初始化歌词获取操作
    LyricsFile *m_lyricsFile;
    void gainCurrentSongLyric();

    ///初始化歌曲获取操作
    SongsFile *m_songsFile;


    ///初始化用户界面
    MainWidget *m_mainWidget;
    void initMainWidget(void);

    ///初始化播放器列表
    QStringList m_songNameList;
    QMediaPlaylist *m_playList;
    void initPlayList(void);

    ///初始化播放器
    QString m_songName;
    QMediaPlayer *m_player;
   // int m_currentPlayIndex;
    //播放时间与歌曲切换
    qint64 m_currentMusicTime;
    qint64 m_totalMusicTime;
    QString m_musicPosition;

    void initPlayer(void);
    int m_initIndex;
};

#endif // PLAYERLOGIC_H
