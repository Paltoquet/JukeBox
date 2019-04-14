import QtQuick 2.12
import QtQuick.Controls 2.2


Button {
    width: 120
    height: 25
    font.pixelSize: 12
    checkable: true
    opacity: 0.7

    hoverEnabled: true
    ToolTip.visible: hovered && ToolTip.text !== ""
    ToolTip.delay: 720

    property string soundName: ""
    property string soundPath: ""
    property color color: backGround.color
    property bool hoverRequested: false

    text: soundName

    background: Rectangle {
        id: backGround
        implicitWidth: 120
        implicitHeight: 40
        color: {
            if(checked) {
                return "#999999";
            } else if(hovered || hoverRequested) {
                return "#ebebeb";
            } else {
                return "#d8d8d8";
            }
        }
    }
}
