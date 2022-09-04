/*
  ==============================================================================

    WaveformDisplay.h
    Created: 29 Aug 2021 7:04:39pm
    Author:  ferna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /**On change, repaint the component*/
    void changeListenerCallback(ChangeBroadcaster *source) override;
    /**Clears last thumbnail and populates a new one from an URL*/
    void loadURL(URL audioURL);
    /**Set the relative position of the playhead*/
    void setPositionRelative(double pos);
private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    /**Stores position to correspond to the track and is used to drawn*/
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
