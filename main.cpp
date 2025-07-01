#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"

int main()
{
    int WIDTH = 800;
    int HEIGHT = 600;
    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Particle System");

    ParticleSystem particles(1000);

    sf::Clock clock;

    while (window.isOpen()) {
        //float dt = clock.restart().asSeconds();
        sf::Event event;

        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // emitter follows mouse
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        particles.setEmitter(window.mapPixelToCoords(mouse));

        // updates
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);

        // rendering
        window.clear(sf::Color::Black);
        window.draw(particles);
        window.display();
    }
    return 0;
}