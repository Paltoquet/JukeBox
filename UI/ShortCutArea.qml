import QtQuick 2.12
import QtQuick.Controls 2.4

Item {
    id: root

    width: 72
    height: 52
    opacity: 0.7

    property int shortCutId: -1
    property string shortCutSequence: ""
    property var audioClip: null
    property string soundName: ""
    property string soundPath: ""

    signal requestSoung(string soundName, string soundPath);

    Rectangle {
        anchors.fill: parent
        color: "#eeeeee"

        Text {
            id: shortCutIdText
            text: shortCutSequence
            font.pixelSize: 12
            anchors.top: parent.top
            anchors.topMargin: 6
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: soundName
            width: parent.width - 6
            font.pixelSize: 12

            clip: true
            horizontalAlignment: Text.AlignHCenter
            anchors.top: shortCutIdText.bottom
            anchors.topMargin: 6
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    DropArea {
        anchors.fill: parent
        onDropped: {
            var src = drag.source;
            root.audioClip = src.audioClip;
            root.soundName = audioClip.soundName;
            root.soundPath = audioClip.soundPath;
        }
    }

    Shortcut {
        sequence: root.shortCutSequence
        context: Qt.ApplicationShortcut
        onActivated: {
            if(audioClip){
                if(audioClip.isPlaying){
                    audioClip.pause();
                } else {
                    audioClip.play();
                }
            }
        }
    }
}
