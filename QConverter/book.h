#ifndef BOOK_H
#define BOOK_H
#include <QObject>
#include <QDir>
#include <QImage>
#include <QProcess>

class Book : public QObject
{
    Q_OBJECT

public:
    void convert();
    void setSource(QString xDir);
    Book(QObject *parent = 0);
    ~Book();

signals:
    void percentCompleted(int );

private:
    QDir *source;
    QDir *temp;
    QFileInfo *convertedBook;

    QFileInfoList chapterList;
    QFileInfoList pageList;
    quint64 pageNumber;
    QImage *pngPage;
    QProcess *ext7zip;
    QStringList arguments;

    void convertChapter(QFileInfo &xChapter);

};

#endif // BOOK_H
