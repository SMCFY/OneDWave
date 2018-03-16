/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public Slider::Listener, public::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void sliderValueChanged(Slider *slider) override;
    void timerCallback() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    float interpolate(float x, float *buffer);
    // Your private member variables go here...
    float value;
    float freq;
    float delta;
    float audioBufferIndex;
    int paintBufferIndex;
    int audioBufferLength;
    int paintBufferLength;
    float U[3][1024];
    int mod;
    Slider waveSlider;
    Slider freqSlider;
    HeapBlock<float> audioBuffer;
    HeapBlock<float> paintBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
