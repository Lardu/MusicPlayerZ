#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = 0);
    ~SettingWidget();

    void initEtcInfo(void);
    void initCheckBoxInfo(void);

    bool writeInit(QString group, QString key, QString value);
    bool writeInit(QString group, QString key, bool value);
    bool readInit(QString group, QString key, QString &value);

    //返回checkbox两个按钮的状态
    bool getCloseCheckBoxStatus();
    bool getWelcomeCheckBoxStatus();

signals:
    void signalSongsAndLyricChanged(QString songPath,QString lyricPath);



private:
    Ui::SettingWidget *ui;

    QString m_settingFileName;

    //保存checkbox两个按钮的状态
    QString m_closeState;
    QString m_welcomeState;


private slots:

    void on_pb_sure_clicked();
    void on_pb_songPath_clicked();
    void on_pb_lyricPath_clicked();
    void on_pb_set_clicked();



};

#endif // SETTINGWIDGET_H
