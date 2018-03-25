#ifndef LYRICSFILE_H
#define LYRICSFILE_H

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include "songsfile.h"
#include <QMap>

class LyricsFile : public QObject
{
    Q_OBJECT
public:
    explicit LyricsFile(QObject *parent = 0);
    ~LyricsFile();
    QStringList &getLyric();
    QMap<quint64, quint64> &getLyricPos();
    void gainSongsLyric(QString songName);
    static void setLyricPath(const QString lyricPath);

private:
    static QString m_songsLyricPath;
    QStringList m_songsLyricList;
    QMap<quint64, quint64> m_mapIndexAndLyric;
};

#endif // LYRICSFILE_H
