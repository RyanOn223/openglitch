//this file has been omitted and included into its header for c++ template reasons


//#include "resource_manager.h"
/*template <typename resource, typename identifier>
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

std::unique_ptr<resource> gen_resource(new resource());
void resource_manager<resource, identifier>::load(identifier id, const std::string& filename)
{
	//create a unique pointer to hold the resource
	std::unique_ptr<resource> gen_resource(new resource());
	//load it and insert it into the resource map
	assert(resource.loadFromFile(filename)= true);
	resource_map::insert(std::make_pair(id, std::move(gen_resource)));

}
resource& resource_manager<resource, identifier>::get(identifier id)
{
	auto found = resource_map.find(id);
	assert(found != resource_map.end());
	return (*found)->second;
}
const resource& resource_manager<resource, identifier>::get(identifier id);
{
	auto found = resource_map.find(id);
	assert(found != resource_map.end());
	return (*found)->second;
}
*/
