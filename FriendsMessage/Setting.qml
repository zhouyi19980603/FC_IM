    import QtQuick 2.14
    import QtQuick.Controls 2.4
    import QtQuick.Controls.Material 2.4
    import QtQuick.Layouts 1.11
//    import Profile_Handle 1.0

    Page{
        id: settingPage
        width: 420
        height: 800

        property alias heading: heading.source
        property alias nickname: nick.text
        property alias sex: sex.text

        header: ToolBar{
            id:toolbar
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
                            settingPage.destroy(); //destroy to this ui
                            tabBar.visible = true
                        }
                    }
                }
                Label {
                    id: label
                    anchors.left: image.right
                    width: 136
                    height: 32
                    anchors.leftMargin: 4
                    text: qsTr("个人信息")
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
                }
            }
            background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 40
                        border.color: "#999"
                        gradient: Gradient {
                            GradientStop { position: 0 ; color: "#fff" }
                            GradientStop { position: 1 ; color: "#eee" }
                        }
                    }
        }

        Rectangle{
            x: 0
            y: 6
            border.color: "#ccc"
            width: parent.width
            height: 70

            Image {
                id: heading
                x: 300
                y: 8
                width: 66
                height: 54
                fillMode: Image.PreserveAspectFit
                source: "asserts/logo.jpg"
            }

            Label {
                id: label1
                x: 33
                y: 20
                width: 48
                height: 37
                text: qsTr("头像")
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Image {
                id: image2
                x: 380
                y: 26
                width: 32
                height: 17
                rotation: 180
                fillMode: Image.PreserveAspectFit
                source: "asserts/return.jpg"
            }
            MouseArea{
                anchors.fill: parent
            }
        }

        Rectangle {
            x: 0
            y: 74
            width: parent.width
            height: 70
            border.color: "#cccccc"

            Label {
                id: label2
                x: 33
                y: 20
                width: 48
                height: 37
                text: qsTr("昵称")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }

            Image {
                id: image4
                x: 380
                y: 26
                width: 32
                height: 17
                fillMode: Image.PreserveAspectFit
                rotation: 180
                source: "asserts/return.jpg"
            }

            TextInput {
                id: nick
                x: 262
                y: 20
                width: 112
                height: 37
                text: profilemsg.nickname
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 14
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    var component = Qt.createComponent("UpdateName.qml");
                    if(component.status === Component.Ready){
                        var object=component.createObject(settingPage);
                        toolbar.visible = false
                    }
                }
            }
        }

        Rectangle {
            x: 0
            y: 144
            width: parent.width
            height: 70
            border.color: "#cccccc"

            Label {
                id: label3
                x: 33
                y: 20
                width: 48
                height: 37
                text: qsTr("帐号")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }

            Label {
                id: account
                x: 289
                y: 20
                width: 112
                height: 37
                text: profilemsg.account
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 14
            }
        }

        Rectangle {
            x: 0
            y: 214
            width: parent.width
            height: 70
            border.color: "#cccccc"

            Label {
                id: label4
                x: 33
                y: 20
                width: 48
                height: 37
                text: qsTr("性别")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }

            Image {
                id: image8
                x: 380
                y: 26
                width: 32
                height: 17
                fillMode: Image.PreserveAspectFit
                rotation: 180
                source: "asserts/return.jpg"
            }

            TextInput {
                id: sex
                x: 262
                y: 20
                width: 112
                height: 37
                text: profilemsg.gender
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 14
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    var component = Qt.createComponent("UpdateSex.qml");
                    if(component.status === Component.Ready){
                        var object=component.createObject(settingPage);
                        toolbar.visible = false
                    }
                }
            }
        }
    }

