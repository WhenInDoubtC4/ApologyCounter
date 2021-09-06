import QtQuick 2.12
import QtQuick.Controls 2.12

import styleBackend 1.0

Item {
    id: root
    width: 150
    height: 30
    property var listModel: ListModel {
        id: model
    }

    function setIndex(index) {
        comboBox.currentIndex = index
    }

    function getIndex() {
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
                color: parent.highlighted ? StyleBackend.getSecondaryColor() : "transparent"
                radius: 10
            }

            highlighted: comboBox.highlightedIndex === index
        }

        contentItem: Text {
            id: label
            leftPadding: 10
            rightPadding: comboBox.indicator.width + comboBox.spacing

            text: model.count > 0 ? comboBox.displayText : "-"
            font: comboBox.font
            color: comboBox.pressed ? StyleBackend.getSecondaryLightColor() : Style.text
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 120
            implicitHeight: 40
            color: Style.primaryDark
            border.color: comboBox.pressed ? StyleBackend.getSecondaryLightColor() : Style.primaryLight
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

        onCurrentIndexChanged: {
            displayText = listModel.count > 0 ? displayText = listModel.get(currentIndex).displayName : "-"
            label.text = displayText
            if (listModel.count === 0) return
            indexChanged()
        }
    }
}
