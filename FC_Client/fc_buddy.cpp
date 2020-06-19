#include "fc_buddy.h"

Buddy* Buddy::instance = nullptr;

Buddy::Buddy(QObject *parent)
    :QObject(parent)
{
    m_value = "0";
}

Buddy *Buddy::getInstance()
{
    if(instance == nullptr)
        instance = new Buddy ();
    return instance;
}

QString Buddy::account() const
{
    return m_account;
}

QString Buddy::nickname() const
{
    return m_nickname;
}

QString Buddy::heading() const
{
    return m_heading;
}

QString Buddy::value() const
{
    return m_value;
}

void Buddy::setAccount(const QString &str)
{
    m_account = str;
    emit dataChanged();
}

void Buddy::setNickname(const QString &str)
{
    m_nickname = str;
    emit dataChanged();
}

void Buddy::setHeading(const QString &str)
{
    m_heading = str;
    emit dataChanged();
}

void Buddy::setValue(const QString &str)
{
    m_value = str;
    emit valueChanged();
}
