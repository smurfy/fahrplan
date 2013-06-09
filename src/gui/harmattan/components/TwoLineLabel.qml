import QtQuick 1.1
import com.nokia.meego 1.1

Item {
    id: root

    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property bool platformInverted: false

    implicitWidth: Math.max(lblTitle.implicitWidth, lblSubtitle.implicitWidth)
    height: childrenRect.height

    Label {
        id: lblTitle
        font: UiConstants.TitleFont
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }
    Label {
        id: lblSubtitle
        font: UiConstants.SubtitleFont
        visible: text != ""
        height: visible ? implicitHeight : 0
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        anchors {
            top: lblTitle.bottom
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            right: parent.right
        }
    }
}
