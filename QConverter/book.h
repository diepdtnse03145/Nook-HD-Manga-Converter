#ifndef BOOK_H
#define BOOK_H

#include <QDir>

class Book
{
public:
    void convert();
    void setSource(QString xDir);
    Book();
    ~Book();

private:
    QDir source;
    QDir temp;

    void convertPage(const QString &pagePath, quint64 &pageNumber);

};

#endif // BOOK_H
