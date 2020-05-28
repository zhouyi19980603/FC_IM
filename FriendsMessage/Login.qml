import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import UserType 1.0
import QtQuick.Window 2.12

Window {
    id:loginPage
    width: 420
    height: 800
    visible:true

    Rectangle {
      id: loginForm
      anchors.centerIn: parent
      color: "white"
      width: content.width + 48
      height: content.height + 16
      radius: 4
    }
    User{
      id:user
    }

    // registerform content
    GridLayout {
      id: content
      anchors.centerIn: loginForm
      columnSpacing: 20
      rowSpacing: 10
      columns: 2

      // headline
      Text {
        Layout.topMargin: 8
        Layout.bottomMargin: 12
        Layout.columnSpan: 2
        Layout.alignment: Qt.AlignHCenter
        text: "Login"
      }

      // email text and field
      Text {
        text: qsTr("E-mail")
        font.pixelSize: 12
      }

      TextField {
        id: txtUsername
        Layout.preferredWidth: 200
        font.pixelSize: 14
      }

      // password text and field
      Text {
        text: qsTr("Password")
        font.pixelSize: 12
      }

      TextField {
        id: txtPassword
        Layout.preferredWidth: 200
        font.pixelSize: 14
        echoMode: TextInput.Password //field
      }
      Loader{
        id:loader
      }

      // column for buttons, we use column here to avoid additional spacing between buttons
      Column {
        Layout.fillWidth: true
        Layout.columnSpan: 2
        Layout.topMargin: 12

        // buttons
        Button {
          text: qsTr("Login")
          flat: false
          anchors.horizontalCenter: parent.horizontalCenter
          onClicked: {
              console.log(txtUsername.text,txtPassword.text)
//              user.test(txtUsername.text,txtPassword.text)
              //相当于传递的是QString类型
              message_handle.login(txtUsername.text,txtPassword.text)


//              if(user.queryMessage(txtUsername.text,txtPassword.text))
//              {
                  var component = Qt.createComponent("Core.qml");
                  component.createObject(loginPage);
                  console.log("login succed")

//              }
          }
        }

        Button {
          text: qsTr("No account yet? Register now")
          flat: true
          anchors.horizontalCenter: parent.horizontalCenter
          onClicked: {
            var component = Qt.createComponent("Register.qml");
            component.createObject(loginPage);
            console.debug("registering...")
          }
        }
      }
    }
}
