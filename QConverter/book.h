#ifndef BOOK_H
#define BOOK_H
#include <QObject>
#include <QDir>
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


private:
    QDir *source;
    QDir *temp;
    QFileInfo *convertedBook;

    QProcess *ext7zip;
    QStringList arguments;

    void convertPage(const QString &pagePath, quint64 &pageNumber);

};

#endif // BOOK_H
