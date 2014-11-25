#ifndef BOOK_H
#define BOOK_H
#include <QObject>
#include <QDir>
#include <QImage>

class Book : public QObject
{
    Q_OBJECT

public:
    void convert();
    void setSource(QString xDir);


signals:
    void percentCompleted(int );

private:
    QDir source;
    QDir temp;
    QFileInfoList chapterList;
    QFileInfoList pageList;
    quint64 pageNumber;
    QImage pngPage;

    void convertChapter(QDir xChapter);

};

#endif // BOOK_H
