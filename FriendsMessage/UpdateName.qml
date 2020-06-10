import QtQuick 2.14
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11

/**************************************************************

  @brief....:这是修改用户昵称时的界面
  @author...: zhouyi
  @date.....:2020-06-08

  ************************************************************/

Page{
    id: updatename
    width: 420
    height: 800
    MouseArea{
        anchors.fill: parent
    }
    header: ToolBar{
        id:toolbar1
        Row {
            id: row
            height: 51
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Image {
                id: image
                x: 5
                y: 8
                width: 24
                height: 24
                fillMode: Image.PreserveAspectFit
                source: "asserts/return.jpg"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        updatename.destroy()
                        toolbar.visible = true
                    }
                }
            }
            Label {
                id: label
                anchors.left: image.right
                width: 136
                height: 32
                anchors.leftMargin: 4
                text: qsTr("更改名字")
                font.pointSize: 15
                verticalAlignment: Text.AlignVCenter
            }
            Button {
                id: save
                anchors.right: parent.right
                anchors.rightMargin: 5
                width: 59
                height: 32
                text: qsTr("保存")
                font.pointSize: 14
                onClicked:{
                    profile_handle.updateNick(newname.text)
                    toolbar.visible = true;
                    updatename.destroy();
                }
            }
        }
        background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    border.color: "white"
        }

    }

    TextInput {
        id: newname
        x: 15
        y: 49
        width: 383
        height: 40
        text: settingPage.nickname
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14

    }

    Label {
        id: textInput1
        x: 15
        y: 102
        width: 383
        height: 25
        text: qsTr("换个名字，小伙伴更容易找到你哟")
        font.pixelSize: 12
    }


}
