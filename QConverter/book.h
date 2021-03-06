#ifndef BOOK_H
#define BOOK_H

#include <QDir>

class QZipWriter;

class Book
{
public:
    void convert();
    void setSource(QString xDir);
    Book();
    ~Book();

private:
    QDir source;
    void convertPage(const QString &pagePath, quint64 &pageNumber, QZipWriter* output);
};

#endif // BOOK_H
