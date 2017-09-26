#ifndef MONSTER_H
#define MONSTER_H
#include "include.h"
#include "entity.h"
#include "resource_manager.h"
class monster : public entity
{
	public:
	enum type
	{
		small_mutant,
		large_mutant,
	};
	private:
		typedef resource_manager<sf::Texture, textures::ID> texture_holder;
	public:
		explicit monster(type _type, const texture_holder& textures);
		virtual void draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const;
	private:
		type monster_type;
		sf::Sprite sprite;
};
#endif
