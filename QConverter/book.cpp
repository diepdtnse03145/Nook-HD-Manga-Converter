#include "book.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QProcess>

#define TEMPDIR "/temp3373"

Book::Book(QObject *parent):QObject(parent)
{
    source = new QDir();
    temp = new QDir();
    pngPage = new QImage();
    ext7zip = new QProcess(this);
}

Book::~Book()
{
    delete source;
    delete temp;
    delete pngPage;
    delete ext7zip;
}

void Book::setSource(QString xDir)
{
    source->setPath(xDir);
}

void Book::convert()
{
    //init chapter list
    pageNumber = 0;
    int i = 0;
    chapterList.clear();
    chapterList << source->entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name | QDir::DirsFirst);
    temp->setPath(TEMPDIR);
    arguments.clear();

    //create temp folder
    if(!temp->removeRecursively())
       qDebug()<<TEMPDIR<<" can't delete!";
    temp->mkpath(TEMPDIR);

    //Moving images
    for (QFileInfoList::iterator ite = chapterList.begin(); ite != chapterList.end(); ++ite)
    {
        i++;
        this->convertChapter(*ite);
        emit percentCompleted((i*100)/(chapterList.size()));
    }

    //Compressing...      
    arguments << "a" << source->absolutePath().append(".zip") << temp->absolutePath().append("/*");
    ext7zip->start("D:/7za", arguments);

    if(ext7zip->waitForFinished(-1))
    {
        //Rename
        QFile::rename(source->absolutePath().append(".zip"), source->absolutePath().append(".cbz"));

        //delete
        if(!temp->removeRecursively())
            qDebug()<<TEMPDIR<<" can't delete!";
        qDebug()<<source->absolutePath();
    }

}

void Book::convertChapter(QFileInfo &xChapter)
{
    //init page list
    pageList.clear();
    pageList << QDir(xChapter.absoluteFilePath()).entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name);
    QString newName;

    //Moving...
    for (QFileInfoList::const_iterator ite = pageList.constBegin(); ite != pageList.constEnd(); ++ite)
    {
        newName = temp->absolutePath().append("/%1.jpg").arg(pageNumber,6,10,QLatin1Char('0'));

        if (ite->suffix().contains("jpg",Qt::CaseInsensitive))
        {
            if(!QFile::copy(ite->absoluteFilePath(),newName))
                qDebug()<<"Move "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";
        }
        else
        {
            if (ite->suffix().contains("png",Qt::CaseInsensitive))
            {
                if(!pngPage->load(ite->absoluteFilePath(),"png"))
                     qDebug()<<"Load "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";

                if (!pngPage->save(newName,"jpg"))
                     qDebug()<<"Save "<<ite->absoluteFilePath()<<" to "<<newName<<" fail";

            }
            else qDebug()<<ite->absoluteFilePath();
        }
        ++pageNumber;
    }
}
