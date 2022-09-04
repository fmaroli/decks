/*
  ==============================================================================

    PlaylistComponent.h
    Created: 29 Aug 2021 8:10:56pm
    Author:  ferna

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

#include <iostream>
#include <fstream>
//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component, public TableListBoxModel, public Button::Listener, public TextEditor::Listener
{
public:
    PlaylistComponent(AudioFormatManager& _formatManager, DJAudioPlayer* player1, DJAudioPlayer* player2, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, std::vector<std::vector<String>> savedTracks);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /**Splice a vector passing index to be removed */
    std::vector<std::vector<String>> splice(int idToRemove);

    /*LISTENERS*/

    /**Implements button listener*/
    void buttonClicked(Button* button) override;
    /**Listener for search button. On every change to TextEditor, this function will be called*/
    void textEditorTextChanged(TextEditor& input);
private:
    TextButton loadButton{"LOAD"};
    TextEditor searchInput;

    TableListBox tableComponent;

    /**New loaded tracks will be pushed to this vector*/
    std::vector<std::vector<String>> trackTitles;

    DJAudioPlayer* player1;
    DJAudioPlayer* player2;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    /**Blanks txt file used to store the tracks*/
    std::ofstream output_file{"loaded_tracks.txt"};

    /**Tracks that will be displayed, necessary to create a filter view, without deleting tracks*/
    std::vector<std::vector<String>> trackTitlesDisplay;

    /**FormatManager to create MP3 reader, this will be used to get the length of the track when loading*/
    AudioFormatManager& formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
