/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 21 Aug 2021 10:02:45pm
    Author:  ferna

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource { //public IIRFilterAudioSource
    public:
        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);
        /**Receives reading from slider and sets new coefficient to modify the source */
        void filterAudio(double var);

        void start();
        void stop();

        /**Get the relative position of the playhead*/
        double getPositionRelative();
    private:
        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{&transportSource, false, 2};
        /**Filter object created to modify the source*/
        IIRFilterAudioSource filterSource{&resampleSource,false};
};