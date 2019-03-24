import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item {

    id: root

    property string value: ""
    property bool isPlaying: false

    width: 120

    Rectangle {
        id: clipName
        color: "#c7c7c7"
        width: parent.width
        height: 40

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: root.value
            font.pointSize: 15
        }
    }

    Row {
        id: playPause

        anchors.top: clipName.bottom
        anchors.left: clipName.left
        anchors.right: clipName.right

        Button {
            width: parent.width/2
            checkable: true
            id: playButton
            font.family: "jukebox"
            text: "\uE801"
        }

        Button {
            width: parent.width/2
            checkable: true
            id: pauseButton
            font.family: "jukebox"
            text: "\uE802"
        }
    }

    Row {
        id: optionMenu

        anchors.top: playPause.bottom
        anchors.left: playPause.left
        anchors.right: playPause.right

        Button {
            id: muteButton
            checkable: true
            width: parent.width/3
            font.family: "jukebox"
            text: "\uE808"
        }

        Button {
            id: lowVolumeButton
            checkable: true
            width: parent.width/3
            font.family: "jukebox"
            text: "\uE806"
        }

        Button {
            id: highVolumeButton
            checkable: true
            width: parent.width/3
            font.family: "jukebox"
            text: "\uE807"
        }
    }
}
