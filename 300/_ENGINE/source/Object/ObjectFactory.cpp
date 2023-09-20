#include "Object/ObjectFactory.h"
#include "Serialization/SerializationTemp.h"
#include "document.h"

void ObjectFactory::DeserializeScene(const std::string& filename)
{
	// loop thru container and store entities

	EntityListJSON entities;
	entities.DeserializeFile(filename);

	//unordered map <oldid, newid> map

	//for (all entties)
	//{
	//    except for parent child or any component that stores an entity

	//    deserialized_eid = deserialize();
	//    newid = newentity()
	//    map[derialized_eid] = newid
	//}
	//for (entities with parent)
	//{
	//    newid = map[id in component]
	//    parent.sibling = newid;
	//}
	//for (entities with child)
	//{

	//}

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
	//std::cout << "containers: " << parent_cont.size() << " " << child_cont.size() << std::endl;

	for (Entity pe : parent_cont)
	{
		for (const auto& p : idMap)
		{
			// if the next sibling's old id matches the old id in the list
			if (pe.GetComponent<Parent>().mNextSibling == (std::uint32_t)p.first)
			{
				// update the next sibling's id with the new id
				pe.GetComponent<Parent>().mNextSibling = (std::uint32_t)p.second;
			}

			if (pe.GetComponent<Parent>().mParent == (std::uint32_t)p.first)
			{
				pe.GetComponent<Parent>().mParent = (std::uint32_t)p.second;
			}

			if (pe.GetComponent<Parent>().mPrevSibling == (std::uint32_t)p.first)
			{
				pe.GetComponent<Parent>().mPrevSibling = (std::uint32_t)p.second;
			}
		}
	}

	for (Entity ce : child_cont)
	{
		for (const auto& c : idMap)
		{
			// if the child's old id matches the old id in the list
			if (ce.GetComponent<Children>().mFirstChild == (std::uint32_t)c.first)
			{
				// update the child's id with the new id
				ce.GetComponent<Children>().mFirstChild = (std::uint32_t)c.second;
			}

			// clarify
			/*if (ce.GetComponent<Children>().mNumChildren == (std::uint32_t)c.first)
			{
				ce.GetComponent<Children>().mNumChildren = (std::uint32_t)c.second;
			}*/
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
		ent.SetIDJSON(e.id);
		ent.SetGeneralJSON(e.GetComponent<General>());
		ent.SetTransformJSON(e.GetComponent<Transform>());

		if (e.HasComponent<RigidBody>())
			ent.SetRBJSON(e.GetComponent<RigidBody>());

		if (e.HasComponent<BoxCollider>())
			ent.SetBCJSON(e.GetComponent<BoxCollider>());

		if (e.HasComponent<SphereCollider>())
			ent.SetSCJSON(e.GetComponent<SphereCollider>());

		if (e.HasComponent<PlaneCollider>())
			ent.SetPCJSON(e.GetComponent<PlaneCollider>());

		if (e.HasComponent<Scripts>())
			ent.SetSJSON(e.GetComponent<Scripts>());

		if (e.HasComponent<Parent>())
			ent.SetPJSON(e.GetComponent<Parent>());

		if (e.HasComponent<Children>())
			ent.SetCJSON(e.GetComponent<Children>());

		if (e.HasComponent<Audio>())
			ent.SetAJSON(e.GetComponent<Audio>());

		// push back after done
		entities.EntitiesList().push_back(ent);
	}

	entities.SerializeFile(filename);
}