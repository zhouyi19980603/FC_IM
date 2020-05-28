import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtQuick.Controls.Material 2.0

Page {
    id: addPage
    visible: true
    width: 420
    height: 800


    Material.theme: Material.Light
    Material.accent: Material.DeepOrange
    Material.primary: Material.Blue
    ColumnLayout{
        anchors.fill: parent
        ListView {
            id: swipeView
            //anchors.fill: parent
            Layout.fillWidth:true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            model:pages
            delegate: delegatePages
            highlightMoveDuration: 1
            highlightMoveVelocity:1000
            orientation: ListView.Horizontal
            highlightRangeMode:ListView.StrictlyEnforceRange
            snapMode: ListView.SnapOneItem
            boundsBehavior: Flickable.StopAtBounds
        }

        ListModel{
            id:pages
            ListElement{
                // @disable-check M16
                pageSource:"MessagePage.qml"
            }
            ListElement{
                // @disable-check M16
                pageSource:"Buddy.qml"
            }
            ListElement{
                // @disable-check M16
                pageSource:"Content.qml"
            }

        }
        Component{
            id:delegatePages
            Loader{
                    width:ListView.view.width
                    height:ListView.view.height
                    source: pageSource
           }
        }

    }

    footer: TabBar{
        id:tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
                    text: qsTr("消息")
                }
                TabButton {
                    text: qsTr("联系人")
                }
                TabButton {
                    text: qsTr("动态")
                }
    }
}

