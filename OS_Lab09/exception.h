#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <QString>

class Exception: public std::exception{
public:
    Exception(): m_message(""){}
    Exception(QString message) : m_message(message){}
    const char* what(){
        return m_message.toUtf8().data();
    }
private:
    QString m_message;
};

#endif // EXCEPTION_H
