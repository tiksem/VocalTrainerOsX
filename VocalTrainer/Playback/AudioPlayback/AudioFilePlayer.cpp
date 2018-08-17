﻿//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"
#include "AudioUtils.h"
#include <QDebug>

int AudioFilePlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const AudioPlayer::PlaybackData &playbackData) {
    int bufferSeekBefore = getBufferSeek();
    int filePos = audioDecoder->seek(bufferSeekBefore * playbackData.numChannels);
    qDebug() << "AudioFilePlayer::readNextSamplesBatch :: bufferSeekBefore" << bufferSeekBefore
             << ", positionInSamples" << audioDecoder->positionInSamples()
            << ", bufferSeekBefore * numChannels" << bufferSeekBefore * playbackData.numChannels
           << ", filePos"  << filePos;

    assert(audioDecoder->positionInSamples() == bufferSeekBefore * playbackData.numChannels);
    int samplesCount = framesCount * playbackData.numChannels;
    int readFramesCount = audioDecoder->read(samplesCount, (short*)intoBuffer)
            / playbackData.numChannels;
    if (readFramesCount > 0) {
        moveBufferSeekIfNotChangedBefore(readFramesCount, bufferSeekBefore);
    }

    int shiftInSemiTones = getPitchShiftInSemiTones();
    if (shiftInSemiTones != 0) {
        soundTouch.setPitchSemiTones(shiftInSemiTones);
        AudioUtils::Int16SamplesIntoFloatSamples((short*)intoBuffer, samplesCount, tempFloatBuffer.data());
        soundTouch.putSamples(tempFloatBuffer.data(), (uint)framesCount);
        soundTouch.receiveSamples(tempFloatBuffer.data(), (uint)framesCount);
        AudioUtils::FloatSamplesIntoInt16Samples(tempFloatBuffer.data(), samplesCount, (short*)intoBuffer);
    }

    qDebug() << "AudioFilePlayer::readNextSamplesBatch :: read" << readFramesCount << shiftInSemiTones ;
    return readFramesCount;
}

void AudioFilePlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    assert(!audioDecoder);
    audioDecoder = AudioDecoder::create();
    audioDecoder->open(std::move(audioData));
    playbackData->numChannels = audioDecoder->channels();
    playbackData->format = paInt16;
    playbackData->sampleRate = audioDecoder->sampleRate();
    playbackData->framesPerBuffer = 256;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
    
    soundTouch.setChannels((uint)playbackData->numChannels);
    soundTouch.setSampleRate((uint)playbackData->sampleRate);
    tempFloatBuffer.resize(playbackData->framesPerBuffer * playbackData->numChannels);
}

AudioFilePlayer::AudioFilePlayer() {

}

void AudioFilePlayer::setAudioData(std::string &&audioData) {
    destroy();
    this->audioData.swap(audioData);
    setBufferSeek(0);
}

void AudioFilePlayer::destroy() {
    AudioPlayer::destroy();
    if (audioDecoder) {
        delete audioDecoder;
    }
}
