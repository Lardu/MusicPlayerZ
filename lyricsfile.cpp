#include "lyricsfile.h"

QString LyricsFile::m_songsLyricPath = "";

LyricsFile::LyricsFile(QObject *parent) : QObject(parent)
{

}

LyricsFile::~LyricsFile()
{

}

void LyricsFile::setLyricPath(const QString lyricPath){
    m_songsLyricPath = lyricPath;
}

void LyricsFile::gainSongsLyric(QString songName){
    //m_songsLyricPath = "C:/UnigressMusic/lyrics";
    QString fileName = m_songsLyricPath + "/" + songName + ".lrc";
    qDebug()<<fileName<<endl;
    QFile lyricFile(fileName);
    if(!lyricFile.open(QIODevice::ReadOnly)){
        qDebug()<<"Error:File opened"<<endl;
    }else{
        quint64 index = 0;
        QString line,lyric,pos;
        QStringList lyricAndPos;
        QTextStream in(&lyricFile);
        m_songsLyricList.clear();//歌词列表清理
        m_mapIndexAndLyric.clear();//map容器处理
        while(!in.atEnd()){
            line = in.readLine();
            lyricAndPos = line.split("]");
            //位置跟踪处理
            pos = lyricAndPos.at(0).mid(1,5);
            QStringList time = pos.split(":");
            quint64 p = time.at(0).toInt() * 60 + time.at(1).toInt();
            m_mapIndexAndLyric.insert(p,index++);
            //歌词内容处理
            lyric = lyricAndPos.at(1);
            m_songsLyricList.append(lyric);
        }
        lyricAndPos.clear();
    }
    lyricFile.close();

}

QMap<quint64, quint64> &LyricsFile::getLyricPos(){
    return m_mapIndexAndLyric;
}

QStringList &LyricsFile::getLyric(void){
    return m_songsLyricList;
}


