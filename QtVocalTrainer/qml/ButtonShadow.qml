import QtQuick 2.0
import QtGraphicalEffects 1.0

DropShadow {
    property var target

    anchors.fill: target

    horizontalOffset: 0
    verticalOffset: 2
    radius: 3.0
    color: "#aa9da3dc"
    source: target
}
