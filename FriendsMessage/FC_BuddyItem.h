#ifndef FC_BUDDYITEM_H
#define FC_BUDDYITEM_H

/**************************************************************

  @brief....:model中的每一个好友项
  @author...: zhouyi
  @date.....:2020-05-23

  ************************************************************/
#include <QObject>

class BuddyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString account READ account WRITE setAccount)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname)
//    Q_PROPERTY(QString heading READ heading WRITE setHeading NOTIFY headingChanged)
public:
    BuddyItem(QObject* parent=nullptr);
    //getter
    QString account()const;
    QString nickname()const;

    //setter
    void setAccount(const QString& str);
    void setNickname(const QString& str);
signals:
//    void dataChanged();
private:
    QString m_account;
    QString m_nickname;
};

#endif // FC_BUDDYITEM_H
