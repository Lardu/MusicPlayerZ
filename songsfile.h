#ifndef SONGSFILE_H
#define SONGSFILE_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

class SongsFile : public QObject
{
    Q_OBJECT
public:

    explicit SongsFile(QObject *parent = 0);
    ~SongsFile();


    void gainSongsSource(void);
    QStringList &getSongNameList();
    QStringList &getSongPathList();

    static void setSongsPath(const QString songPath);
    
private:
    static QString m_songPath;
    QStringList m_songsNameList;
    QStringList m_songsPathList;
};

#endif // SONGSFILE_H
