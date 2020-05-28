/**************************************************************

  @brief....: 显示好友列表的接口
  @author...: zhouyi
  @date.....:2020-05-14

  ************************************************************/


import QtQuick 2.14
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import buddy 1.0

Page {
    visible: true
    width: 420
    height: 800
    Material.background: "white"
    header: ToolBar{
        Label{
            text: qsTr("好友列表")
            styleColor: "#dc5757"
            font.pointSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
        }
    }


    ListView{
        id:listView
        anchors.fill: parent
        anchors.top: parent.top
        anchors.topMargin:20
        spacing: 20
        Material.background: "white"
        model: teamModel.teams
        delegate: list_delegate

    }
    Component{
        id:list_delegate

        Column{
            id:objColumn


            //在这里设置的是否显现
            Component.onCompleted: {
                for(var i = 1; i < objColumn.children.length - 1; ++i) {
                    objColumn.children[i].visible = false
                }
            }

            MouseArea{
                width:listView.width
                height: objItem.implicitHeight
                enabled: objColumn.children.length > 2
                onClicked: {
                    console.log("onClicked..")
                    var flag = false;
                    for(var i = 1; i < parent.children.length - 1; ++i) {
                        console.log("onClicked..i=",i)
                        flag = parent.children[i].visible;
                        parent.children[i].visible = !parent.children[i].visible
                    }
                    console.log("onClicked..flag = ",flag)

                    //旋转，图片
                    if(!flag){
                        iconAin.from = 0
                        iconAin.to = 90
                        iconAin.start()
                    }
                    else{
                        iconAin.from = 90
                        iconAin.to = 0
                        iconAin.start()
                    }
                }
                Row{
                    id:objItem
                    spacing: 10
                    leftPadding: 20

                    Image {
                        id: icon
                        width: 10
                        height: 20
                        source: "asserts/right.jpg"
                        anchors.verticalCenter: parent.verticalCenter
                        RotationAnimation{
                            id:iconAin
                            target: icon
                            duration: 100
                        }
                    }
                    Label{
                        id:meeting_name
                        text: teamname
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label{
                        text: date
                        color:"grey"
                        anchors.verticalCenter: parent.verticalCenter

                    }
                }
            }
            Repeater {
               model:buddys

               delegate: Rectangle{
                   id:rect
                   width: 500
                   height: 120
                   Rectangle {
                       id: fileicon
                       width: 80
                       height: 80
                       radius: 50
                       color:index%2?"red":"yellow"
                       anchors{
                           left: parent.left
                           leftMargin: 20
                           verticalCenter: parent.verticalCenter
                       }

                       //使其位于margin
                   }
                   Column{
                       anchors{
                           left: fileicon.right
                           leftMargin: 20
                           top: parent.top
                           topMargin:20
                       }
                       topPadding: 10
                       spacing: 10

                       Label{
                           text: account
                       }
                       Label{
                           text: nickname
                           color: "grey"
                       }
                   }
                   MouseArea{
                    anchors.fill: rect
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        if(mouse.button == Qt.RightButton)
                            contextMenu.popup()
                    }
                    //弹出菜单
                    Menu {
                              id: contextMenu
                              MenuItem { text: "修改好友备注"
                                onClicked: console.log("wef")
                              }
                              MenuItem { text: "删除好友" }
                              MenuItem { text: "移动联系人至"}
                          }
                    }
               }

            }
        }
    }


}
