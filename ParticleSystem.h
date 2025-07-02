#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
    explicit ParticleSystem(std::size_t reserve = 0)
    {
        m_particles.reserve(reserve);
    }

    // Function that sets the emitters position
    void setEmitter(const sf::Vector2f& position)
    {
        m_emitter = position;
    }

    void setAmplitude(float amp) {
        m_audioAmp = std::clamp(amp, 0.1f, 1.f);
    }

    // Function that sets the lifetime of the particles.
    void setParticleLifetime(sf::Time lifetime)
    {
        m_lifetime = lifetime;
    }

    // Function that is responsible for spawning the particles.
    void spawnParticles(std::size_t count)
    {
        for (std::size_t n = 0; n < count; ++n)
        {
            m_particles.emplace_back();
            m_vertices.append(sf::Vertex());

            resetParticle(m_particles.back(),
                          m_vertices[m_vertices.getVertexCount() - 1]);
        }
    }

    // Function that is responsible for handling the and updating the particles every frame.
    void update(sf::Time dt)
    {
        constexpr float hitThreshold = 0.3f;
        constexpr std::size_t maxBurst = 100;

        if (m_audioAmp > hitThreshold) {
            float t = (m_audioAmp - hitThreshold) / (1.f - hitThreshold);
            std::size_t count = static_cast<std::size_t>(10 + t * (maxBurst - 10));
            spawnParticles(count);
        }

        for (std::size_t i = 0; i < m_particles.size(); )
        {
            Particle& p     = m_particles[i];
            sf::Vertex& v   = m_vertices[i];

            p.lifetime -= dt;
            v.position += p.velocity * dt.asSeconds();

            if (p.lifetime <= sf::Time::Zero)
            {
                std::size_t last = m_particles.size() - 1;
                m_particles[i]  = m_particles[last];
                m_vertices[i]   = m_vertices[last];
                m_particles.pop_back();
                m_vertices.resize(last);
                continue;
            }

            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            ratio = std::clamp(ratio, 0.f, 1.f);
            v.color.a = static_cast<std::uint8_t>(ratio * 255);

            ++i;
        }
    }

private:
    // Basic particle struct
    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time     lifetime;
    };

    std::vector<Particle> m_particles;
    sf::VertexArray       m_vertices{sf::Points};
    sf::Time              m_lifetime{sf::seconds(3.f)};
    sf::Vector2f          m_emitter{};
    float m_audioAmp = 0.f;

    static std::random_device rd;
    static std::mt19937 rng;
    static std::uniform_real_distribution<float> angleDeg;
    static std::uniform_real_distribution<float> speedDist;

    // Function that is responsible for handling the 'reset' logic of the particles.
    void resetParticle(Particle& p, sf::Vertex& v)
    {
        float theta = angleDeg(rng) * 3.14159265f / 180.f;
        // Make speed scale with current audio amplitude
        float audioScale = 0.6f + 2.0f * m_audioAmp;
        float speed = speedDist(rng) * audioScale;

        p.velocity = { std::cos(theta) * speed, std::sin(theta) * speed };
        p.lifetime = m_lifetime;

        v.position = m_emitter;
        if (m_audioAmp > 0.9) {
            v.color = sf::Color::Red;
        }
        else if (m_audioAmp > 0.6 && m_audioAmp < 0.8) {
            v.color = sf::Color::Yellow;
        }
        else if (m_audioAmp < 0.6 && m_audioAmp > 0.25) {
            v.color    = sf::Color::Green;
        }
        else {v.color = sf::Color::White;}
    }

    // Basic draw function
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_vertices, states);
    }
};

// Static member definitions (must appear in header if header-only)
inline std::random_device ParticleSystem::rd;
inline std::mt19937 ParticleSystem::rng(ParticleSystem::rd());
inline std::uniform_real_distribution<float> ParticleSystem::angleDeg(-360.f, 90.f);
inline std::uniform_real_distribution<float> ParticleSystem::speedDist(10.f, 50.f);

#endif
