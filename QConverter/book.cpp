#include "book.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QImage>
#include <QApplication>
#include <private/qzipwriter_p.h>
#include <algorithm>
#include <QBuffer>


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

    //rename and convert image to JPG
    QStringList pageList;
    QDirIterator pageIte(source.absolutePath(),QDir::Files,QDirIterator::Subdirectories);

    while(pageIte.hasNext())
    {
        pageList.append(pageIte.next());
    }
    std::sort(pageList.begin(),pageList.end());

    QZipWriter cbzOutput{source.absolutePath().append(".cbz")};

    for(const auto& p: pageList) {
        this->convertPage(p,pageNumber,&cbzOutput);
    }
}

void Book::convertPage(const QString &pagePath, quint64 &pageNumber, QZipWriter* output)
{
    QImage page;
    QString newName = QStringLiteral("%1.jpg").arg(pageNumber,6,10,QLatin1Char('0'));

    if(!page.load(pagePath)) {
        qDebug()<<"Can't load"<<pagePath<<"into QImage";
    }

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    if(!page.save(&buffer,"jpg")) {
        qDebug()<<"Can't save"<<pagePath<<"into QBuffer";
    }

    output->addFile(newName,ba);
    ++pageNumber;
}

