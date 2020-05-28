import QtQuick 2.2
import QtQuick.Controls 2.4

Page{
    id: addui
    visible: true
    width: 420
    height: 800
    //屏蔽父对象的鼠标事件
    MouseArea{
        anchors.fill: parent
    }
    Image {
        id: image
        x: 0
        y: 8
        width: 32
        height: 32
        fillMode: Image.PreserveAspectFit
        source: "asserts/return.jpg"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                console.log("mouseArea")
//                addui.destroy(); //destroy to this ui
                var component = Qt.createComponent("Main.qml");
                if(component.status === Component.Ready){
                    var object=component.createObject(addui);
                }
            }
        }
    }
    Row{
        x: 8
        y: 64
        spacing: 2
        Rectangle{
            width: 300
            height: 45
            border.color: "green"

            Row {
                id: row
                anchors.fill: parent
                spacing: 2

                Image {
                    id: findImage
                    width: 30
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "asserts/find.jpg"
                }

                TextInput {
                    id: textInputSearch
                    width: 260
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    selectionColor: "#02020a"
                    font.pixelSize: 14

                    onTextChanged: {
                        buttonText.text = "搜索"
                    }
                }

            }
        }

        Rectangle {
            id: rectangle
            width: 80
            height: parent.height
            color: "#ffffff"
            Text {
                id: buttonText
                anchors.fill: parent
                text: qsTr("取消")
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                MouseArea{
                    id:findClick
                    anchors.fill: parent
                    onClicked: {
                        console.log(textInputSearch.text)
                        console.log("Friends click")
                        var component = Qt.createComponent("FriendsMessage.qml");
                        if(component.status === Component.Ready){
                            var object=component.createObject(addui);
                        }
                        message_handle.search_friends(textInputSearch.text)
                    }
                }
            }
        }
    }

    function changeTextSearch(msg)
    {
        console.log(msg)
        textInputSearch.text=msg
    }

}
