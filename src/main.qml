import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 400
    height: 300
    title: qsTr("Password Authentication")

    Connections {
        target: ufc
        onErrorMessage: {
            console.log(message)
        }
    }

    Column {
        anchors.fill: parent

        TextField {
            id: usernameIn
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Username"
        }

        TextField {
            id: passIn
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        Button {
            text: "Create file"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: ufc.generateFile(usernameIn.text, passIn.text) ?
                            text = "Success!" : text = "Fail"
        }
    }
}
