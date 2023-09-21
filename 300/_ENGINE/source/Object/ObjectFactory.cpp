#include "Object/ObjectFactory.h"
#include "Serialization/SerializationTemp.h"
#include "document.h"

void ObjectFactory::DeserializeScene(const std::string& filename)
{
	// loop thru container and store entities

	EntityListJSON entities;
	entities.DeserializeFile(filename);

	std::unordered_map<entt::entity, entt::entity> idMap;

	for (auto& obj : entities.EntitiesList())
	{
		Entity e = systemManager->ecs->NewEntity();

		// (oldid, newid)
		idMap.insert({ obj.GetIDJSON(), e.id });

		e.GetComponent<General>() = obj.GetGeneralJSON();
		e.GetComponent<Transform>() = obj.GetTransformJSON();

		if (obj.mrb_t)
		{
			e.AddComponent<RigidBody>();
			e.GetComponent<RigidBody>() = obj.GetRBJSON();
		}

		if (obj.mmr_t)
		{
			e.AddComponent<MeshRenderer>();
			e.GetComponent<MeshRenderer>() = obj.GetMRJSON();
		}

		if (obj.mbc_t)
		{
			e.AddComponent<BoxCollider>();
			e.GetComponent<BoxCollider>() = obj.GetBCJSON();
		}

		if (obj.msc_t)
		{
			e.AddComponent<SphereCollider>();
			e.GetComponent<SphereCollider>() = obj.GetSCJSON();
		}

		if (obj.mpc_t)
		{
			e.AddComponent<PlaneCollider>();
			e.GetComponent<PlaneCollider>() = obj.GetPCJSON();
		}

		if (obj.ms_t)
		{
			e.AddComponent<Scripts>();
			e.GetComponent<Scripts>() = obj.GetSJSON();
			/*for (Script ya : e.GetComponent<Scripts>().scriptsContainer)
				std::cout << ya.scriptFile << std::endl;*/
		}

		if (obj.mp_t)
		{
			e.AddComponent<Parent>();
			e.GetComponent<Parent>() = obj.GetPJSON();
		}

		if (obj.mc_t)
		{
			e.AddComponent<Children>();
			e.GetComponent<Children>() = obj.GetCJSON();
		}

		if (obj.ma_t)
		{
			e.AddComponent<Audio>();
			e.GetComponent<Audio>() = obj.GetAJSON();
		}

		// weird stuff, why still have value in rb when the component does not exist?
		//std::cout << e.GetComponent<General>().name << " " << obj.mrb_t << " " << e.GetComponent<RigidBody>().mMass << std::endl;
	}

	auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
	auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();

	// working
	std::cout << "containers: " << parent_cont.size() << " " << child_cont.size() << std::endl;

	for (Entity pe : parent_cont)
	{
		Parent& parent = pe.GetComponent<Parent>();
		std::cout << "old parent stuff: " << parent.mNextSibling << ", " << parent.mParent << ", " << parent.mPrevSibling << std::endl;
		parent.mNextSibling = (uint32_t)idMap[(entt::entity)parent.mNextSibling];
		parent.mParent = (uint32_t)idMap[(entt::entity)parent.mParent];
		parent.mPrevSibling = (uint32_t)idMap[(entt::entity)parent.mPrevSibling];

		std::cout << "new parent stuff: " << parent.mNextSibling << ", " << parent.mParent << ", " << parent.mPrevSibling << std::endl;
	}

	for (Entity ce : child_cont)
	{
		for (const auto& c : idMap)
		{
			Children& child = ce.GetComponent<Children>();
			std::cout << "old child stuff: " << child.mFirstChild << std::endl;
			child.mFirstChild = (uint32_t)idMap[(entt::entity)child.mFirstChild];
			std::cout << "new child stuff: " << child.mFirstChild << std::endl;
		}
	}

	// working
	for (const auto& n : idMap)
	{
		std::cout << "old id: " << (std::uint32_t)n.first << ", new id: " << (std::uint32_t)n.second << std::endl;
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

	auto container = systemManager->ecs->GetEntitiesWith<General, Transform>();

	for (Entity e : container)
	{
		// basic stuff that must be set
		std::string name = e.GetComponent<General>().name;
		ent.SetIDJSON(e.id);
		ent.SetGeneralJSON(e.GetComponent<General>());
		ent.SetTransformJSON(e.GetComponent<Transform>());

		if (e.HasComponent<RigidBody>())
			ent.SetRBJSON(e.GetComponent<RigidBody>());
		else ent.mrb_t = false;

		if (e.HasComponent<MeshRenderer>())
			ent.SetMRJSON(e.GetComponent<MeshRenderer>());
		else ent.mmr_t = false;

		if (e.HasComponent<BoxCollider>())
			ent.SetBCJSON(e.GetComponent<BoxCollider>());
		else ent.mbc_t = false;

		if (e.HasComponent<SphereCollider>())
			ent.SetSCJSON(e.GetComponent<SphereCollider>());
		else ent.msc_t = false;

		if (e.HasComponent<PlaneCollider>())
			ent.SetPCJSON(e.GetComponent<PlaneCollider>());
		else ent.mpc_t = false;

		if (e.HasComponent<Scripts>())
			ent.SetSJSON(e.GetComponent<Scripts>());
		else ent.ms_t = false;

		if (e.HasComponent<Parent>())
			ent.SetPJSON(e.GetComponent<Parent>());
		else ent.mp_t = false;

		if (e.HasComponent<Children>())
			ent.SetCJSON(e.GetComponent<Children>());
		else ent.mc_t = false;

		if (e.HasComponent<Audio>())
			ent.SetAJSON(e.GetComponent<Audio>());
		else ent.ma_t = false;

		// push back after done
		entities.EntitiesList().push_back(ent);
	}

	entities.SerializeFile(filename);
}

void ObjectFactory::SerializePrefab(Entity e, const std::string& filename)
{
	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);
	ofs.close();

	EntityJSON ent;
	EntityListJSON entities;
	entities.EntitiesList().clear();

	// basic stuff that must be set
	ent.SetIDJSON(e.id);
	ent.SetGeneralJSON(e.GetComponent<General>());
	ent.SetTransformJSON(e.GetComponent<Transform>());

	// basic stuff that must be set
	std::string name = e.GetComponent<General>().name;
	ent.SetIDJSON(e.id);
	ent.SetGeneralJSON(e.GetComponent<General>());
	ent.SetTransformJSON(e.GetComponent<Transform>());

	if (e.HasComponent<RigidBody>())
		ent.SetRBJSON(e.GetComponent<RigidBody>());
	else ent.mrb_t = false;

	if (e.HasComponent<MeshRenderer>())
		ent.SetMRJSON(e.GetComponent<MeshRenderer>());
	else ent.mmr_t = false;

	if (e.HasComponent<BoxCollider>())
		ent.SetBCJSON(e.GetComponent<BoxCollider>());
	else ent.mbc_t = false;

	if (e.HasComponent<SphereCollider>())
		ent.SetSCJSON(e.GetComponent<SphereCollider>());
	else ent.msc_t = false;

	if (e.HasComponent<PlaneCollider>())
		ent.SetPCJSON(e.GetComponent<PlaneCollider>());
	else ent.mpc_t = false;

	if (e.HasComponent<Scripts>())
		ent.SetSJSON(e.GetComponent<Scripts>());
	else ent.ms_t = false;

	if (e.HasComponent<Parent>())
		ent.SetPJSON(e.GetComponent<Parent>());
	else ent.mp_t = false;

	if (e.HasComponent<Children>())
		ent.SetCJSON(e.GetComponent<Children>());
	else ent.mc_t = false;

	if (e.HasComponent<Audio>())
		ent.SetAJSON(e.GetComponent<Audio>());
	else ent.ma_t = false;

	// push back after done
	entities.EntitiesList().push_back(ent);

	entities.SerializeFile(filename);
}