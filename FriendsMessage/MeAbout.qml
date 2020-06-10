import QtQuick 2.14
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11

Page{
    id: mepage
    width: 420
    height: 800

    Column {
        id: column
        x: 0
        y: 0
        width: 420
        height: 207
    }

    Image {
        id: image
        x: 56
        y: 61
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "asserts/logo.jpg"
    }

    Row {
        id: row
        x: 168
        y: 61
        width: 230
        height: 100
    }

    Label {
        id: label
        x: 184
        y: 83
        width: 127
        height: 30
        text: profilemsg.nickname
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pointSize: 14

        Image {
            id: image1
            x: 147
            y: 40
            width: 43
            height: 21
            fillMode: Image.PreserveAspectFit
            source: "asserts/return.jpg"
            rotation: 180

        }
    }

    Label {
        id: label1
        x: 184
        y: 119
        width: 127
        height: 30
        text: profilemsg.account
        font.pointSize: 14
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            var component = Qt.createComponent("Setting.qml");
            if(component.status === Component.Ready){
                var object=component.createObject(mepage);
                tabBar.visible = false
            }
        }
    }
}
