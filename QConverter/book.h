#ifndef BOOK_H
#define BOOK_H
#include <QObject>
#include <QDir>

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
    QFileInfoList chapterList;
    QFileInfoList pageList;
    quint64 pageNumber;

    void convertChapter(QDir xChapter, quint64 &xPage);

};

#endif // BOOK_H
