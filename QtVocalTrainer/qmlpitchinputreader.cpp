#include "qmlpitchinputreader.h"
#include "../PitchDetection/CppUtils/TimeUtils.h"
#include "../PitchDetection/CppUtils/Executors.h"
#include "app.h"

static const int PITCH_DETECTION_BUFFER_SIZE = 1200;
static const int PITCH_SMOOTH_LEVEL = 4;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    AudioInputReader* audioInputReader = CreateDefaultAudioInputReader(PITCH_DETECTION_BUFFER_SIZE);
    init(audioInputReader, PITCH_SMOOTH_LEVEL, true);
}

void QmlPitchInputReader::start() {
    PitchInputReaderCollector::start();
}

void QmlPitchInputReader::stop() {
    PitchInputReaderCollector::stop();
}

QmlPitch QmlPitchInputReader::pitchAt(int index) {
    return QmlPitch(getFrequencyAt(index), getTimeAt(index));
}

void QmlPitchInputReader::pitchDetected(float frequency, double time) {
    emit pitchDetected(QmlPitch(frequency, time));
}
