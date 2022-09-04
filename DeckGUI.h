/*
  ==============================================================================

    DeckGUI.h
    Created: 22 Aug 2021 2:44:00pm
    Author:  ferna

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public Button::Listener, public Slider::Listener, public FileDragAndDropTarget, public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /**  Listen to changes on all buttons */
    void buttonClicked(Button*) override;

    /** Listen to changes on all sliders */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    /**Call to WaveFormDisplay, sending track loaded to deck*/
    void loadWave(URL url);
    
    void timerCallback() override;

private:
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };

    Slider volSlider;
    Label volLabel;

    Slider speedSlider;
    Label speedLabel;

    Slider posSlider;

    Slider sSlider;
    Label sLabel;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
