#include "projectile.h"

projectile::projectile(type pptype, const texture_manager& textures) :
    ptype(pptype),
    sprite(textures.get(textures::bullet)),
    entity(1)
{
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

projectile::~projectile()
{
    //dtor
}
