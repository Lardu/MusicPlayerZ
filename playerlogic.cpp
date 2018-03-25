#include "playerlogic.h"
#include <QTime>

PlayerLogic::PlayerLogic(QObject *parent) : QObject(parent)
{
    initMainWidget();
    initWelcomeWidget();
    initDeskLyricWidget();
    initSettingWidget();
    initDesktopRemote();



    m_songsFile = new SongsFile;
    m_lyricsFile = new LyricsFile;
    m_playList = new QMediaPlaylist;
    m_player = new QMediaPlayer;
    m_songsFile->gainSongsSource();
    initPlayList();
    initPlayer();
}

PlayerLogic::~PlayerLogic()
{

}

//歌词歌曲路径改变时，音乐播放器重置
void PlayerLogic::slotSongsAndLyricChanged(QString songPath, QString lyricPath){

    //清除原有的资源内容及显示
    m_playList->clear();

    m_mainWidget->clearWidgetSongListAndLyricList();
    m_songsFile->getSongNameList().clear();
    m_songsFile->getSongPathList().clear();

    //填充新的资源内容
    m_lyricsFile->setLyricPath(lyricPath);
    m_songsFile->setSongsPath(songPath);
    m_songsFile->gainSongsSource();

    //重新初始化
    initPlayList();
    initPlayer();
}

void PlayerLogic::initPlayList(void){


    foreach (QString songPath, m_songsFile->getSongPathList()) {
        qDebug()<<songPath;
        m_playList->addMedia(QUrl(songPath));
    }
    m_mainWidget->setWidgetSongNameList(m_songsFile->getSongNameList());

    connect(m_playList,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slotCurrentIndexChanged(int)));

    ///播放模式初始化
    m_playMode = 3;
    m_playList->setPlaybackMode(QMediaPlaylist::Loop);
    m_mainWidget->setCurrentPlayMode(m_playMode);
    m_deskLyricWidget->setCurrentPlayMode(m_playMode);
    m_desktopRemote->setCurrentPlayMode(m_playMode);
}

void PlayerLogic::gainCurrentSongLyric(){
    m_lyricsFile->gainSongsLyric(m_songName);
    m_mainWidget->setWidgetLyricShow(m_lyricsFile->getLyric());
}

void PlayerLogic::slotPlayModeChanged(){
    m_playMode++;
    //保留Loop,CurrentItemInLoop,Random三种播放模式
    //其余两中有数组越界现象，不知道怎么解决
    if(m_playMode == 5){
        m_playMode=0;
        m_playMode++;
    }
    if(m_playMode == 2){
        m_playMode++;
    }
    //qDebug()<<m_playMode;
    switch(m_playMode){
    case QMediaPlaylist::CurrentItemOnce:
        m_playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        m_playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case QMediaPlaylist::Sequential:
        m_playList->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case QMediaPlaylist::Loop:
        m_playList->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case QMediaPlaylist::Random:
        m_playList->setPlaybackMode(QMediaPlaylist::Random);
        break;
    }
    //播放模式切换
    m_mainWidget->setCurrentPlayMode(m_playMode);
    m_deskLyricWidget->setCurrentPlayMode(m_playMode);
    m_desktopRemote->setCurrentPlayMode(m_playMode);
}

void PlayerLogic::slotPlayPrevSong(void){
    //这里这么处理是因为单曲循环时，上一曲下一曲没有反应，所以我们这里单独处理一下
    if(m_playMode == 1){
        m_currentIndex = m_playList->currentIndex() - 1;
        if(m_currentIndex == -1)
        {
            m_currentIndex = m_songsFile->getSongNameList().length() - 1;
        }
        m_playList->setCurrentIndex(m_currentIndex);
    }else{
        m_currentIndex = m_playList->previousIndex();
        m_playList->setCurrentIndex(m_currentIndex);
    }
}

void PlayerLogic::slotPlayNextSong(void){
    if(m_playMode == 1){
        m_currentIndex = m_playList->currentIndex() + 1;
        if(m_currentIndex > m_songsFile->getSongNameList().length()-1)
        {
            m_currentIndex = 0;
        }
        m_playList->setCurrentIndex(m_currentIndex);
    }else{
        m_currentIndex = m_playList->nextIndex();
        m_playList->setCurrentIndex(m_currentIndex);
    }
}

void PlayerLogic::slotCurrentIndexChanged(int position){
    m_songName = m_songsFile->getSongNameList().at(position);
    m_mainWidget->setCurrentSongSource(position,m_songName);
    m_deskLyricWidget->setSongName(m_songName);
    m_desktopRemote->setSongName(m_songName);
    m_deskLyricWidget->clearDeskLyricList();
    m_mainWidget->writeInit("Index","CurrentIndex",position);

    //初始化播放状态 歌曲切换后，播放状态全部切换为： 播放
    m_player->play();
    m_mainWidget->setInitPlayStatus("Pause");
    m_deskLyricWidget->setInitPlayStatus("Pause");
    m_desktopRemote->setInitPlayStatus("Pause");

    gainCurrentSongLyric();
}

void PlayerLogic::slotInitIndexSuccess(int index){
    m_initIndex = index;
}

void PlayerLogic::initPlayer(void)
{

    m_player->setPlaylist(m_playList);
    connect(m_player,SIGNAL(durationChanged(qint64)),
            this,SLOT(slotDurationChanged(qint64)));
    connect(m_player,SIGNAL(positionChanged(qint64)),
            this,SLOT(slotPositionChanged(qint64)));
    m_player->setVolume(InitVolume);
    m_playList->setCurrentIndex(m_initIndex);
    m_player->play();

    m_mainWidget->setInitVolume(InitVolume);
}

//歌曲播放位置自定义槽
void PlayerLogic::slotPositionChanged(qint64 position){
    m_currentMusicTime = position / 1000;

    QTime currentTime((m_currentMusicTime/3600)%60,
                      (m_currentMusicTime/60)%60,
                      (m_currentMusicTime%60),
                      (m_currentMusicTime*1000)%1000);
    QTime totalTime((m_totalMusicTime/3600)%60,
                    (m_totalMusicTime/60)%60,
                    (m_totalMusicTime%60),
                    (m_totalMusicTime*1000)%1000);
    m_musicPosition = currentTime.toString("mm:ss")
                            + "/" +
                       totalTime.toString("mm:ss");
    if(m_lyricsFile->getLyricPos().contains(m_currentMusicTime)){
        int currentLyricIndex = m_lyricsFile->getLyricPos().value(m_currentMusicTime);
        m_mainWidget->setCurrentLyricPos(currentLyricIndex);
        //qDebug()<<currentLyricIndex;
        //int tmp = currentLyricIndex;
        if(currentLyricIndex + 1 < m_lyricsFile->getLyric().length()){
            m_deskLyricWidget->showDeskLyric(currentLyricIndex+1,m_lyricsFile->getLyric().at(currentLyricIndex+1));

        }else{
            m_deskLyricWidget->showLastDeskLyric();
        }

       //m_deskLyricWidget->show();

    }
    m_mainWidget->setCurrentSongTime(m_musicPosition);
    m_mainWidget->setCurrentSongPosition(m_currentMusicTime);
}

//当前歌曲总时长自定义槽
void PlayerLogic::slotDurationChanged(qint64 duration){
    m_totalMusicTime = duration / 1000;
    m_mainWidget->setCurrentSongMaxPosition(m_totalMusicTime);
}

//歌曲播放位置控制自定义槽
void PlayerLogic::slotSliderMusicPositionChanged(int position){
    m_player->setPosition(position * 1000);
}
void PlayerLogic::slotSliderVolumePositionChanged(int position){
    m_player->setVolume(position);
}


///播放状态控制的自定义槽
void PlayerLogic::slotPlayStatusChanged(void){
    if(m_player->state() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
        m_mainWidget->setCurrentPlayStatus("play");
        m_deskLyricWidget->setCurrentPlayStatus("play");
        m_desktopRemote->setCurrentPlayStatus("play");

    }else
    {
        m_player->play();
        m_mainWidget->setCurrentPlayStatus("pause");
        m_deskLyricWidget->setCurrentPlayStatus("pause");
        m_desktopRemote->setCurrentPlayStatus("pause");
    }
}

void PlayerLogic::slotControlPlaySongs(int index){
    //qDebug()<<index<<endl;
    m_playList->setCurrentIndex(index);
    m_player->play();
}

void PlayerLogic::initMainWidget(void){
    m_mainWidget = new MainWidget;
    //初始化播放模式
    m_mainWidget->setInitPlayStatus("Pause");
    connect(m_mainWidget,SIGNAL(signalMusicSliderPositionChanged(int)),
            this,SLOT(slotSliderMusicPositionChanged(int)));
    connect(m_mainWidget,SIGNAL(signalVolumeSliderPositionChanged(int)),
            this,SLOT(slotSliderVolumePositionChanged(int)));
    connect(m_mainWidget,SIGNAL(siganlPlayStatusChanged()),
            this,SLOT(slotPlayStatusChanged()));
    connect(m_mainWidget,SIGNAL(signalControlPlaySongs(int)),
            this,SLOT(slotControlPlaySongs(int)));
    connect(m_mainWidget,SIGNAL(signalPlayModeChanged()),
            this,SLOT(slotPlayModeChanged()));
    connect(m_mainWidget,SIGNAL(signalPlayPrevSong()),
            this,SLOT(slotPlayPrevSong()));
    connect(m_mainWidget,SIGNAL(signalPlayNextSong()),
            this,SLOT(slotPlayNextSong()));
    connect(this,SIGNAL(signalInitPlaySongsIndexBegin()),
            m_mainWidget,SLOT(slotInitPlaySongsIndexBegin()));
    connect(m_mainWidget,SIGNAL(signalInitIndexSuccess(int)),
            this,SLOT(slotInitIndexSuccess(int)));
    connect(this,SIGNAL(signalPathsChanged()),
            m_mainWidget,SLOT(slotPathsChanged()));
    connect(m_mainWidget,SIGNAL(signalCloseMainWidget()),
            this,SLOT(slotCloseMainWidget()));
    connect(m_mainWidget,SIGNAL(signalCloseWelcomeWidget()),
            this,SLOT(slotCloseWelcomeWidget()));
    emit signalInitPlaySongsIndexBegin();

}

void PlayerLogic::showMainWidget(void){
    if(m_settingWidget->getWelcomeCheckBoxStatus()){
        m_welcomeWidget->show();
    }
    m_mainWidget->show();

}

///点击关闭按钮时  或者关闭  或者最小化到托盘
void PlayerLogic::slotCloseMainWidget(void){
    if(m_settingWidget->getCloseCheckBoxStatus()){
        m_mainWidget->minimizeToTray();
    }else if(!m_settingWidget->getCloseCheckBoxStatus()){
        m_mainWidget->mSysTrayIcon->deleteLater();//删除图标
        m_mainWidget->close();
    }
}

///settingWidget页面
void PlayerLogic::initSettingWidget(void){
    m_settingWidget = new SettingWidget;
    connect(m_mainWidget,SIGNAL(signalOpenSettingWidget()),
            this,SLOT(slotOpenSettingWidget()));
    connect(m_mainWidget,SIGNAL(signalCloseSettingWidget()),
            this,SLOT(slotCloseSettingWidget()));
    connect(m_settingWidget,SIGNAL(signalSongsAndLyricChanged(QString,QString)),
            this,SIGNAL(signalPathsChanged()));
    connect(m_settingWidget,SIGNAL(signalSongsAndLyricChanged(QString,QString)),
            this,SLOT(slotSongsAndLyricChanged(QString,QString)));

}

void PlayerLogic::slotOpenSettingWidget(void){
    //设置为非模态对话框，未保存路径之前，不可操作当前界面之外的其他界面
    m_settingWidget->setWindowModality(Qt::ApplicationModal);
    //m_settingWidget->setWindowFlags(m_settingWidget->windowFlags() | Qt::WindowStaysOnTopHint);
    m_settingWidget->show();
}

///当设置界面设置为模态对话框后  此处代码可删去
void PlayerLogic::slotCloseSettingWidget(void){
    if(!m_settingWidget->getCloseCheckBoxStatus()){
        m_settingWidget->close();
    }
}

///初始化桌面歌词界面
void PlayerLogic::initDeskLyricWidget(){
    m_deskLyricWidget = new DeskLyricWidget;
    m_deskLyricWidget->setInitPlayStatus("Pause");
    connect(m_mainWidget,SIGNAL(signalOpenDeskLyricWidget()),
            this,SLOT(slotOpenDeskLyricWidget()));
    connect(m_mainWidget,SIGNAL(signalCloseDeskLyricWidget()),
            this,SLOT(slotCloseDeskLyricWidget()));
    connect(m_deskLyricWidget,SIGNAL(siganlPlayStatusChanged()),
            this,SLOT(slotPlayStatusChanged()));
    connect(m_deskLyricWidget,SIGNAL(signalPlayModeChanged()),
            this,SLOT(slotPlayModeChanged()));
    connect(m_deskLyricWidget,SIGNAL(signalPlayPrevSong()),
            this,SLOT(slotPlayPrevSong()));
    connect(m_deskLyricWidget,SIGNAL(signalPlayNextSong()),
            this,SLOT(slotPlayNextSong()));

}

void PlayerLogic::slotOpenDeskLyricWidget(void){
    //如果已经显示，就hide
    //如果还未显示，就show
    if(m_deskLyricWidget->isVisible()){
        m_deskLyricWidget->hide();
    }else{
        m_deskLyricWidget->show();
    }
}

void PlayerLogic::slotCloseDeskLyricWidget(){
    if(!m_settingWidget->getCloseCheckBoxStatus()){
        m_deskLyricWidget->close();
    }
}

///初始化桌面遥控器界面
void PlayerLogic::initDesktopRemote(){
    m_desktopRemote = new DesktopRemote;
    m_desktopRemote->setInitPlayStatus("Pause");
    connect(m_mainWidget,SIGNAL(signalOpenDesktopPremoye()),
            this,SLOT(slotOpenDesktopPremoye()));
    connect(m_mainWidget,SIGNAL(signalCloseDesktopPremoye()),
            this,SLOT(slotCloseDesktopPremoye()));
    connect(m_desktopRemote,SIGNAL(siganlPlayStatusChanged()),
            this,SLOT(slotPlayStatusChanged()));
    connect(m_desktopRemote,SIGNAL(signalPlayModeChanged()),
            this,SLOT(slotPlayModeChanged()));
    connect(m_desktopRemote,SIGNAL(signalPlayPrevSong()),
            this,SLOT(slotPlayPrevSong()));
    connect(m_desktopRemote,SIGNAL(signalPlayNextSong()),
            this,SLOT(slotPlayNextSong()));
}

void PlayerLogic::slotOpenDesktopPremoye(void){
    //如果已经显示，就hide
    //如果还未显示，就show
    if(m_desktopRemote->isVisible()){
        m_desktopRemote->hide();
    }else{
        m_desktopRemote->show();
    }
}

void PlayerLogic::slotCloseDesktopPremoye(void){
    if(!m_settingWidget->getCloseCheckBoxStatus()){
        m_desktopRemote->close();
    }
}


void PlayerLogic::initWelcomeWidget(){
    m_welcomeWidget = new WelcomeWidget;
}


void PlayerLogic::slotCloseWelcomeWidget(void){
    if(m_settingWidget->getCloseCheckBoxStatus()){
        m_welcomeWidget->hide();
    }else if(!m_settingWidget->getCloseCheckBoxStatus()){
        m_welcomeWidget->close();
    }
}

