#include "qmlpitchinputreader.h"
#include "appsettings.h"

using namespace CppUtils;

QmlPitchInputReader::QmlPitchInputReader(QObject *parent) : QObject(parent) {
    init(AppSettings().getMicrophoneDeviceName());
    addPitchDetectedListener([=] (const Pitch& pitch, double time) {
        emit pitchDetected(QmlTimedPitch(pitch, time));
        return DONT_DELETE_LISTENER;
    });
}

void QmlPitchInputReader::start() {
    PitchInputReaderAndPlayer::start();
}

void QmlPitchInputReader::stop() {
    PitchInputReaderAndPlayer::stop();
}

QmlTimedPitch QmlPitchInputReader::pitchAt(int index) {
    return QmlTimedPitch(getFrequencyAt(index), getTimeAt(index));
}

void QmlPitchInputReader::setThreshold(float threshold) {
    PitchInputReaderAndPlayer::setThreshold(threshold);
    emit thresholdChanged(threshold);
}

QmlTimedPitch QmlPitchInputReader::getLastDetectedPitch() {
    return QmlTimedPitch(getLastDetectedFrequency(), getLastDetectedTime());
}
