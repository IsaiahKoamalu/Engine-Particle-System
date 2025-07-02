#ifndef REACTIVERECORDER_H
#define REACTIVERECORDER_H

#include <SFML/Audio.hpp>
#include <atomic>
#include "ParticleSystem.h"

class ReactiveRecorder : public sf::SoundRecorder {
public:
    ReactiveRecorder(ParticleSystem& ps, float thresh = 8000.f)
        : particles(ps), threshold(thresh){}
private:
    ParticleSystem& particles;
    float threshold;
    std::atomic<float> smoothedAmp = 0.f;

    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override {

        //static std::size_t calls = 0;
        //std::cout << "callback #" << ++calls << ", samples = " << sampleCount<< "\\n";

        double sumSq = 0;
        for (std::size_t i = 0; i < sampleCount; ++i) {
            sumSq += samples[i] * samples[i];
        }

        float rms = std::sqrt(sumSq / sampleCount);
        smoothedAmp = 0.9f * smoothedAmp + 0.1f * rms;

        if (rms > threshold) {
            particles.spawnParticles(20);
        }
        //std::cout << "RMS: " << rms << std::endl;
        return true;
    }

public:
    float currentAmplitude() const { return smoothedAmp.load();}
};


#endif