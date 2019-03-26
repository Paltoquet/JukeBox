import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    width: 120
    height: 25
    font.pixelSize: 10
    checkable: true
    opacity: 0.7

    property string soundName: ""
    property string soundPath: ""

    text: soundName

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        color: checked ? "#999999" : "#dddddd"
    }
}
