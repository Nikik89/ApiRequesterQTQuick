import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: inputDialog
    width: 300
    height: 150
    visible: false
    color: "transparent"
    signal accepted(string text)
    signal rejected
    Rectangle {
        width: 300
        height: 150
        color: "white"

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                text: "Введите комментарий:"
            }

            TextField {
                id: inputField
                width: parent.width - 20
                placeholderText: "Комментарий"
            }

            Row {
                spacing: 10

                Button {
                    text: "Отмена"
                    onClicked: {
                        inputDialog.rejected()
                        inputDialog.close()
                    }
                }

                Button {
                    text: "Ок"
                    onClicked: {
                        inputDialog.accepted(inputField.text)
                        inputDialog.close()
                    }
                }
            }
        }
    }
}
