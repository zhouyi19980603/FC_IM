#ifndef FC_BUDDYMODEL_H
#define FC_BUDDYMODEL_H
#include <QObject>
#include <QQmlListProperty>
#include "FC_BuddyTeam.h"
#include "fc_client.h"

/**************************************************************

  @brief....:使用了单例模式，使只会产生一个model
  @author...: zhouyi
  @date.....:2020-05-26

  ************************************************************/


class BuddyModel : public QObject
{
  Q_OBJECT
    Q_PROPERTY(QQmlListProperty<BuddyTeam> teams READ teams)
public:
    static BuddyModel* getInstance();
    QQmlListProperty<BuddyTeam> teams();
    void appendTeam(BuddyTeam*);
    int teamCount() const;
    BuddyTeam *team(int)const;
    void clearTeams();


private:
    static BuddyModel* instance;

    BuddyModel(QObject* parent=nullptr);
    static void appendTeam(QQmlListProperty<BuddyTeam>*,BuddyTeam*);
    static int teamCount(QQmlListProperty<BuddyTeam>*);
    static BuddyTeam* team(QQmlListProperty<BuddyTeam>*,int);
    static void clearTeams(QQmlListProperty<BuddyTeam>*);

private:
    QVector<BuddyTeam*>m_teams;
};

#endif // FC_BUDDYMODEL_H
