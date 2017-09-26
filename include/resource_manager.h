#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "include.h"
template <typename resource, typename identifier>
class resource_manager
{
	public:
	//functions to return a reference to a texture
		resource& get(identifier id);
		const resource& get(identifier id) const;
		//used to load
		void load(identifier id, std::string const& filename);
		template <typename parameter>
		void load(identifier id, const std::string& filename,
			const parameter& secondparam);
	private:
	//the actual texture map
		std::map<identifier,
			std::unique_ptr<resource>> resource_map;
};


template <typename resource, typename identifier>
template <typename parameter>
void resource_manager<resource, identifier>::load(identifier id,
const std::string& filename, const parameter& secondparam)
{
	//create a unique pointer to hold the resource
	std::unique_ptr<resource> gen_resource(new resource());
	//load it and insert it into the resource map
	gen_resource.loadFromFile(filename, secondparam) = true;
	resource_map.insert(std::make_pair(id, std::move(gen_resource)));
}
template <typename resource, typename identifier>
void resource_manager<resource, identifier>::load(identifier id, const std::string& filename)
{
	//create a unique pointer to hold the resource
	std::unique_ptr<resource> gen_resource(new resource());
	//load it and insert it into the resource map
	gen_resource->loadFromFile(filename);
	resource_map.insert(std::make_pair(id, std::move(gen_resource)));

}
template <typename resource, typename identifier>
resource& resource_manager<resource, identifier>::get(identifier id)
{
	auto found = resource_map.find(id);
	assert(found != resource_map.end());
	return *found->second;
}
template <typename resource, typename identifier>
const resource& resource_manager<resource, identifier>::get(identifier id) const
{
	auto found = resource_map.find(id);
	assert(found != resource_map.end());
	return *found->second;
}
#endif // RESOURCE_MANAGER_H
