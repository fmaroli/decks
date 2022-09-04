/*
  ==============================================================================

    DeckGUI.cpp
    Created: 22 Aug 2021 2:44:00pm
    Author:  ferna

  ==============================================================================
*/


#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse) 
                : player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(posSlider);

    addAndMakeVisible(sSlider);
    addAndMakeVisible(sLabel);

    addAndMakeVisible(waveformDisplay);
    

    playButton.addListener(this);
    stopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    sSlider.addListener(this);

    volSlider.setRange(0.0,1.0);
    speedSlider.setRange(0.01, 2.0);
    posSlider.setRange(0.0, 1.0);
    sSlider.setRange(0.1, 3.0);

    volSlider.setValue(1);
    speedSlider.setValue(1);
    sSlider.setValue(1);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (Colour::fromRGB(40, 40, 40));   // clear the background

    g.setColour (Colour::fromRGB(0, 0, 0));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    //g.drawText ("Playback Speed", getLocalBounds(), juce::Justification::centred, true); //speedSlider.getLocalBounds()

    posSlider.setValue(player->getPositionRelative());
    playButton.setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));
    stopButton.setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volSlider.setColour(Slider::trackColourId, Colour::fromRGB(70, 130, 220));
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    volLabel.setText("GAIN", dontSendNotification);
    
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedLabel.setText("Playback Speed", dontSendNotification);
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGB(90, 90, 90));
    speedSlider.setColour(Slider::thumbColourId, Colour::fromRGB(70, 130, 220));
    speedSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(70, 70, 70));

    sSlider.setSliderStyle(Slider::Rotary);
    sSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    sLabel.setText("LOW", dontSendNotification);
    sSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGB(90, 90, 90));
    sSlider.setColour(Slider::thumbColourId, Colour::fromRGB(70, 130, 220));
    sSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(70, 70, 70));
    
    posSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    posSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(90, 90, 90));
    posSlider.setColour(Slider::trackColourId, Colour::fromRGB(70, 70, 70));
    posSlider.setColour(Slider::thumbColourId, Colour::fromRGB(70, 130, 220));

    double rowH = getHeight() / 10;
    double colW = getWidth() / 20;
    
    waveformDisplay.setBounds   (colW * 0 , rowH * 0, colW * 20  , rowH * 3);
    posSlider.setBounds         (colW * 0 , rowH * 3, colW * 20  , rowH * 1);
    playButton.setBounds        (colW * 0 , rowH * 4, colW * 10  , rowH * 1);
    stopButton.setBounds        (colW * 10, rowH * 4, colW * 10  , rowH * 1);
    volSlider.setBounds         (colW * 18, rowH * 5, colW * 2   , rowH * 5);
    volLabel.setBounds          (colW * 18, rowH * 7, colW * 2   , rowH * 1);
    speedSlider.setBounds       (colW * 1 , rowH * 5, colW * 7.5 , rowH * 5);
    speedLabel.setBounds        (colW * 2 , rowH * 7, colW * 5   , rowH * 1);
    sSlider.setBounds           (colW * 10, rowH * 5, colW * 7.5 , rowH * 5);
    sLabel.setBounds            (colW * 13, rowH * 7, colW * 2   , rowH * 1);
}
void DeckGUI::buttonClicked(Button* button) {
    if (button == &playButton) {
        player->start();
    }
    if (button == &stopButton) {
        player->stop();
    }
}
void DeckGUI::sliderValueChanged(Slider* slider) {
    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
    if (slider == &sSlider) {
        player->filterAudio(slider->getValue());
    }
    if (slider->isMouseButtonDown()) {
        if (slider == &posSlider) {
            player->setPositionRelative(slider->getValue());
        }
    }
    
}
bool DeckGUI::isInterestedInFileDrag(const StringArray& files) {
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}
void DeckGUI::filesDropped(const StringArray& files, int x, int y) {
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1) {
        player->loadURL(URL{ File{ files[0] } });
        waveformDisplay.loadURL(URL{File(files[0])});
    }
}
void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
void DeckGUI::loadWave(URL url) {
    waveformDisplay.loadURL(url);
}