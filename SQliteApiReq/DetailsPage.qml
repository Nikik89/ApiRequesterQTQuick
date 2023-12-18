import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Details Page"

    property var personData

    Column {
        anchors.centerIn: parent
        spacing: 10

        Label {
            text: "Details for Person"
            font.bold: true
        }

        Label {
            text: "Name: " + personData.name
        }

        Label {
            text: "Gender: " + personData.gender
        }

        Label {
            text: "Location: " + personData.location
        }

        Label {
            text: "Email: " + personData.email
        }

        Label {
            text: "Username: " + personData.login
        }

        Label {
            text: "Date of Birth: " + personData.dob
        }

        // Кнопка для закрытия окна
        Button {
            text: "Close"
            onClicked: {
                // Закрыть окно
                close()
            }
        }
    }
}
