#include "../include/texture_manager.h"
void texture_manager::load(textures::ID id, const std::string& filename)
{
	//create a unique pointer to hold the texture
	std::unique_ptr<sf::Texture> gen_texture(new sf::Texture);
	//load it and insert it into the texture map
	assert(gen_texture->loadFromFile(filename));
	texture_map.insert(std::make_pair(id, std::move(gen_texture)));
}
sf::Texture& texture_manager::get(textures::ID id)
{
	auto found = texture_map.find(id);
	assert(found != texture_map.end());
	return *found->second;
}
const sf::Texture& texture_manager::get(textures::ID id) const
{
	auto found = texture_map.find(id);
	assert(found != texture_map.end());
	return *found->second;
}
