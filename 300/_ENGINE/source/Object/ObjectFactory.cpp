#include "Object/ObjectFactory.h"

void ObjectFactory::DeserializeScene(const std::string& filename)
{
	// loop thru container and store entities

	EntityListJSON entities;
	entities.DeserializeFile(filename);

	for (auto& obj : entities.EntitiesList())
	{
		Entity e = ECS::GetInstance()->NewEntity();
		General gen = e.GetComponent<General>();
		gen = obj.GetGeneralJSON();
		Transform trans = e.GetComponent<Transform>();
		trans = obj.GetTransformJSON();
	}
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
		//ent.SetID(e.id); // no need for now
		General gen = e.GetComponent<General>();
		ent.SetGeneralJSON(gen);
		Transform tran = e.GetComponent<Transform>();
		ent.SetTransformJSON(tran);

		// push back after done
		entities.EntitiesList().push_back(ent);
	}
	entities.SerializeFile(filename);
}