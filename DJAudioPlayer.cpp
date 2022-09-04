/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 21 Aug 2021 10:02:45pm
    Author:  ferna

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager) {

}
DJAudioPlayer::~DJAudioPlayer() {

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    filterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    filterSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
    filterSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) //good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::filterAudio(double var) {
    filterSource.setCoefficients(IIRCoefficients::makeLowPass(48000, 4800, var));
}
void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1) {
        DBG("setGain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100) {
        DBG("setSpeed should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}
void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1) {
        DBG("setPositionRelative should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() {
    transportSource.start();
}
void DJAudioPlayer::stop() {
    transportSource.stop();
}
double DJAudioPlayer::getPositionRelative() {
    if (transportSource.getLengthInSeconds() != 0) {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
    return 0;
}