#include "book.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QImage>
#include <QApplication>
#include <private/qzipwriter_p.h>
#include <algorithm>

#define TEMPDIR (qApp->applicationDirPath() + "/temp3373")

Book::Book()
{

}

Book::~Book()
{

}

void Book::setSource(QString xDir)
{
    source.setPath(xDir);
}

void Book::convert()
{
    quint64 pageNumber = 0;
    temp.setPath(TEMPDIR);

    //create temp folder
    if(!temp.removeRecursively())
        qDebug()<<TEMPDIR<<" can't delete!";
    temp.mkpath(TEMPDIR);

    //rename and convert image to JPG
    QStringList pageList;
    QDirIterator pageIte(source.absolutePath(),QDir::Files,QDirIterator::Subdirectories);

    while(pageIte.hasNext())
    {
        pageList.append(pageIte.next());
    }
    std::sort(pageList.begin(),pageList.end());

    for(const auto& p: pageList){
        this->convertPage(p,pageNumber);
    }

    //Compressing...
    QZipWriter cbzOutput{source.absolutePath().append(".cbz")};
    QDirIterator ite{TEMPDIR};
    while(ite.hasNext()){
        if(ite.fileInfo().isFile()){
            QFile f(ite.filePath());
            f.open(QIODevice::ReadOnly);
            auto data = f.readAll();
            cbzOutput.addFile(ite.fileName(),data);
        }
        ite.next();
    }

    //Remove temp folder
    if(!temp.removeRecursively())
        qDebug()<<TEMPDIR<<" can't delete!";
}

void Book::convertPage(const QString& pagePath, quint64 &pageNumber)
{
    QString newName;
    QFileInfo page(pagePath);

    //Moving...
    newName = temp.absolutePath().append("/%1.jpg").arg(pageNumber,6,10,QLatin1Char('0'));
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

