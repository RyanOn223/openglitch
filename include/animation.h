#ifndef ANIMATION_H
#define ANIMATION_H
#include "SFML/System.hpp"
#include "texture_manager.h"

class animation : public sf::Drawable, public sf::Transformable
{
    public:
        enum type
        {
            explosion,
            player_walk,
            small_mutant_walk,
            none,
            animation_type_count
        };
        animation(animation::type Type);
        explicit animation(const texture_manager& textures, animation::type Type);
        void set_texture(const sf::Texture& texture);
        const sf::Texture* get_texture() const;
        void set_frame_size(sf::Vector2i size);
        sf::Vector2i get_frame_size() const;
        void set_num_frames(std::size_t num);
        std::size_t get_num_frames() const;
        void set_duration(sf::Time);
        sf::Time get_duration() const;
        void set_repeat(bool);
        bool is_repeat() const;
        void restart();
        bool is_finished() const;
        animation::type get_type() const;
        //this will be used in the context of other sfml functions
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
        void update(sf::Time delta);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        sf::Sprite sprite;
        sf::Vector2i frame_size;
        std::size_t num_frames;
        std::size_t current_frame;
        sf::Time duration;
        sf::Time elapsed_time;
        bool repeat;
        type atype;
};

#endif // ANIMATION_H
