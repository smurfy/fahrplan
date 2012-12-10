import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

TumblerDialog {
    id: root

    /*
     * Property: year
     * [int] The displayed year.
     */
    property int year: dateTime.currentYear()

    /*
     * Property: month
     * [int] The displayed month.
     */
    property int month: 1

    /*
     * Property: day
     * [int] The displayed day.
     */
    property int day: 1

    /*
     * Property: minimumYear
     * [int] Optional, the minimum year shown on the tumbler. This property should
     *       only be set once during construction. If the value is not specified,
     *       it is default to current year - 1.
     */
    property int minimumYear: dateTime.currentYear() - 1

    /*
     * Property: maximumYear
     * [int] Optional, the maximum year shown on the tumbler. This property should
     *       only be set once during construction. If the value is not specified,
     *       it is default to current year + 20.
     */
    property int maximumYear: dateTime.currentYear() + 20

    /*
     * Property: hour
     * [int] The displayed hour (in 24h format).
     */
    property int hour: 0

    /*
     * Property: minute
     * [int] The displayed minute.
     */
    property int minute: 0

    property alias nowButtonText: nowButton.text

    titleText: "Select date and time"

    content: Item {
        height: 300
        width: parent.width
        Tumbler {
            id: tumbler
            height: 300
            privateDelayInit: true

            columns: [ dayColumn, monthColumn, yearColumn, hourColumn, minuteColumn ]
        }
    }

    TumblerColumn {
        id: dayColumn
        label: "DAY"
        items: ListModel {
            id: dayList
        }
        selectedIndex: root.day - (root.day > 0 ?  1 : 0)
    }

    TumblerColumn {
        id: monthColumn
        label: "MONTH"
        items: ListModel {
            id: monthList
        }
        selectedIndex: root.month - (root.month > 0 ?  1 : 0)
    }

    TumblerColumn {
        id: yearColumn
        label: "YEAR"
        items: ListModel {
            id: yearList
        }
        selectedIndex: yearList.length > 0 ? internal.year - yearList.get(0).value : 0
    }

    TumblerColumn {
        id: hourColumn
        items: ListModel {
            id: hourList
        }
        label: "HR"
        selectedIndex: root.hour
    }

    TumblerColumn {
        id: minuteColumn
        items: ListModel {
            id: minuteList
        }
        label: "MIN"
        selectedIndex: root.minute
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            if (!internal.initialized)
                internal.initializeDataModels();
        }
    }

    onAccepted: {
        root.year = yearColumn.selectedIndex + yearList.get(0).value;
        root.month = monthColumn.selectedIndex + 1;
        root.day = dayColumn.selectedIndex + 1;
        root.hour = hourColumn.selectedIndex;
        root.minute = minuteColumn.selectedIndex;
    }

    onHourChanged: {
        internal.validateTime();
        hourColumn.selectedIndex = root.hour
    }

    onMinuteChanged: {
        internal.validateTime();
        minuteColumn.selectedIndex = root.minute
    }

    onDayChanged: {
        internal.validateDate();
        if (dayColumn.items.length > root.day - 1)
            dayColumn.selectedIndex = root.day - 1
    }
    onMonthChanged: {
        internal.validateDate();
        monthColumn.selectedIndex = root.month - 1
    }
    onYearChanged: {
        internal.year = root.year
        internal.validateDate();
        internal.year = internal.year < internal.minYear ? internal.minYear :
                              (internal.year > root.maximumYear ? root.maximumYear : internal.year)

        if (internal.initialized)
            yearColumn.selectedIndex = internal.year - yearList.get(0).value
    }

    buttons: Row {
           id: nowButtonContainer

           Button {
               id: nowButton
               text: "Now"
               width: root.width - 3
               style: ButtonStyle { inverted: true }
               onClicked: {
                   internal.setToday();
                   accept();
               }
           }
       }

    QtObject {
        id: internal

        property variant initialized: false
        property int year
        property int minYear

        function initializeDataModels() {
            var currentYear = new Date().getFullYear();
            minimumYear = minimumYear ? minimumYear : currentYear - 1;
            maximumYear = maximumYear ? maximumYear : currentYear + 20;

            for (var y = minimumYear; y <= maximumYear; ++y)
                yearList.append({"value" : y}) // year

            var nDays = dateTime.daysInMonth(internal.year, root.month);
            for (var d = 1; d <= nDays; ++d)
                dayList.append({"value" : d})  // day

            for (var m = 1; m <= 12; ++m)
                monthList.append({"value" : dateTime.shortMonthName(m)});

            for (var i = 0; i < 24; ++i)
                hourList.append({"value" : (i < 10 ? "0" : "") + i});

            for (var i = 0; i < 60; ++i)
                minuteList.append({"value" : (i < 10 ? "0" : "") + i });

            nowButtonContainer.anchors.top = buttons[0].bottom;
            nowButtonContainer.anchors.topMargin = 20

            tumbler.privateInitialize();
            internal.initialized = true;
        }

        function setToday() {
            var d = new Date();
            yearColumn.selectedIndex = d.getFullYear() - yearList.get(0).value;
            monthColumn.selectedIndex = d.getMonth();
            dayColumn.selectedIndex = d.getDate() - 1;
            hourColumn.selectedIndex = d.getHours();
            minuteColumn.selectedIndex = d.getMinutes();
        }

        function validateDate() {
            if (internal.year < 1){
                internal.year = new Date().getFullYear()
                if (maximumYear < internal.year)
                    root.maximumYear = dateTime.currentYear() + 20;
                if (minimumYear > internal.year)
                    internal.minYear = dateTime.currentYear() - 1;
            }

            root.month = Math.max(1, Math.min(12, root.month))
            var d = dateTime.daysInMonth(internal.year, root.month);
            root.day = Math.max(1, Math.min(d, root.day))
        }

        function validateTime() {
            root.hour = Math.max(0, Math.min(23, root.hour))
            root.minute = Math.max(0, Math.min(59, root.minute))
        }
    }
}

