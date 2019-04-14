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
    property bool isWaiting: piste.isWaiting

    property bool isRepeating: repeatButton.checked
    property bool isRandomizing: randomButton.checked
    property real volume: 100

    width: 140
    height: 45

    function pause() {
        pauseAnimation.start();
        //piste.pause();
    }

    function play() {
        piste.play();
    }

    AudioClipViewModel {
        id: piste
        volume: root.volume
        source: root.soundPath
        isRepeating: root.isRepeating
        isRandomizing: root.isRandomizing
    }

    JukeBoxButton {
        id: playButton
        soundName: root.soundName
        soundPath: root.soundPath
        checked: root.isPlaying || root.isWaiting

        width: parent.width
        height: 22
    }

    ButtonGroup {
        id: optionButtonGroup
        exclusive: false
    }

    Row {
        id: optionMenus

        anchors.top: playButton.bottom
        anchors.left: playButton.left
        anchors.right: playButton.right

        JukeBoxButton {
            id: repeatButton
            width: parent.width/6
            height: volumeSlider.height
            hoverEnabled: true
            hoverRequested: playButton.hovered && !playButton.checked
            ToolTip.text: "repeat"

            ButtonGroup.group: optionButtonGroup
            checkable: true
            font.family: "jukebox"
            soundName: "\uE800"

            onClicked: {
               if(checked && randomButton.checked){
                   randomButton.checked = false;
               }
            }
        }

        JukeBoxButton {
            id: randomButton
            width: parent.width/6
            height: volumeSlider.height
            hoverEnabled: true
            hoverRequested: playButton.hovered && !playButton.checked
            ToolTip.text: "random"

            ButtonGroup.group: optionButtonGroup
            checkable: true
            font.family: "jukebox"
            soundName: "\uE80b"

            onClicked: {
                if(checked && repeatButton.checked){
                    repeatButton.checked = false;
                }
            }
        }

        Rectangle {
            width: childrenRect.width
            height: childrenRect.height
            color: playButton.color
            opacity: 0.7

            UI.JukeBoxSlider{
                id: volumeSlider
                width: optionMenus.width - repeatButton.width - randomButton.width
                height: 23
                value: 50
                from: 0
                to: 100
                onValueChanged: {
                    root.volume = value;
                }
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
            var shouldPlay = !playButton.checked;
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

    NumberAnimation {
        id: pauseAnimation

        property int startedVolume: 100

        target: root
        properties: "volume"
        duration: 750
        from: piste.volume
        to: 0
        onStarted: {
            startedVolume = piste.volume;
        }

        onRunningChanged: {
            if(!running){
                piste.pause();
                root.volume = startedVolume;
            }
        }
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
