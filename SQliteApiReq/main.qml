import QtQuick 2.12
import QtQuick.Controls 2.12
import HttpRequestManager 1.0
import DatabaseManager 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Qt Quick HTTP Request Example"
    HttpRequestManager {
        id: requestManager

        property int numberOfPeople: 1 // количество запросов
        property ListModel peopleModel: ListModel {
            id: model
        }

        onRequestFinished: {
            databaseManager.insertData(data)
            databaseManager.fetchData()
        }
    }
    DatabaseManager {
        id: databaseManager
        onDataFetched: {
            dataModel.clear()
            for (var i = 0; i < dataList.length; ++i) {
                dataModel.append(dataList[i])
            }
        }
    }
    Component.onCompleted: {
        databaseManager.openDatabase()
        databaseManager.createTable()
    }
    Rectangle {
        id: recList
        height: 300
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: reqBut.left
        border.color: "black"
        border.width: 1

        ListView {
            id: listView
            width: parent.width
            height: parent.height - 2

            model: ListModel {
                id: dataModel
            }
            Component.onCompleted: {
                databaseManager.fetchData()
            }
            delegate: Item {
                id: person
                width: parent.width
                height: 50
                property int currentIndex: index
                Rectangle {
                    id: coloredRec
                    width: parent.width
                    height: 50
                    border.color: "black"
                    border.width: 1
                    color: "lightblue"
                    Text {
                        anchors.centerIn: parent
                        text: model.name + " " + model.gender + " Комментарий: " + model.comments
                    }
                    MouseArea {
                        anchors.fill: coloredRec
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: {
                            if (mouse.button === Qt.RightButton) {
                                menu.popup()
                            }
                        }
                    }
                    Menu {
                        id: menu
                        MenuItem {
                            text: "Открыть на отдельной странице"
                            onTriggered: {
                                openDetailsPage(person.currentIndex)
                            }
                        }

                        MenuItem {
                            text: "Удалить элемент"
                            onTriggered: {
                                // Удаление элемента из списка и базы данных
                                var index = person.currentIndex
                                databaseManager.removeData(index + 1)
                            }
                        }
                        MenuItem {
                            text: "Добавить коментарий"
                            onTriggered: {
                                var index = person.currentIndex
                                var inputDialogComponent = Qt.createComponent(
                                            "InputDialog.qml")
                                if (inputDialogComponent.status === Component.Ready) {
                                    var inputDialog = inputDialogComponent.createObject()
                                    inputDialog.show()
                                    inputDialog.accepted.connect(
                                                function (text) {
                                                    databaseManager.addComment(
                                                                index + 1, text)
                                                })
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Button {
        id: reqBut
        anchors.top: recList.bottom
        text: "Сделать запрос"
        onClicked: {
            for (var i = 0; i < requestManager.numberOfPeople; ++i) {
                requestManager.makeRequest()
            }
        }
    }
    SpinBox {
        id: numberOfPeopleInput
        anchors.left: reqBut.right
        anchors.top: recList.bottom
        from: 1
        to: 100
        value: requestManager.numberOfPeople
        stepSize: 1
        anchors.bottomMargin: 10
        textFromValue: function (value) {
            return "Количество запросов: " + value
        }
        onValueChanged: {
            requestManager.numberOfPeople = value
        }
    }
    Button {
        text: "Очистить базу данных"
        anchors.left: numberOfPeopleInput.right
        anchors.top: recList.bottom
        onClicked: {
            databaseManager.clearData()
        }
    }
    function openDetailsPage(index) {
        var personData = databaseManager.fetchPersonData(index)
        var detailsPageComponent = Qt.createComponent("DetailsPage.qml")

        if (detailsPageComponent.status === Component.Ready) {
            var detailsPageObject = detailsPageComponent.createObject(null, {
                                                                          "personData": personData
                                                                      })
            detailsPageObject.show()
        } else {
            console.error("Error creating DetailsPage component:",
                          detailsPageComponent.errorString())
        }
    }
}
