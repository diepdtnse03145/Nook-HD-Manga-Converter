#include "book.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QDirIterator>
#include <QImage>

#define TEMPDIR "/temp3373"

Book::Book(QObject *parent):QObject(parent)
{
    source = new QDir();
    temp = new QDir();
    ext7zip = new QProcess(this);
}

Book::~Book()
{
    delete source;
    delete temp;
    delete ext7zip;
}

void Book::setSource(QString xDir)
{
    source->setPath(xDir);
}

void Book::convert()
{
    //init chapter list
    quint64 pageNumber = 0;

    temp->setPath(TEMPDIR);
    arguments.clear();

    //create temp folder
    if(!temp->removeRecursively())
        qDebug()<<TEMPDIR<<" can't delete!";
    temp->mkpath(TEMPDIR);

    //Convert Book
    QDirIterator pageIte(source->absolutePath(),QDir::Files,QDirIterator::Subdirectories);
    while(pageIte.hasNext())
    {
        //Convert page
        this->convertPage(pageIte.next(),pageNumber);
    }

    //Compressing...
    arguments << "a" << source->absolutePath().append(".zip") << temp->absolutePath().append("/*");
    ext7zip->start(QDir::currentPath() + QStringLiteral("/7za"), arguments);
    qDebug()<<source->absolutePath().append(".cbz");
    if(ext7zip->waitForFinished(-1))
    {
        //Rename
        QFile::rename(source->absolutePath().append(".zip"), source->absolutePath().append(".cbz"));

        //delete
        if(!temp->removeRecursively())
            qDebug()<<TEMPDIR<<" can't delete!";
    }

}

void Book::convertPage(const QString& pagePath, quint64 &pageNumber)
{
    //init page list
    QString newName;
    QFileInfo page(pagePath);

    //Moving...
    newName = temp->absolutePath().append("/%1.jpg").arg(pageNumber,6,10,QLatin1Char('0'));
    if (page.suffix().contains("jpg",Qt::CaseInsensitive))
    {
        if(!QFile::copy(page.absoluteFilePath(),newName))
            qDebug()<<"Move "<<page.absoluteFilePath()<<" to "<<newName<<" fail";
    }
    else
    {
        if (page.suffix().contains("png",Qt::CaseInsensitive))
        {
            QImage *pngPage = new QImage();

            if(!pngPage->load(page.absoluteFilePath(),"png"))
                qDebug()<<"Load "<<page.absoluteFilePath()<<" to "<<newName<<" fail";

            if (!pngPage->save(newName,"jpg"))
                qDebug()<<"Save "<<page.absoluteFilePath()<<" to "<<newName<<" fail";

            delete pngPage;

        }
        else qDebug()<<page.absoluteFilePath();
    }
    ++pageNumber;
}

