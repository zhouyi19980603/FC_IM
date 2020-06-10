import QtQuick 2.14
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11

/**************************************************************

  @brief....:这是修改用户性别时的好友界面
  @author...: zhouyi
  @date.....:2020-06-08

  ************************************************************/

Page{
    id: updatesex
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
                        updatesex.destroy()
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
                text: qsTr("更改性别")
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
                onClicked:
                {
                    profile_handle.updateGender(combobox.currentText);
                    toolbar.visible = true;
                    updatesex.destroy();
                }

            }
        }
        background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    border.color: "white"
        }

    }


    ComboBox {
        id:combobox
        width: parent.width
        font.pointSize: 14
        model: ["男","女"]
        background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    border.color: "white"
        }
    }


}
