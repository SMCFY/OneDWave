/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
: value(0), delta(0.0f), audioBufferIndex(0), paintBufferIndex(1023), audioBufferLength(44100),
  paintBufferLength(1024), audioBuffer(audioBufferLength), paintBuffer(paintBufferLength), mod(0)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    
    // create a slider GUI for instigating wave propogation
    addAndMakeVisible(slider);
    slider.setSliderStyle(Slider::LinearBarVertical);
    slider.setRange(-1.0, 1.0);
    slider.addListener(this);
    
    // initialize the audio buffer
    for (int i = 0; i < audioBufferLength; i++)
    {
        audioBuffer[i] = 0.0f;
    }
    // initialize the paint buffer
    for (int i = 0; i < 1024; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            U[j][i] = 0.0f;
            paintBuffer[i] = 0.0f;
        }
    }
    // finite-difference function will be calculated at a rate of 120 Hz
    startTimerHz(120);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    
    AudioBuffer<float> *buffer = bufferToFill.buffer;
    
    for (int n = 0; n < buffer->getNumSamples(); n++, audioBufferIndex+=delta)
    {
        if (audioBufferIndex >= paintBufferLength)
            audioBufferIndex -= paintBufferLength;
        for (int ch = 0; ch < buffer->getNumChannels(); ch++)
        {
            float *buffch = buffer->getWritePointer(ch);
            buffch[n] = 0.2f*interpolate(audioBufferIndex, audioBuffer);
        }
    }
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    double length = paintBufferLength;
    for (int i = 0; i < 1023; i++)
    {
        // 1D wave will be painted green
        g.setColour(juce::Colours::green);
        // waves are drawn depending on what's stored in the dynamically-changing paint buffer
        g.drawLine(i*getWidth()/length, 50.0+getWidth()/4.0+getWidth()/4.0*paintBuffer[i], (i+1)*getWidth()/length, 50.0+(getWidth()/4.0)+(getWidth()/4.0)*paintBuffer[i+1], 2);
    }
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    slider.setBounds(0, 50, 10, getHeight()-200);
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    value = slider->getValue();
    delta = 1.0f;
    if (paintBufferIndex<0)
        paintBufferIndex += paintBufferLength;
}

void MainComponent::timerCallback()
{
    float v,c, dt, dx;
    int j = 2;
    
    dx = 1;
    dt = 1.0/44100.0f;
    v = 40000.0f;
    c = v*(dt/dx);
    float U1, U2;
    for (int i = 0; i < 1024; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            U[j][i] = U[j+1][i];
        }
    }
    U[2][0] = -value;
    U[2][1023] = -value;
    for (int i = 1; i < 1023; i++)
    {
        U1 = 2*U[j-1][i] - U[j-2][i];
        U2 = U[j-1][i-1] - 2*U[j-1][i] + U[j-1][i+1];
        U[j][i] =  U1 + c*c*U2;
    }
    for (int i = 0; i < 1024; i++)
    {
        paintBuffer[i] = audioBuffer[i] = U[2][i];
    }
    for (int i = 0; i < 44100; i++)
    {
        
    }
    mod++;
    // draw the current waveform, every 40 Hz
    if (mod%3)
        repaint();
}

float MainComponent::interpolate(float x, float *buffer)
{
    int x0 = std::floor(x), x1 = x0+1;
    float phi = x - x0;
    return buffer[x0]*(1-phi)+buffer[x1]*(phi);
}
