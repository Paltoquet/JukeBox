import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.0
import QtMultimedia 5.8

import JukeBox 1.0
import "./UI" as UI

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("JukeBox")

    JukeBoxViewModel{
        id: jukeBox
        folder: "D:/sound"
    }

    Image {
        source: "pirate.jpg"
        anchors.fill: parent
    }

    Button{
        id: folderButton
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 80
        anchors.rightMargin: 80
        anchors.topMargin: 18
        opacity: 0.7
        text: "folder"
        visible: true
        onClicked: {
            fileDialog.open();
        }
    }

    /*UI.AudioClip{
        value: "coucou"
    }*/

    /*UI.JukeBoxButton {
        text: "pendejo"
    }*/

    GridLayout {
        id: grid
        columns: 3
        anchors.top: folderButton.bottom
        anchors.left: folderButton.left
        anchors.right: folderButton.right
        anchors.topMargin: 15

        rowSpacing: 8

        Repeater {
            model: jukeBox.sounds

            Item {
                width: 120
                height: 30

                function select() {
                    piste.source = modelData.path;
                    if(piste.playbackState === MediaPlayer.PlayingState) {
                        piste.stop();
                    } else {
                        piste.play();
                    }
                }

                UI.JukeBoxButton {
                    id: soundButton
                    anchors.fill: parent
                    checkable: true

                    soundName: modelData.name
                    soundPath: modelData.path
                    onClicked: {
                        select();
                    }
                    checked: piste.playbackState === MediaPlayer.PlayingState
                }

                Rectangle {
                    id: dragableObject

                    property var soundButton: soundButton

                    anchors.fill: parent
                    Drag.active: dragArea.drag.active
                    Drag.dragType: Drag.Automatic
                    visible: dragArea.drag.active
                    color: "transparent"
                }

                MouseArea {
                    id: dragArea
                    anchors.fill: dragableObject
                    drag.target: dragableObject

                    onClicked: soundButton.clicked()
                    onPressed: parent.grabToImage(function(result) {
                        dragObject.Drag.imageSource = result.url
                    })
                }

                Audio {
                    id: piste
                }
            }
        }
    }

    Rectangle {
        id: shortCutSeparator
        width: folderButton.width
        height: 7
        anchors.left: folderButton.left
        anchors.right: folderButton.right
        anchors.top: grid.bottom
        anchors.topMargin: 15
        color: "#dddddd"
        opacity: 0.7
    }

    Row {

        anchors.top: shortCutSeparator.bottom
        anchors.left: folderButton.left
        anchors.right: folderButton.right
        anchors.topMargin: 15
        spacing: 15

        Repeater {
            model: ["a", "z" , "e", "r", "t", "y", "u"]

            UI.ShortCutArea {
                shortCutId: index
                shortCutSequence: modelData
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a folder for the sounds"
        selectFolder: true

        onAccepted: {
            jukeBox.folder = fileDialog.folder;
        }
    }
}
