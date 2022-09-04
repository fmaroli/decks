/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 29 Aug 2021 8:10:56pm
    Author:  ferna

  ==============================================================================
*/

#include "JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================//, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse waveformDisplay(formatManagerToUse, cacheToUse)
PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManager, DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, std::vector<std::vector<String>> savedTracks)
                                    : formatManager(_formatManager), player1(_player1), player2(_player2), deckGUI1(_deckGUI1), deckGUI2(_deckGUI2), trackTitles(savedTracks)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs

    tableComponent.getHeader().addColumn("A", 1, 50);
    tableComponent.getHeader().addColumn("B", 2, 50);
    tableComponent.getHeader().addColumn("Track title", 3, 600);
    tableComponent.getHeader().addColumn("Length", 4, 50);
    tableComponent.getHeader().addColumn("Delete", 5, 50);
    tableComponent.setModel(this);

    addAndMakeVisible(searchInput);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(tableComponent); 

    searchInput.addListener(this);
    loadButton.addListener(this);
    
    /**Populates txt file after blanking it*/
    std::ostream_iterator<String> output_iterator(output_file, ",");
    for (int i = 0; i < trackTitles.size(); i++) {
        copy(trackTitles.at(i).begin(), trackTitles.at(i).end(), output_iterator);
        output_file << std::endl;
    }

    trackTitlesDisplay = trackTitles;
    searchInput.setTextToShowWhenEmpty("Search here..", Colour::fromRGB(255,255,255));
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    tableComponent.getHeader().setColour(TableHeaderComponent::backgroundColourId, Colour::fromRGB(60, 60, 60));
    tableComponent.getHeader().setColour(TableHeaderComponent::textColourId, Colour::fromRGB(255, 255, 255));
}

void PlaylistComponent::resized()
{
    tableComponent.setColour(TableListBox::backgroundColourId, Colour::fromRGB(40, 40, 40));

    searchInput.setBounds(0, 0, getWidth()/2, 25);
    searchInput.setColour(TextEditor::backgroundColourId, Colour::fromRGB(40, 40, 40));

    loadButton.setBounds(getWidth()/2, 0, getWidth()/2, 25);
    loadButton.setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));

    tableComponent.setBounds(0,25,getWidth()+15,getHeight()-25);
}
int PlaylistComponent::getNumRows() {
    return trackTitlesDisplay.size();
}
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(Colours::cadetblue);
    }
    else {
        g.fillAll(Colour::fromRGB(40,40,40));
    }
}
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.setColour(Colour::fromRGB(255, 255, 255));
    if (columnId == 3) {
        g.drawText(trackTitlesDisplay[rowNumber][0], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    if (columnId == 4) {
        g.drawText(trackTitlesDisplay[rowNumber][1], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    if (columnId == 1) {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Play" };
            String id{std::to_string(rowNumber)};
            btn->setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));

            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Play" };
            String id{ std::to_string(rowNumber+100) };
            btn->setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "-" };
            String id{ std::to_string(rowNumber + 200) };
            btn->setColour(TextButton::buttonColourId, Colour::fromRGB(40, 40, 40));
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}
void PlaylistComponent::buttonClicked(Button* button) {
    //if button hasn't got an ID, do nothing
    if (button->getComponentID().toStdString() == "") {
        
    }
    else {
        int id = std::stoi(button->getComponentID().toStdString());
        if (id < 100) {
            player1->loadURL(trackTitlesDisplay[id][2]);
            deckGUI1->loadWave(trackTitlesDisplay[id][2]);
        }
        else if (id < 200 && id >= 100) {
            id = id - 100;
            player2->loadURL(trackTitlesDisplay[id][2]);
            deckGUI2->loadWave(trackTitlesDisplay[id][2]);
        }
        else {
            id = id - 200;
            trackTitles = splice(id);
            trackTitlesDisplay = trackTitles;
            tableComponent.updateContent();

            std::ofstream output_file{ "loaded_tracks.txt" };
            std::ostream_iterator<String> output_iterator(output_file, ",");
            for (int i = 0; i < trackTitles.size(); i++) {
                copy(trackTitles.at(i).begin(), trackTitles.at(i).end(), output_iterator);
                output_file << std::endl;
            }
        }
    }
    if (button == &loadButton) {
        FileChooser chooser{ "Select a file.." };
        if (chooser.browseForMultipleFilesToOpen()) {
            Array<File> files = chooser.getResults();
            for (int i = 0; i < files.size(); i++) {
                AudioFormatReader* reader = formatManager.createReaderFor(files[i]);
                String s = URL{ files[i] }.toString(true);
                String name = URL{ files[i] }.getFileName();
                String len = std::to_string(int(reader->lengthInSamples / reader->sampleRate));
                trackTitles.push_back({ name, len, s });
                trackTitlesDisplay = trackTitles;

                std::ostream_iterator<String> output_iterator(output_file, ",");
                copy(trackTitles.at(trackTitles.size() - 1).begin(), trackTitles.at(trackTitles.size() - 1).end(), output_iterator);
                output_file << std::endl;
            }
            tableComponent.updateContent();
        }
    }
}
void PlaylistComponent::textEditorTextChanged(TextEditor& input) {
    if (input.getText().length() > 0) {
        trackTitlesDisplay.clear();
        for (int i = 0; i < trackTitles.size(); ++i) {
            if (trackTitles[i][0].startsWith(input.getText())) {
                trackTitlesDisplay.push_back(trackTitles[i]);
            }
        }
        tableComponent.updateContent();
    }
    else if (input.getText().length() == 0) {
        trackTitlesDisplay = trackTitles;
        tableComponent.updateContent();
    }
}

std::vector<std::vector<String>> PlaylistComponent::splice(int idToRemove) {
    std::vector<std::vector<String>> newV;
    for (int i = 0; i < trackTitles.size(); ++i) {
        if (i == idToRemove) {
            continue;
        }
        newV.push_back(trackTitles[i]);
    }
    return newV;
}
