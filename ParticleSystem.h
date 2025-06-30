#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <cstdint>
#include <random>

#include "SFML/Graphics.hpp"

class ParticleSystem : public sf::Drawable, public  sf::Transformable {
public:
    ParticleSystem(unsigned int count) : m_particles(count), m_vertices(sf::PrimitiveType::Points, count){}

    void setEmitter(sf::Vector2f position) {
        m_emitter = position;
    };

    void update(sf::Time elapsed) {
        for (std::size_t i = 0; i < m_particles.size(); ++i) {
            // Update lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // Respawn particle if dead
            if (p.lifetime <= sf::Time::Zero) {
                resetParticle(i);
            }

            //Update position of corresponding vertex
            m_vertices[i].position += p.velocity * elapsed.asSeconds();

            //update transparency according to lifetime
            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
        }
    };

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply transform
        states.transform *= getTransform();

        // no texture as of now
        states.texture = nullptr;

        // drawing vertex array
        target.draw(m_vertices, states);
    }

    struct Particle {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t index) {
        // random number gen
        static std::random_device rd;
        static std::mt19937 rng(rd());

        // assign random velocity and lifetime
        const float angle = std::uniform_real_distribution<float>(0.f, 180.f)(rng);
        const float speed = std::uniform_real_distribution(1.f, 5.f)(rng);
        m_particles[index].velocity = sf::Vector2f(speed, angle);
        m_particles[index].lifetime = sf::milliseconds(std::uniform_int_distribution(1000, 3000)(rng));

        // reset position of corresponding vertex
        m_vertices[index].position = m_emitter;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime{sf::seconds(3)};
    sf::Vector2f m_emitter;
};


#endif