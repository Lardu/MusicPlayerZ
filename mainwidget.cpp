#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QTimer>
#include <QMenu>
#include <QAction>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    m_setIndexFileName = "./../MusicPlayerZ/index.ini";
    initWidgetUnits();

    connect(ui->pb_play,SIGNAL(clicked()),
            this,SIGNAL(siganlPlayStatusChanged()));

    m_curIndex = 0;//当前角码
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWidget::showTicker);
    timer->start(1000);//定时器
    //显示系统托盘图标
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon("../MusicPlayerZ/icon/trayicon.ico");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("测试系统托盘图标"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    //建立托盘操作的菜单
    createActions();
    createMenu();
    //在系统托盘显示此对象
    mSysTrayIcon->show();

}

MainWidget::~MainWidget()
{
    delete ui;
}

///最小化到托盘
void MainWidget::minimizeToTray()
{
    //隐藏程序主窗口
    this->hide();
}
///点击系统托盘图标，显示主界面
void MainWidget::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示主程序窗口
        this->show();
        break;
    default:
        break;
    }
}

void MainWidget::createActions()
{
    mShowMainAction = new QAction(QObject::trUtf8("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(QObject::trUtf8("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

}

void MainWidget::createMenu()
{
    mMenu = new QMenu(this);
    //新增菜单项---显示主界面
    mMenu->addAction(mShowMainAction);
    //增加分隔符
    mMenu->addSeparator();
    //新增菜单项---退出程序
    mMenu->addAction(mExitAppAction);
    //把QMenu赋给QSystemTrayIcon对象
    mSysTrayIcon->setContextMenu(mMenu);
}
/*
* 当在系统托盘点击菜单内的显示主界面操作
*/
void MainWidget::on_showMainAction()
{
    this->show();
}

/*
* 当在系统托盘点击菜单内的退出程序操作
*/
void MainWidget::on_exitAppAction()
{
    this->close();
    mSysTrayIcon->deleteLater();//删除图标
}

///托盘结束
void MainWidget::clearWidgetSongListAndLyricList(void){
    ui->lw_songNameList->clear();
    ui->lw_lyricShow->clear();
}

void MainWidget::setCurrentPlayStatus(QString status){
    ui->pb_play->setText(status);
}
void MainWidget::setInitPlayStatus(QString initstatus){
    ui->pb_play->setText(initstatus);
}

void MainWidget::setCurrentSongSource(int index,QString name){
    ui->lw_songNameList->setCurrentRow(index);
    ui->lb_songName->setText(name);
    m_showText = name;
}

void MainWidget::showTicker()
{
    int iSize = m_showText.size();
    if (m_curIndex > iSize)
        m_curIndex = 0;
//    qDebug()<<m_curIndex;
//    qDebug()<<m_showText.mid(m_curIndex++);
    ui->lb_songName->setText(m_showText.mid(m_curIndex++));
}

void MainWidget::setCurrentPlayMode(int m_playMode){
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


void MainWidget::setWidgetSongNameList(QStringList namelist){
    ui->lw_songNameList->addItems(namelist);
    qDebug()<<"Set Song Name Success";
}

void MainWidget::setWidgetLyricShow(QStringList lyriclist){
    ui->lw_lyricShow->clear();
    ui->lw_lyricShow->addItems(lyriclist);

    for(int i = 0; i < lyriclist.length(); i++)
    {
        ui->lw_lyricShow->item(i)->setTextAlignment(Qt::AlignCenter);
    }
}

void MainWidget::setCurrentLyricPos(int index){
    ui->lw_lyricShow->setCurrentRow(index);

}


void MainWidget::setInitVolume(int volume){
    ui->hs_volume->setValue(volume);
}

void MainWidget::setCurrentSongTime(QString time){
    ui->lb_position->setText(time);
}

void MainWidget::setCurrentSongPosition(qint64 position){
    if(!ui->hs_position->isSliderDown()) //当slider不被按下时
    {
        ui->hs_position->setValue(position);
    }
}
void MainWidget::setCurrentSongMaxPosition(qint64 totalPosition){
    ui->hs_position->setMaximum(totalPosition);
}

void MainWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        m_widgetMove = ev->globalPos() -
        this->frameGeometry().topLeft();
        ev->accept(); //鼠标事件被系统接收
    }
}



///当鼠标左键被按下时，移动主窗口
void MainWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton)
    {
        this->move(ev->globalPos() - m_widgetMove);
        ev->accept();
    }
}

///初始化界面布局
void MainWidget::initWidgetUnits(void)
{
    //去除Widget界面边框
    this->setWindowFlags(Qt::FramelessWindowHint);
}
///最小化界面
void MainWidget::on_pb_min_clicked()
{
    this->showMinimized();
}
///最大化界面
void MainWidget::on_pb_max_clicked()
{
    if(this->isMaximized()){
        this->showNormal();
    }else{
        this->showMaximized();
    }
}
///关闭界面
void MainWidget::on_pb_close_clicked()
{
    emit signalCloseSettingWidget();
    emit signalCloseDeskLyricWidget();
    emit signalCloseDesktopPremoye();
    emit signalCloseMainWidget();
    emit signalCloseWelcomeWidget();
}

void MainWidget::on_hs_position_sliderMoved(int position)
{
    emit signalMusicSliderPositionChanged(position);
}

void MainWidget::on_hs_volume_sliderMoved(int position)
{
    emit signalVolumeSliderPositionChanged(position);
}

void MainWidget::on_lw_songNameList_doubleClicked(const QModelIndex &index)
{
    emit signalControlPlaySongs(index.row());
    //行信息写入文件
    //writeInit("Index","CurrentIndex",index.row());
    ui->pb_play->setText("pause");
}

void MainWidget::on_pb_playMode_clicked()
{
    emit signalPlayModeChanged();
}

void MainWidget::on_pb_prev_clicked()
{
    emit signalPlayPrevSong();
}

void MainWidget::on_pb_next_clicked()
{
    emit signalPlayNextSong();
}

void MainWidget::on_pb_setting_clicked()
{
    emit signalOpenSettingWidget();
}

void MainWidget::slotInitPlaySongsIndexBegin(void){
    readInit("Index","CurrentIndex",m_getIniIndex);
    //qDebug()<<"iniIndex:"<<m_getIniIndex;
    emit signalInitIndexSuccess(m_getIniIndex);
}

void MainWidget::slotPathsChanged(void){
    writeInit("Index","CurrentIndex",INITINDEX);
    emit signalInitIndexSuccess(INITINDEX);
    //qDebug()<<"hhhhhhhhh";
}

///操作读写文件 头 #include <QSettings>
bool MainWidget::writeInit(QString group, QString key, int value)
{
    if(group.isEmpty() || key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings config(m_setIndexFileName, QSettings::IniFormat);

        //将信息写入配置文件
        config.beginGroup(group);
        config.setValue(key, value);
        config.endGroup();

        return true;
    }
}

bool MainWidget::readInit(QString group, QString key, int &value)
{
    value = 0;
    if(m_setIndexFileName.isEmpty() || key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings config(m_setIndexFileName, QSettings::IniFormat);

        //读取用户配置信息
        value = config.value(group + "/" + key).toInt();
        qDebug()<<value;

        return true;
    }
}

void MainWidget::on_pb_deskLyric_clicked()
{
    emit signalOpenDeskLyricWidget();
}

void MainWidget::on_pb_remote_clicked()
{
    emit signalOpenDesktopPremoye();
}
