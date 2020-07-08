import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 400
    height: 450
    title: qsTr("Password Authentication")

    Connections {
        target: ufc
        onErrorMessage: {
            console.log(message)
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 12
        property var login: true

        Label {
            text: parent.login
                  ? "Retrieve key by logging in with the username and password"
                  : "Create a new UserFile based on the username and password"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ColumnLayout {
            id: registerItem
            visible: !parent.login
            spacing: 4
            anchors.horizontalCenter: parent.horizontalCenter

            TextField {
                id: usernameIn
                placeholderText: "Username"
            }

            TextField {
                id: passIn
                placeholderText: "Password"
                echoMode: TextInput.Password
            }

            TextField {
                id: passIn2
                placeholderText: "Repeat password"
                echoMode: TextInput.Password
            }

            Button {
                text: "Create file"
                onClicked: {
                    if (passIn.text === passIn2.text){
                        ufc.generateFile(usernameIn.text, passIn.text) ?
                                    text = "Success!" : text = "Fail"
                    }
                }
            }
        }

        ColumnLayout {
            id: loginItem
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 4
            visible: !registerItem.visible

            TextField {
                id: usernameLogin
                placeholderText: "Username"
            }

            TextField {
                id: passLogin

                placeholderText: "Password"
                echoMode: TextInput.Password
            }

            Button {
                text: "Retrieve key"

                onClicked: ufc.loadFile(usernameLogin.text, passLogin.text) ?
                                text = "Success!" : text = "Fail"
            }
        }

        Button {
            text: parent.login ? "Register" : "Get key"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: parent.login = !parent.login
        }

        Button {
            text: "Print key"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: ufc.printKey()
        }
    }
}
