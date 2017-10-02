#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include "include.h"
//#include "resource_manager.h"
class texture_manager
{
	public:
	//functions to return a reference to a texture
		sf::Texture& get(textures::ID id);
		const sf::Texture& get(textures::ID id) const;
		//used to load
		void load(textures::ID id, const std::string& filename);
	private:
	//the actual texture map
		std::map<textures::ID,
			std::unique_ptr<sf::Texture>> texture_map;
};
#endif // TEXTURE_MANAGER_H
