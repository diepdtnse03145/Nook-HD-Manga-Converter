#include "book.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFile>

#define TEMPDIR "/temp3373"


void Book::setSource(QString xDir)
{
    source.setPath(xDir);
}

void Book::convert()
{
    //init chapter list
    pageNumber = 0;
    int i = 0;
    chapterList.clear();
    chapterList << source.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::DirsFirst);
    temp.setPath(TEMPDIR);

    //create temp folder
    if (temp.exists())
    {
        if(QMessageBox::question(NULL, TEMPDIR, "Temp directory exists, overwrite data?") == QMessageBox::Yes)
        {
            if(temp.removeRecursively())
                temp.mkpath(TEMPDIR);
            else {
                qDebug()<<TEMPDIR<<" can't delete!";
                return;
            }
        }
        else return;
    }
    else
        temp.mkpath(TEMPDIR);

    //Moving images
    for (QFileInfoList::const_iterator ite = chapterList.constBegin(); ite != chapterList.constEnd(); ++ite)
    {
        i++;
        this->convertChapter(QDir(ite->absoluteFilePath()));
        emit percentCompleted((i*100)/(chapterList.size()));
    }

    //Compressing...

    //Rename

    //delete temp
    /*if(!temp.removeRecursively())
       qDebug()<<TEMPDIR<<" can't delete!";*/


}

void Book::convertChapter(QDir xChapter)
{
    //init page list
    pageList.clear();
    pageList << xChapter.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name);
    QString newName;

    //Moving...
    for (QFileInfoList::const_iterator ite = pageList.constBegin(); ite != pageList.constEnd(); ++ite)
    {
        newName = temp.absolutePath().append("/%1.jpg").arg(pageNumber,6,10,QLatin1Char('0'));

        if (ite->suffix().contains("jpg",Qt::CaseInsensitive))
        {
            if(!QFile::copy(ite->absoluteFilePath(),newName))
                qDebug()<<"Move "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";
        }
        else
        {
            if (ite->suffix().contains("png",Qt::CaseInsensitive))
            {
                if(!pngPage.load(ite->absoluteFilePath(),"png"))
                     qDebug()<<"Load "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";

                if (!pngPage.save(newName,"jpg"))
                     qDebug()<<"Save "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";

            }
            else qDebug()<<ite->absoluteFilePath();
        }
        ++pageNumber;
    }
}
