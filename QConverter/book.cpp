#include "book.h"
#include <QDir>
#include <QDebug>


void Book::setSource(QString xDir)
{
    source = xDir;
}

void Book::convert()
{
    //init chapter list
    pageNumber = 0;
    int i = 0;
    chapterList.clear();
    chapterList << source.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::DirsFirst);
    //
    for (QFileInfoList::const_iterator ite = chapterList.constBegin(); ite != chapterList.constEnd(); ++ite)
    {
        i++;
        this->convertChapter(QDir(ite->absoluteFilePath()) , pageNumber);
        emit percentCompleted((i*100)/(chapterList.size()));
    }
}

void Book::convertChapter(QDir xChapter, quint64 &xPage)
{
    //init page list
    pageList.clear();
    pageList << xChapter.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name);
    //
    for (QFileInfoList::const_iterator ite = pageList.constBegin(); ite != pageList.constEnd(); ++ite)
    {
        qDebug()<<ite->absoluteFilePath();
        ++pageNumber;
    }
}
