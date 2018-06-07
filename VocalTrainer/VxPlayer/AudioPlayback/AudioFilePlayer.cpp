//
// Created by Semyon Tikhonenko on 6/7/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "AudioFilePlayer.h"

int AudioFilePlayer::readNextSamplesBatch(void *intoBuffer, int framesCount, const AudioPlayer::PlaybackData &playbackData) {
    return audioDecoder->read(framesCount * playbackData.numChannels, (SAMPLE*)intoBuffer) / playbackData.numChannels;
}

void AudioFilePlayer::prepareAndProvidePlaybackData(AudioPlayer::PlaybackData *playbackData) {
    if (audioDecoder) {
        delete audioDecoder;
    }

    audioDecoder = AudioDecoder::create();
    audioDecoder->open(std::move(audioData));
    playbackData->numChannels = audioDecoder->channels();
    playbackData->format = paInt16;
    playbackData->sampleRate = audioDecoder->sampleRate();
    playbackData->framesPerBuffer = paFramesPerBufferUnspecified;
    playbackData->totalDurationInSeconds = audioDecoder->duration();
}

int AudioFilePlayer::getBufferSeek() const {
    return 0;
}

AudioFilePlayer::~AudioFilePlayer() {
    if (audioDecoder) {
        delete audioDecoder;
    }
}

AudioFilePlayer::AudioFilePlayer(std::string &&audioData) : audioData(std::move(audioData)) {

}
