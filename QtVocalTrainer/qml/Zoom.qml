import QtQuick 2.0

Item {
    readonly property real baseWidth: 1374.0
    readonly property real zoomFactor: 12.0 * 4
    property real zoom: 1.0
    property real minZoom: 1.0
    property real maxZoom: 2.0
    readonly property real verticalToHoriznotalGridIntervalRelation: 2.4117701323665077

    function getIntervalWidth() {
        var linesCount = Math.round(zoomFactor / zoom);
        var baseIntervalsCount = linesCount + 1;
        return baseWidth / baseIntervalsCount;
    }

    function getIntervalHeight() {
        return getIntervalWidth() / verticalToHoriznotalGridIntervalRelation
    }

    function iterateIntervals(target,args) {
        var intervalWidth = getIntervalWidth();

        // draw beats and tacts
        for (var x = intervalWidth, index = 1; x < target.width; x += intervalWidth, index++) {
            args.beatsIterator(x, index % 4 != 0);
        }

        if (args.verticalIterator) {
            var intervalHeight = intervalWidth / verticalToHoriznotalGridIntervalRelation;
            index = 1;
            for (var y = intervalHeight; y < target.height; y += intervalHeight, index++) {
                args.verticalIterator(target.height - y, index % 12 == 0);
            }
        }
    }
}