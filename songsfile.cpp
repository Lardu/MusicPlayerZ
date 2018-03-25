#include "songsfile.h"

QString SongsFile::m_songPath= "";

SongsFile::SongsFile(QObject *parent) : QObject(parent)
{
   // m_songPath.clear();
    m_songsNameList.clear();
    m_songsPathList.clear();   
}

SongsFile::~SongsFile()
{

}

void SongsFile::setSongsPath(const QString songPath)//做哪一类操作时，要注意加上const呢？
{
    m_songPath = songPath;
}



void SongsFile::gainSongsSource(){
   // m_songPath = "C:/UnigressMusic/songs";
    QDir dir(m_songPath);
    QFileInfoList infos = dir.entryInfoList(QStringList() << "*.mp3",QDir::Files, QDir::Name);
    foreach(const QFileInfo &info, infos)
    {
        m_songsNameList.append(info.baseName());
        m_songsPathList.append(info.absoluteFilePath());
    }
}

QStringList &SongsFile::getSongNameList()
{

    return m_songsNameList;
}

QStringList &SongsFile::getSongPathList()
{
    return m_songsPathList;
}

