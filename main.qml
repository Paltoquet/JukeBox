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
    width: 640
    height: 480
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

    GridLayout {
        id: grid
        columns: 3
        anchors.top: folderButton.bottom
        anchors.left: folderButton.left
        anchors.right: folderButton.right
        anchors.topMargin: 15
        opacity: 0.5

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

                Button {
                    id: soundButton
                    anchors.fill: parent
                    text: modelData.name
                    checkable: true
                    onClicked: {
                        select();
                    }
                    checked: piste.playbackState === MediaPlayer.PlayingState
                }

                Audio {
                    id: piste
                }
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
