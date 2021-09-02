import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    width: 150
    height: 30
    property var listModel: ListModel {
        id: model
    }

    function setIndex(index)
    {
        comboBox.currentIndex = index
    }

    function getIndex()
    {
        return comboBox.currentIndex
    }

    signal indexChanged

    ComboBox {
        id: comboBox
        width: parent.width
        height: parent.height

        model: listModel

        delegate: ItemDelegate {
            width: comboBox.width
            height: comboBox.height
            contentItem: Text {
                text: displayName
                color: Style.text
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.highlighted ? Style.secondary : "transparent"
                radius: 10
            }

            highlighted: comboBox.highlightedIndex === index
        }

        contentItem: Text {
            leftPadding: 10
            rightPadding: comboBox.indicator.width + comboBox.spacing

            text: model.count > 0 ? comboBox.displayText : "-"
            font: comboBox.font
            color: comboBox.pressed ? Style.secondaryLight : Style.text
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 120
            implicitHeight: 40
            color: Style.primaryDark
            border.color: comboBox.pressed ? Style.secondaryLight : Style.primaryLight
            border.width: comboBox.visualFocus ? 2 : 1
            radius: 10
        }

        popup: Popup {
            y: comboBox.height - 1
            width: comboBox.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
                currentIndex: comboBox.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                color: Style.primaryDark
                border.color: Style.primaryLight
                radius: 10
            }
        }

        Component.onCompleted: () =>
                               {
                                   //if (listModel.count > 0) comboBox.currentIndex = 0
                               }

        onCurrentIndexChanged: () =>
                               {
                                   displayText = listModel.count > 0 ? displayText = listModel.get(currentIndex).displayName : "-"
                                   if (listModel.count === 0) return
                                   indexChanged()
                               }
    }
}
