import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import style 1.0
import counterManager 1.0

Window {
    readonly property int menuHeight: 50

    function findCounterIndex(name: string)
    {
        for (let i = 0; i < listModel.count; i++)
        {
            if (listModel.get(i).n === name) return i;
        }
        return -1;
    }

    function updateStats()
    {
        statsPage.counters = backend.getCounters()
    }

    id: root
    width: 640
    height: 480
    visible: true
    title: "Apology Counter"
    color: Style.primary

    CounterManager {
        id: backend
    }

    Component.onCompleted: () =>
                           {
                               console.log("ON COMPLETED CALLED")
                               for (const counterData of backend.getCounters())
                               {
                                   listModel.append({dispName: counterData.displayName, n: counterData.name})
                               }
                               console.log(backend.getCounters())
                               updateStats()
                           }

    Dialog {
        id: dialog
        title: "New counter"
        readonly property int globalPadding: 10

        contentItem: Rectangle {
            color: Style.primaryDark
            implicitWidth: dialogCol.width
            implicitHeight: dialogCol.height
            Column {
                id: dialogCol
                Text {
                    text: "Type the name of the person who apologizes a lot"
                    color: Style.text
                    font.pixelSize: 13
                    bottomPadding: 6
                    topPadding: dialog.globalPadding
                    rightPadding: dialog.globalPadding
                    leftPadding: dialog.globalPadding
                }
                TextField {
                    id: dialogTextField
                    placeholderText: "Name"
                    font.pixelSize: 13
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: dialog.globalPadding
                    anchors.rightMargin: dialog.globalPadding
                    color: Style.text
                    background: Rectangle {
                        color: Style.primaryLight
                        radius: parent.height / 2
                        border.width: parent.activeFocus ? 1 : 0
                        border.color: Style.secondaryLight
                    }
                }
                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    layoutDirection: Qt.RightToLeft
                    topPadding: dialog.globalPadding
                    leftPadding: dialog.globalPadding
                    rightPadding: dialog.globalPadding
                    bottomPadding: dialog.globalPadding
                    spacing: dialog.globalPadding
                    RoundButton {
                        id: okButton
                        text: "OK"
                        height: 30
                        width: 80
                        font.pixelSize: 13
                        palette.button: Style.secondary
                        palette.buttonText: Style.text
                        onPressed: () =>
                                   {
                                       backend.createNewCounter(dialogTextField.text)
                                       dialog.close()
                                   }
                    }
                    RoundButton {
                        id: cancelButton
                        text: "Cancel"
                        height: 30
                        width: 80
                        font.pixelSize: 13
                        palette.button: Style.secondary
                        palette.buttonText: Style.text
                        onPressed: () =>
                                   {
                                       dialog.close()
                                   }
                    }
                }
            }
        }

        Connections {
            target: backend
            function onDisplayNewCounterDialog() {
                dialog.open()
            }
        }
    }

    SwipeView {
        id: swipeView
        width: parent.width
        height: parent.height - menuHeight
        currentIndex: 1

        Settings {
            id: settingsPage
        }

        ListView {
            id: listView
            spacing: 1
            delegate: SwipeDelegate {
                id: swipeDelegate
                width: listView.width
                height: ct.height

                background: Counter {
                    id: ct
                    width: parent.width
                    displayName: dispName
                    name: n

                    Connections {
                        target: ct.counterBackend
                        function onDeleteQmlCounter(name: string)
                        {
                            listModel.remove(findCounterIndex(name))
                            backend.clearSettingsForCounter(name)
                            updateStats()
                        }
                    }
                    Connections {
                        target: ct
                        function onMoveDown()
                        {
                            //Check if last one
                            let index = findCounterIndex(n)
                            if (index === listModel.count - 1) return
                            listModel.move(index, index + 1, 1)
                            backend.swap(index, index + 1)
                        }
                        function onMoveUp()
                        {
                            //Check if first one
                            let index = findCounterIndex(n)
                            if (index === 0) return
                            listModel.move(index, index - 1, 1)
                            backend.swap(index, index - 1)
                        }
                    }
                }
                swipe.left: Rectangle {
                    height: parent.height
                    width: root.width / 2
                    color: Style.destructive
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                        spacing: 10
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Resources/trash-fill.svg"
                            anchors.leftMargin: 0
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }
                        Text {
                            text: "Remove"
                            color: Style.text
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                swipe.right: Rectangle {
                    height: parent.height
                    width: root.width / 2
                    color: Style.stats
                    anchors.right: parent.right
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        layoutDirection: Qt.RightToLeft
                        anchors.rightMargin: 0
                        rightPadding: 10
                        spacing: 10
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Resources/bar-chart-line-fill.svg"
                            anchors.leftMargin: 0
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }

                        Text {
                            text: "Show stats"
                            color: Style.text
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                swipe.onCompleted: () =>
                                   {
                                       //Left (delete)
                                       if (swipe.position === 1)
                                       {
                                           ct.requestDeleteCounter()
                                           swipe.close()
                                       }
                                       //Right (stats)
                                       else if (swipe.position === -1)
                                       {
                                           swipe.close()
                                           statsPage.initName = ct.name
                                           swipeView.setCurrentIndex(2)
                                       }
                                   }
                onPressed: swipeView.interactive = false
                onCanceled: swipeView.interactive = true
            }
            remove: Transition {
                PropertyAnimation {
                    property: "x"
                    from: 0
                    to: root.width
                    duration: 200
                }
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 200
                }
            }
            model: ListModel {
                id: listModel
            }
            Connections {
                target: backend
                function onAddNewCounter(displayName: string, name: string) {
                    listModel.append({dispName: displayName, n: name})
                    updateStats()
                }
            }
        }

        Stats {
            id: statsPage
        }

        onCurrentIndexChanged: () =>
                               {
                                   if (swipeView.currentIndex !== 1)
                                   {
                                       centerStack.replace(mainPageButton)
                                       //Stats
                                       if (swipeView.currentIndex === 2)
                                       {
                                           statsPage.update()
                                           statsButton.checked = true
                                       }
                                       //Settings
                                       else if (swipeView.currentIndex === 0)
                                       {
                                           settingsPage.counters = backend.getCounters()
                                           settingsPage.update()
                                           settingsButton.checked = true
                                       }
                                   }
                                   else
                                   {
                                       centerStack.replace(addNewButton)
                                       statsButton.checked = false
                                       settingsButton.checked = false
                                   }
                               }
    }

    Rectangle {
        id: menuBorderRect
        y: parent.height - height
        width: parent.width
        height: menuHeight
        color: Style.secondaryLight

        Rectangle {
            id: menuRect
            anchors {
                fill: parent
                topMargin: 1
            }
            color: Style.primaryDark



            Grid {
                id: grid
                columns: 3
                rows: 1
                spacing: 30
                verticalItemAlignment: Grid.AlignVCenter
                horizontalItemAlignment: Grid.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Button {
                    id: settingsButton
                    width: 65
                    height: 40
                    icon.source: "qrc:/Resources/sliders.svg"
                    icon.color: checked ? Style.secondaryLight : Style.text
                    checkable: true
                    autoExclusive: true
                    bottomPadding: 16
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                    palette.buttonText: Style.text

                    onCheckedChanged: () =>
                                      {
                                          let clr = checked ? Style.secondaryLight : Style.text
                                          settingsLabel.color = clr
                                          if (checked)
                                          {
                                              centerStack.replace(mainPageButton)
                                              swipeView.setCurrentIndex(0)
                                          }
                                      }

                    Text {
                        id: settingsLabel
                        text: "Settings"
                        color: Style.text
                        font.pixelSize: 13
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                StackView {
                    id: centerStack
                    width: addNewButton.width
                    height: addNewButton.height

                    replaceEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 200
                        }
                        PropertyAnimation {
                            property: "y"
                            from: -15
                            to: 0
                            duration: 200
                        }
                    }

                    replaceExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to: 0
                            duration: 200
                        }
                    }

                    initialItem: RoundButton {
                        id: addNewButton
                        height: 30
                        text: "Add new"
                        icon.source: "qrc:/Resources/plus-lg.svg"
                        icon.height: 16
                        icon.width: 16
                        icon.color: Style.text
                        font.pixelSize: 13
                        palette.button: Style.secondary
                        palette.buttonText: Style.text
                        leftPadding: 20
                        rightPadding: 20
                        onPressed: backend.displayNewCounterMessageBox()
                    }

                    Button {
                        id: mainPageButton
                        visible: false
                        width: 65
                        height: 40
                        icon.source: "qrc:/Resources/bullseye.svg"
                        icon.color: checked ? Style.secondaryLight : Style.text
                        bottomPadding: 16
                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                        }
                        palette.buttonText: Style.text
                        Text {
                            id: mainPageButtonLabel
                            text: "Main"
                            color: Style.text
                            font.pixelSize: 13
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        onPressed: () =>
                                   {
                                       statsButton.checked = false
                                       settingsButton.checked = false
                                       swipeView.setCurrentIndex(1)
                                   }
                    }
                }

                Button {
                    id: statsButton
                    width: 65
                    height: 40
                    icon.source: "qrc:/Resources/bar-chart-line-fill.svg"
                    icon.color: checked ? Style.secondaryLight : Style.text
                    checkable: true
                    autoExclusive: true
                    bottomPadding: 16
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                    palette.buttonText: Style.text
                    onCheckedChanged: () =>
                                      {
                                          let clr = checked ? Style.secondaryLight : Style.text
                                          dataLabel.color = clr
                                          if (checked)
                                          {
                                              swipeView.setCurrentIndex(2)
                                              statsPage.initName = ""
                                          }
                                      }

                    Text {
                        id: dataLabel
                        text: "Stats"
                        color: Style.text
                        font.pixelSize: 13
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

            }
        }
    }



}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
