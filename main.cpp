#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"

int main()
{
    constexpr unsigned WIN_W = 800;
    constexpr unsigned WIN_H = 600;

    sf::RenderWindow window({WIN_W, WIN_H}, "Particle Burst");
    window.setVerticalSyncEnabled(true);

    ParticleSystem particles;                          // default reserve = 0
    particles.setEmitter({WIN_W / 2.f, WIN_H / 2.f});  // centre of the window
    particles.setParticleLifetime(sf::seconds(3.f));   // tweak as you like

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
        particles.update(dt);

        window.clear();
        window.draw(particles);
        window.display();
    }

    return 0;
}
