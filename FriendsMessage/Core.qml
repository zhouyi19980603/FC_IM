import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4

Page{
    id: corePage
    visible: true
    width: 420
    height: 800


    Component.onCompleted: {
        var component = Qt.createComponent("Main.qml");
        if(component.status === Component.Ready){
            var object=component.createObject(corePage);
            console.log("click")
        }
    }
}
