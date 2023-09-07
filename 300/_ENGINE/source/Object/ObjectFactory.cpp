#include "../../include/Object/ObjectFactory.h"

void ObjectFactory::DeserializeScene(const std::string& filename)
{
	// loop thru container and store entities
}

void ObjectFactory::SerializeScene(const std::string& filename)
{
	// loop thru container and store entity data from editor
	// into .json file

	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);
	ofs.close();

	EntityJSON ent;
	EntityListJSON entities;
	entities.EntitiesList().clear();

	auto container = ECS::GetInstance()->GetEntitiesWith<General, Transform>();

	for (Entity e : container)
	{
		// set the data for general component
		General gen = e.GetComponent<General>();
		ent.SetGeneralJSON(gen);

		// push back after done
		entities.EntitiesList().push_back(ent);
	}
	entities.SerializeFile(filename);
}