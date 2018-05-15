import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2

Item {
    width: 126.5
    height: 38.25

    DropShadow {
        anchors.fill: bg
        source: bg
        color: "#9DA3DC"
        opacity: 0.8
        verticalOffset: 1.5
        radius: 3
    }

    Rectangle {
        id: bg
        radius: 7.5
        color: "#C2CFFE"
        anchors.fill: parent
    }

    SvgImage {
        anchors.left: parent.left
        id: left
        width: 54
        height: parent.height
        source: "images/tonality_controller_left.svg"

        Text {
            anchors.centerIn: parent
            font.family: "LatoBold"
            font.bold: true
            text: "C# maj"
            color: "#615f97"
            font.pointSize: 10.0
            font.letterSpacing: 0
        }
    }

    Rectangle {
        anchors.left: left.right
        anchors.leftMargin: 1
        width: 51.75
        height: parent.height

        Text {
            id: tonality

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: tonalityLabel.top

            text: "0"
            font.pointSize: 15
            color: "#615f97"
            font.bold: true
            font.family: "LatoBold"
            anchors.bottomMargin: 1

            onTextChanged: {
                if (parseInt(text) > 0 && text[0] !== "+") {
                    tonality.text = "+" + text;
                }
            }
        }

        Label {
            id: tonalityLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4.5

            text: "TONALITY"
            color: "#8790d0"
            font.family: "LatoRegular"
            font.pointSize: 7.5
        }
    }

    ImageButton {
        width: 18.75
        height: 18.75
        anchors.right: parent.right
        anchors.top: parent.top
        idleImage: "images/tonality_up.svg"
        hoverImage: "images/tonality_up_pressed.svg"

        onClicked: {
            tonality.text = (parseInt(tonality.text) + 1).toString()
        }
    }

    ImageButton {
        width: 18.75
        height: 18.75
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        idleImage: "images/tonality_down.svg"
        hoverImage: "images/tonality_down_pressed.svg"

        onClicked: {
            tonality.text = (parseInt(tonality.text) - 1).toString()
        }
    }
}
