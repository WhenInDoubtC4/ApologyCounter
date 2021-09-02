import QtQuick 2.12
import QtQuick.Controls 2.12
import QtCharts 2.3
import QtGraphicalEffects 1.0

import stats 1.0

Item {
    property string initName: ""
    property var counters: ({})

    function update()
    {
        comboBox.listModel.clear()
        for (const counter of counters)
        {
            comboBox.listModel.append({name: counter.name, displayName: counter.displayName})
        }

        if (initName === "")
        {
            comboBox.setIndex(0)
        }
        else
        {
            let index = -1
            for (let i = 0; i < comboBox.listModel.count; i++)
            {
                if (comboBox.listModel.get(i).name === initName)
                {
                    index = i
                    break
                }
            }
            const currentIndex = index !== -1 ? index : 0
            comboBox.setIndex(currentIndex)
        }
    }

    function updateRanges()
    {
        dataPointModel.clear()
        for (const rangePoint of backend.getRangePoints())
        {
            dataPointModel.append({start: rangePoint.start, end: rangePoint.end})
        }
        pointSelector.currentIndex = 0
        backend.setRangePointIndex(0)
        rangeText.text = dataPointModel.count > 0 ? dataPointModel.get(0).start + "\n" + dataPointModel.get(0).end : "-\n-"
        nextPointButton.enabled = false
        prevPointButton.enabled = dataPointModel.count > 1
    }

    StatsBackend {
        id: backend
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: column.height
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        Column {
            id: column
            width: parent.width

            Row {
                id: row
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                topPadding: 5
                Text {
                    text: "Showing stats for"
                    anchors.verticalCenter: parent.verticalCenter
                    color: Style.text
                }

                CounterComboBox {
                    id: comboBox
                    anchors.verticalCenter: parent.verticalCenter

                    onIndexChanged: () =>
                                    {
                                        backend.name = comboBox.listModel.get(comboBox.currentIndex).name
                                        backend.updateChart(chart)

                                        weekButton.enabled = backend.isRangeAllowed(StatsBackend.CHART_RANGE_WEEK)
                                        monthButton.enabled = backend.isRangeAllowed(StatsBackend.CHART_RANGE_MONTH)
                                        yearButton.enabled = backend.isRangeAllowed(StatsBackend.CHART_RANGE_YEAR)
                                        updateRanges()
                                    }
                }
            }

            TabBar {
                topPadding: 7
                contentHeight: 30
                anchors.horizontalCenter: parent.horizontalCenter
                background: Rectangle {
                    color: "transparent"
                }
                TabButton {
                    id: weekButton
                    text: "W"
                    width: 60
                    contentItem: Text {
                        anchors.centerIn: parent
                        text: parent.text
                        color: Style.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.enabled ? (parent.checked ? Style.secondary : Style.primaryDark) : Style.primaryLight
                        radius: 10
                    }
                    enabled: backend.isRangeAllowed(StatsBackend.CHART_RANGE_WEEK)
                    onPressed: () =>
                               {
                                   backend.rangeSelected = StatsBackend.CHART_RANGE_WEEK
                                   updateRanges()
                                   backend.updateChart(chart)
                               }
                }
                TabButton {
                    id: monthButton
                    text: "M"
                    width: 60
                    contentItem: Text {
                        anchors.centerIn: parent
                        text: parent.text
                        color: Style.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.enabled ? (parent.checked ? Style.secondary : Style.primaryDark) : Style.primaryLight
                        radius: 10
                    }
                    enabled: backend.isRangeAllowed(StatsBackend.CHART_RANGE_MONTH)
                    onPressed: () =>
                               {
                                   backend.rangeSelected = StatsBackend.CHART_RANGE_MONTH
                                   updateRanges()
                                   backend.updateChart(chart)
                               }
                }
                TabButton {
                    id: yearButton
                    text: "Y"
                    width: 60
                    contentItem: Text {
                        anchors.centerIn: parent
                        text: parent.text
                        color: Style.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.enabled ? (parent.checked ? Style.secondary : Style.primaryDark) : Style.primaryLight
                        radius: 10
                    }
                    enabled: backend.isRangeAllowed(StatsBackend.CHART_RANGE_YEAR)
                    onPressed: () =>
                               {
                                   backend.rangeSelected = StatsBackend.CHART_RANGE_YEAR
                                   updateRanges()
                                   backend.updateChart(chart)
                               }
                }
                bottomPadding: 10
            }

            Rectangle {
                id: pointSelector
                color: Style.primaryDark
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                height: 40
                radius: 10

                property int currentIndex: 0

                ListModel {
                    id: dataPointModel
                }


                Button {
                    id: prevPointButton
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    height: parent.height
                    width: 60
                    icon.source: "qrc:/Resources/chevron-left.svg"
                    icon.color: enabled ? Style.text : Style.primaryLight
                    background: Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: parent.pressed ? Style.primaryLight : Style.primaryDark
                    }
                    onPressed: () =>
                               {
                                   pointSelector.currentIndex += 1
                                   rangeText.text = dataPointModel.get(pointSelector.currentIndex).start + "\n" + dataPointModel.get(pointSelector.currentIndex).end
                                   prevPointButton.enabled = !(pointSelector.currentIndex === dataPointModel.count - 1)
                                   nextPointButton.enabled = !(pointSelector.currentIndex === 0)
                                   backend.setRangePointIndex(pointSelector.currentIndex)
                                   backend.updateChart(chart)
                               }
                }
                Row {
                    id: row1
                    anchors.centerIn: parent
                    spacing: 10
                    Text {
                        text: "From:\nTo:"
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        color: Style.text
                        font.pixelSize: 13
                    }

                    Text {
                        id: rangeText
                        text: "-\n-"
                        anchors.verticalCenter: parent.verticalCenter
                        color: Style.text
                        font.pixelSize: 13
                    }
                }
                Button {
                    id: nextPointButton
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    width: 60
                    icon.source: "qrc:/Resources/chevron-right.svg"
                    icon.color: enabled ? Style.text : Style.primaryLight
                    background: Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: parent.pressed ? Style.primaryLight : Style.primaryDark
                    }
                    onPressed: () =>
                               {
                                   pointSelector.currentIndex -= 1
                                   rangeText.text = dataPointModel.get(pointSelector.currentIndex).start + "\n" + dataPointModel.get(pointSelector.currentIndex).end
                                   prevPointButton.enabled = !(pointSelector.currentIndex === dataPointModel.count - 1)
                                   nextPointButton.enabled = !(pointSelector.currentIndex === 0)
                                   backend.setRangePointIndex(pointSelector.currentIndex)
                                   backend.updateChart(chart)
                               }
                }
            }

            ChartView {
                id: chart
                width: parent.width
                height: 400
                backgroundColor: Style.primaryDark

                antialiasing: true
                animationOptions: ChartView.SeriesAnimations

                Rectangle {
                    width: notEnoughDataText.width + 20
                    height: notEnoughDataText.height + 10
                    anchors.centerIn: parent
                    radius: 10
                    color: Style.primaryLight
                    visible: !weekButton.enabled
                    Text {
                        id: notEnoughDataText
                        text: "Not enough data"
                        color: Style.text
                        anchors.centerIn: parent
                    }
                }
            }

        }
    }
}
