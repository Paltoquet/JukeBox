import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

import JukeBox 1.0
import "../UI" as UI

Item {

    id: root

    property string soundName: ""
    property url soundPath: ""

    property bool isPlaying: piste.isPlaying

    property bool isRepeating: repeatButton.checked
    property real volume: volumeSlider.value

    width: 120
    height: 45

    function pause() {
        piste.pause();
    }

    function play() {
        piste.play();
    }

    JukeBoxButton {
        id: playButton
        soundName: root.soundName
        soundPath: root.soundPath
        checked: root.isPlaying

        width: parent.width
        height: 22
    }

    Row {
        id: optionMenus

        anchors.top: playButton.bottom
        anchors.left: playButton.left
        anchors.right: playButton.right

        JukeBoxButton {
            id: repeatButton
            width: parent.width/4
            height: volumeSlider.height
            checkable: true
            font.family: "jukebox"
            soundName: "\uE800"
        }

        Rectangle {
            width: childrenRect.width
            height: childrenRect.height
            color: root.isPlaying ? "#999999" : "#dddddd"
            opacity: 0.7

            UI.JukeBoxSlider{
                id: volumeSlider
                width: optionMenus.width - repeatButton.width
                height: 23
                value: 50
                from: 0
                to: 100
            }
        }
    }

    Rectangle {
        id: dragableObject

        property var audioClip: root

        anchors.fill: playButton
        Drag.active: dragArea.drag.active
        Drag.dragType: Drag.Automatic
        visible: dragArea.drag.active
        color: "transparent"
    }

    MouseArea {
        id: dragArea
        anchors.fill: dragableObject
        drag.target: dragableObject
        propagateComposedEvents: true

        onClicked: {
            var shouldPlay = !root.isPlaying;
            if(shouldPlay) {
                root.play();
            }else {
                root.pause();
            }
        }
        onPressed: {
            parent.grabToImage(function(result) {
                dragObject.Drag.imageSource = result.url
            })
        }
    }

    AudioClipViewModel {
        id: piste
        volume: root.volume
        source: root.soundPath
        isRepeating: root.isRepeating
    }

    /*MediaPlayer {
        id: piste
        loops: root.isRepeating ? Audio.Infinite : 1
        volume: root.volume
        source: root.soundPath
        onStatusChanged: {
            //console.log("status " + status);
        }
        onErrorChanged: {
            //console.log("error " + errorString);
            //console.log(error);
        }
        onErrorStringChanged: {
            //console.log("error name " + errorString)
        }
        onAvailabilityChanged: {
            //console.log("availability " + availability)
        }
        onBufferProgressChanged: {
            //console.log("buffer " + bufferProgress)
        }
    }*/

}
