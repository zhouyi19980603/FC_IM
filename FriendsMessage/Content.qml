import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 2.0

Item {
    width: 480
    height: 800

    Text {
        id: element
        text: qsTr("Welcome to Login")
        font.family: "Arial"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
        font.pixelSize: 40
    }
}
