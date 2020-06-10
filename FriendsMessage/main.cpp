#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "database.h"
#include "fc_client.h"
#include "fc_message_handle.h"
#include "FC_BuddyModel.h"
#include "FC_BuddyItem.h"
#include "FC_BuddyTeam.h"
#include "fc_profile_handle.h"
#include "fc_profile.h"
#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<User>("UserType", 1, 0, "User");
    qmlRegisterType<FC_Message_Handle>("Message_Handle",1,0,"Handle");
    qmlRegisterType<BuddyItem>("buddy",1,0,"BuddyItem");
    qmlRegisterType<BuddyTeam>("buddy",1,0,"BuddyTeam");
//    qmlRegisterType<FC_Profile>("Profile_Handle",1,0,"Profile");
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/Login.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    FC_Client *clients=new FC_Client();
    FC_Message_Handle *handle;
    ProfileMsg* profilemsg;
    profilemsg = ProfileMsg::getInstance();
    BuddyModel* model;
    model = BuddyModel::getInstance();
    handle= new FC_Message_Handle(clients);
    FC_Profile* profile = new FC_Profile (clients);
    engine.rootContext()->setContextProperty("message_handle",handle);
    engine.rootContext()->setContextProperty("teamModel",model);
    engine.rootContext()->setContextProperty("profile_handle",profile);
    engine.rootContext()->setContextProperty("profilemsg",profilemsg);

    engine.load(url);

    return app.exec();
}

