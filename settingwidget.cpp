#include "settingwidget.h"
#include "ui_settingwidget.h"
#include "songsfile.h"
#include "lyricsfile.h"
#include <QDebug>

SettingWidget::SettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    initEtcInfo();
    initCheckBoxInfo();

}

SettingWidget::~SettingWidget()
{
    delete ui;
}


void SettingWidget::initCheckBoxInfo(void){

    readInit("Setting", "Close", m_closeState);
    readInit("Setting", "Welcome", m_welcomeState);
    if(m_closeState == "true"){
        ui->cb_close->setChecked(true);
    }else{
        ui->cb_close->setChecked(false);
    }

    if(m_welcomeState == "true"){
        ui->cb_welcome->setChecked(true);
    }else{
        ui->cb_welcome->setChecked(false);
    }
}

bool SettingWidget::getCloseCheckBoxStatus(){
    if(m_closeState == "true"){
        return true;
    }else{
        return false;
    }
}

bool SettingWidget::getWelcomeCheckBoxStatus(){
    if(m_welcomeState == "true"){
        return true;
    }else{
        return false;
    }
}

void SettingWidget::initEtcInfo(void)
{
    QString songsPath;
    QString lyricPath;

    m_settingFileName = "./../MusicPlayerZ/user.ini";

    readInit("Path", "songPath", songsPath);
    readInit("Path", "lyricPath", lyricPath);

    ui->le_songPath->setText(songsPath);
    ui->le_lyricPath->setText(lyricPath);

    SongsFile::setSongsPath(songsPath);
    LyricsFile::setLyricPath(lyricPath);
}


bool SettingWidget::writeInit(QString group, QString key, QString value)
{
    if(group.isEmpty() || key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings config(m_settingFileName, QSettings::IniFormat);

        //将信息写入配置文件
        config.beginGroup(group);
        config.setValue(key, value);
        config.endGroup();

        return true;
    }
}

bool SettingWidget::readInit(QString group, QString key, QString &value)
{
    value.clear();
    if(m_settingFileName.isEmpty() || key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings config(m_settingFileName, QSettings::IniFormat);

        //读取用户配置信息
        value = config.value(group + "/" + key).toString();
        qDebug()<<value;

        return true;
    }
}



void SettingWidget::on_pb_sure_clicked()
{
    writeInit("Path", "songPath", ui->le_songPath->text());
    writeInit("Path", "lyricPath", ui->le_lyricPath->text());
    emit signalSongsAndLyricChanged(ui->le_songPath->text(),
                                    ui->le_lyricPath->text());
    this->hide();
}

void SettingWidget::on_pb_songPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select the song path"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->le_songPath->setText(dir);
}

void SettingWidget::on_pb_lyricPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select the lyric path"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->le_lyricPath->setText(dir);
}

void SettingWidget::on_pb_set_clicked()
{

    writeInit("Setting", "Close", ui->cb_close->isChecked());
    writeInit("Setting", "Welcome", ui->cb_welcome->isChecked());
    readInit("Setting", "Close", m_closeState);
    readInit("Setting", "Welcome", m_welcomeState);
    this->hide();

}

bool SettingWidget::writeInit(QString group, QString key, bool value)
{
    if(group.isEmpty() || key.isEmpty())
    {
        return false;
    }
    else
    {
        //创建配置文件操作对象
        QSettings config(m_settingFileName, QSettings::IniFormat);

        //将信息写入配置文件
        config.beginGroup(group);
        config.setValue(key, value);
        config.endGroup();

        return true;
    }
}
