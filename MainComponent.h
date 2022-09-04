/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================.
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    /**Find a comma separated value on your build and parse it into tokens to be stored in a String Vector*/
    std::vector<std::vector<String>> parseTxtFile();
    
private:
    //==============================================================================
    // Your private member variables go here...
    /**This variable is required to be initialised here as when the application is opened, the stored tracks will be populated into new array
      *The txt file will later be blanked and rewritten from this variable.
       */
    std::vector<std::vector<String>> savedTracks{ parseTxtFile() };

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{&player2, formatManager, thumbCache };

    MixerAudioSource mixerSource;

    /**Component that creates playlist, requires view of decks and players to load the tracks*/
    PlaylistComponent playlistComponent{ formatManager, &player1 , &player2, &deckGUI1, &deckGUI2, savedTracks };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
