#include <SFML/Graphics.hpp>
#include <iostream>
#include "ParticleSystem.h"
#include "ReactiveRecorder.h"
#include "SFML/Audio/SoundRecorder.hpp"

int main()
{
    if (!sf::SoundRecorder::isAvailable()) {
        std::cerr << "Microphone capture not available.\n";
        return EXIT_FAILURE;
    }

    constexpr unsigned WIN_W = 800;
    constexpr unsigned WIN_H = 600;

    sf::RenderWindow window({WIN_W, WIN_H}, "Isaiah Broderson - Audio Particles");
    window.setVerticalSyncEnabled(true);

    ParticleSystem particles;                          // default reserve = 0
    particles.setEmitter({WIN_W / 2.f, WIN_H / 2.f});  // set emitter position
    particles.setParticleLifetime(sf::seconds(3.f));   // setting the particle lifetime

    ReactiveRecorder recorder(particles, 8000.f);
    recorder.setChannelCount(1);
    if (!recorder.start(44100)) {
        std::cerr << "Failed to start recording(audio).\n";
        return EXIT_FAILURE;
    }

    for (auto&& name : sf::SoundRecorder::getAvailableDevices()) {
        std::cout << name << "\n";
    }

    recorder.setDevice("OpenAL Soft on Headset (WH-1000XM4)");

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // single burst per key-down
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
            {
                particles.spawnParticles(1000);         // particle burst of X amount
            }
        }

        sf::Time dt = clock.restart();

        particles.setAmplitude(recorder.currentAmplitude() / 500.f);
        particles.update(dt);

        window.clear();
        window.draw(particles);
        window.display();
    }

    recorder.stop();
    return 0;
}
