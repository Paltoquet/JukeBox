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
        folder: "file:///D:/sound"
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

            UI.AudioClip {
                id: soundButton
                width: 120
                height: 45
                soundName: modelData.name
                soundPath: modelData.path
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
