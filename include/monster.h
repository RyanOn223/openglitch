#ifndef MONSTER_H
#define MONSTER_H
#include "include.h"
#include "entity.h"
#include "resource_manager.h"
#include "texture_manager.h"
#include "datatables.h"
#include "text_node.h"
class monster : public entity
{
	public:
	enum type
	{
		small_mutant,
		large_mutant,
		player,
		type_count,
	};
	private:
		typedef resource_manager<sf::Texture, textures::ID> texture_holder;
	public:
		explicit monster(type _type, const texture_manager& textures, const resource_manager<sf::Font, fonts::ID>& fonts, int hp);
		virtual void draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const;
        virtual unsigned int get_category() const;
        virtual void update_current(sf::Time delta);
        struct monster_mover
        {
            monster_mover(float vx, float vy) : velocity(vx, vy) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                //assume the node we've been passed represents a monster reference
                //this is safe only here, even then watch out later for this
                monster& mon = static_cast<monster&>(node);
                mon.accelerate(velocity);
            }
            sf::Vector2f velocity;
        };
        struct monster_rotator
        {
            monster_rotator(float angle) : theta(angle) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.sprite.setRotation(theta);
            }
            float theta;
        };
	private:
		type monster_type;
		sf::Sprite sprite;
		text_node* health_text;
};
#endif
