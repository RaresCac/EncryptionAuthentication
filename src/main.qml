import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Password Authentication")

    Column {
        TextField {
            id: usernameIn
            placeholderText: "Username"
        }

        TextField {
            id: passIn
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        Button {
            text: "Create file"
            onClicked: encryptor.generateFileQML(usernameIn.text, passIn.text)
        }
    }

}
