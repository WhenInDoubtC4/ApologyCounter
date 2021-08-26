import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.2

import style 1.0
import counter 1.0

 Item {
    CounterBackend {
       id: backend

       Component.onCompleted: {
            backend.setName(name)
        }
    }
    property CounterBackend counterBackend: backend
    property string displayName: "NAME"
    property string name: "name"
    readonly property int buttonRectWidth: 60
    readonly property int countRectWidth: 100

    signal moveUp()
    signal moveDown()

    function requestDeleteCounter()
    {
        backend.displayDeleteCounterMessageBox(displayName, name)
    }

    id: root
    width: 600
    height: 80

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
                    text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:13pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Do you want to delete the counter for <span style=\" font-weight:600;\">%1</span>? This will delete all the stats and the data!</p></body></html>".arg(displayName)
                    color: Style.text
                    font.pixelSize: 13
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    bottomPadding: 6
                    topPadding: dialog.globalPadding
                    rightPadding: dialog.globalPadding
                    leftPadding: dialog.globalPadding
                    width: 300
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
                        id: deleteButton
                        text: "Delete"
                        height: 30
                        width: 80
                        font.pixelSize: 13
                        font.bold: true
                        palette.button: Style.secondary
                        palette.buttonText: Style.destructive
                        onPressed: () =>
                                   {
                                       backend.deleteCounter()
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
            function onDisplayDeleteCounterDialog()
            {
                dialog.open()
            }
        }
    }

    Rectangle {
        id: textRect
        width: parent.width - countRectWidth - buttonRectWidth
        height: parent.height
        color: Style.primaryDark

        RoundButton {
            id: menuButton
            width: 30
            height: 30
            radius: 15
            text: "..."
            palette {
                button: "transparent"
                buttonText: Style.text
            }

            anchors.verticalCenter: parent.verticalCenter
            bottomPadding: 15
            onClicked: () =>
                       {
                           menu.open()
                       }

            Menu {
                id: menu
                y: menuButton.height
                background: Rectangle {
                    id: menuBackgroundRect
                    implicitWidth: 150
                    //implicitHeight: 200
                    color: Style.primaryDark
                    radius: 10
                    border.width: 1
                    border.color: Style.primaryLight
                }
                MenuItem {
                    id: decrementMenuItem
                    property bool isPressed: false
                    contentItem: Text {
                        Image {
                            anchors.right: parent.right
                            source: "qrc:/Resources/dash-circle-fill.svg"
                            anchors.rightMargin: 6
                            sourceSize.width: 16
                            sourceSize.height: 16
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }
                        text: "Decrement"
                        color: Style.text
                    }
                    background: Rectangle {
                        color: decrementMenuItem.isPressed || decrementMenuItem.highlighted ? Style.secondary : "transparent"
                        radius: 10
                    }
                    onPressedChanged: () =>
                                      {
                                          decrementMenuItem.isPressed = !decrementMenuItem.isPressed
                                      }
                    onPressed: backend.decrementCount(true)
                }
                MenuItem {
                    id: showStatsMenuItem
                    property bool isPressed: false
                    contentItem: Text {
                        Image {
                            anchors.right: parent.right
                            source: "qrc:/Resources/bar-chart-line-fill.svg"
                            anchors.rightMargin: 6
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }
                        text: "Show stats"
                        color: Style.text
                    }
                    background: Rectangle {
                        color: showStatsMenuItem.isPressed || showStatsMenuItem.highlighted ? Style.secondary : "transparent"
                        radius: 10
                    }
                    onPressedChanged: () =>
                                      {
                                          showStatsMenuItem.isPressed = !showStatsMenuItem.isPressed
                                      }
                }
                MenuItem {
                    id: moveUpMenuItem
                    property bool isPressed: false
                    contentItem: Text {
                        Image {
                            anchors.right: parent.right
                            source: "qrc:/Resources/chevron-up.svg"
                            anchors.rightMargin: 6
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }
                        text: "Move up"
                        color: Style.text
                    }
                    background: Rectangle {
                        color: moveUpMenuItem.isPressed || moveUpMenuItem.highlighted ? Style.secondary : "transparent"
                        radius: 10
                    }
                    onPressedChanged: () =>
                                      {
                                          moveUpMenuItem.isPressed = !moveUpMenuItem.isPressed
                                      }
                    onPressed: () =>
                               {
                                   menu.close()
                                   root.moveUp()
                               }
                }
                MenuItem {
                    id: moveDownMenuItem
                    property bool isPressed: false
                    contentItem: Text {
                        Image {
                            anchors.right: parent.right
                            source: "qrc:/Resources/chevron-down.svg"
                            anchors.rightMargin: 6
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.text
                            }
                        }
                        text: "Move down"
                        color: Style.text
                    }
                    background: Rectangle {
                        color: moveDownMenuItem.isPressed || moveDownMenuItem.highlighted ? Style.secondary : "transparent"
                        radius: 10
                    }
                    onPressedChanged: () =>
                                      {
                                          moveDownMenuItem.isPressed = !moveDownMenuItem.isPressed
                                      }
                    onPressed: () =>
                               {
                                   menu.close()
                                   root.moveDown()
                               }
                }
                MenuItem {
                    id: deleteMenuItem
                    property bool isPressed: false
                    contentItem: Text {
                        Image {
                            anchors.right: parent.right
                            source: "qrc:/Resources/trash-fill.svg"
                            anchors.rightMargin: 6
                            ColorOverlay {
                                anchors.fill: parent
                                source: parent
                                color: Style.destructive
                            }
                        }
                        text: "Delete"
                        color: Style.destructive
                        font.bold: true
                    }
                    background: Rectangle {
                        color: deleteMenuItem.isPressed || deleteMenuItem.highlighted ? Style.secondary : "transparent"
                        radius: 10
                    }
                    onPressedChanged: () =>
                                      {
                                          deleteMenuItem.isPressed = !deleteMenuItem.isPressed
                                      }
                    onPressed: () =>
                               {
                                   requestDeleteCounter()
                                   menu.close()
                               }
                }
            }
        }

        Flow {
            id: textFlow
            x: menuButton.width
            width: parent.width - menuButton.width
            anchors.verticalCenter: parent.verticalCenter
            //height: parent.height
            spacing: fnt.advanceWidth(' ')

            FontMetrics {
                id: fnt
                font.pixelSize: 13
            }

            readonly property string lasText: "apologized when there is nothing to apologize for"

            Text {
                id: timesLabel
                text: "Times"
                font: fnt.font
                topPadding: 2
                color: Style.text
            }

            Rectangle {
                id: nameLabelRect
                width: nameLabel.width + border.width
                height: nameLabel.height + border.width
                radius: 4
                border.width: 4
                border.color: Style.secondary
                color: Style.secondary

                Text {
                    id: nameLabel
                    text: displayName
                    color: Style.text
                    rightPadding: 2
                    leftPadding: 2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 13
                    font.bold: true
                }
            }

            Repeater {
                id: textFlowRepeater
                model: textFlow.lasText.split(' ')
                delegate: Text {
                    text: modelData
                    font: fnt.font
                    color: Style.text
                    topPadding: 2
                }
            }


        }

    }

    Rectangle {
        id: countRect
        x: textRect.width
        width: countRectWidth
        height: parent.height
        color: Style.primaryDark

        FontLoader {
            id: robotoMono
            source: "qrc:/Resources/RobotoMono.ttf"
        }

        Text {
            id: countLabel
            width: parent.width
            height: parent.height
            rightPadding: 6
            color: Style.secondary
            text: backend.count
            font.pixelSize: 24
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.family: robotoMono.name


        }

        Glow {
            id: countLabelGlow
            anchors.fill: countLabel
            source: countLabel
            color: Style.secondaryLight
            spread: 0.15
            radius: 10
            samples: 16
        }
    }

    Rectangle {
        id: buttonRect
        x: textRect.width + countRect.width
        width: buttonRectWidth
        height: parent.height
        color: Style.primaryDark

        RoundButton {
            id: incrementButton
            icon.source: "qrc:/Resources/plus-lg.svg"
            icon.width: 16
            icon.height: 16
            icon.color: Style.text
            display: AbstractButton.IconOnly
            palette {
                button: Style.secondaryDark
                buttonText: Style.text
            }

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onPressAndHold: backend.decrementCount()
            onPressed: backend.incrementCount()
        }

    }
}
