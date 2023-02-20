#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    
    formatManager.registerBasicFormats();

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //Synthesis
    //phase = 0.0 ;
    //dphase = 0.0001;
    
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);


}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{

    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // You can add your drawing code here!
}


/** VIEW */
void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() /2, getHeight() /2);
    deckGUI2.setBounds(getWidth() /2, 0, getWidth() /2, getHeight() /2);
    playlistComponent.setBounds(0, getHeight() /2, getWidth(), getHeight() /2);
}

/*
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    auto* leftChan = bufferToFill.buffer->getWritePointer(0,
                                                          bufferToFill.startSample);
    auto* rightChan = bufferToFill.buffer->getWritePointer(0,
                                                          bufferToFill.startSample);
    
    for (auto i=0 ; i < bufferToFill.numSamples ; ++i )
    {
        //double sample = rand.nextDouble() * 0.5;
        //double sample = fmod(phase,0.2);
        double sample = sin(phase)*0.1;
        
        leftChan[i] = sample;
        rightChan[i] = sample;
        
        phase += dphase;
        
    }

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.clearActiveBufferRegion();
}
 */
