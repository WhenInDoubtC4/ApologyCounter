import QtQuick 2.12
import QtQuick.Controls 2.12

import settings 1.0
import counterManager 1.0

Item {
    readonly property int settingRectHeight: 60
    property var counters: ({})

    function getNameIndex(name)
    {
        for (var i = 0; i < counter1ComboBox.listModel.count; i++)
        {
            if (counter1ComboBox.listModel.get(i).name === name) return i
        }
        return -1
    }

    function update()
    {
        counter1ComboBox.listModel.clear()
        counter2ComboBox.listModel.clear()
        for (var counter of counters)
        {
            counter1ComboBox.listModel.append({displayName: counter.displayName, name: counter.name})
            counter2ComboBox.listModel.append({displayName: counter.displayName, name: counter.name})
        }

        var settings = backend.getWidgetSettings()
        const index1 = getNameIndex(settings[0])
        const index2 = getNameIndex(settings[1])
        console.log("Settings: " + settings)
        if (index1 !== -1) counter1ComboBox.setIndex(index1)
        else counter1ComboBox.setIndex(0)
        if (index2 !== -1) counter2ComboBox.setIndex(index2)
        else counter2ComboBox.setIndex(0)
    }

    SettingsBacked {
        id: backend
    }

    Column {
        width: parent.width
        spacing: 2

        Rectangle {
            color: Style.primaryDark
            width: parent.width
            height: settingRectHeight

            Text {
                id: widgetCounter1Text
                text: "Widget counter #1"
                font.pixelSize: 13
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                color: Style.text
            }

            CounterComboBox {
                id: counter1ComboBox
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10

                onIndexChanged: {
                    backend.setWidgetName(0, listModel.get(getIndex()).name)
                }
            }
        }

        Rectangle {
            id: rectangle
            color: Style.primaryDark
            width: parent.width
            height: settingRectHeight

            Text {
                id: widgetCounter2Text
                text: "Widget counter #2"
                font.pixelSize: 13
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                color: Style.text
            }

            CounterComboBox {
                id: counter2ComboBox
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10

                onIndexChanged: {
                    backend.setWidgetName(1, listModel.get(getIndex()).name)
                }
            }
        }
    }

    Component.onCompleted: () =>
                           {
                               update()
                           }
}
