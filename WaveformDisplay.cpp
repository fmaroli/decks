/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 29 Aug 2021 7:04:39pm
    Author:  ferna

  ==============================================================================
*/

#include "JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colour::fromRGB(40, 40, 40));   // clear the background

    g.setColour (Colour::fromRGB(0, 0, 0));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colour::fromRGB(70, 130, 220));
    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colours::cyan);
        g.drawRect(position*getWidth(), 0, getWidth()/40, getHeight());
    }
    else {
        g.setFont(14.0f);
        g.drawText("Load file to start", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    //DBG(audioThumb.getTotalLength());
    if (fileLoaded) {
        //DBG("WaveformDisplay::LOADED");
    }
    else {
        //DBG("WaveformDisplay::NOT LOADED");
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source) {
    repaint();
}
void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }
}