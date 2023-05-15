# Entity Component System

## Contents
1. [Introduction](#introduction)
   1. [Why is ECS Fast?](#why-is-ecs-fast)
   2. [The ECS Structure](#the-ecs-structure)
   3. [What is different about this ECS?](#what-is-different-about-this-ecs)


2. [Entity](#entity)

   1. [EntityID id](#entityid-id)
   2. [Destroy](#destroy)
   3. [Add Component](#addcomponent)
   4. [How to declate an entity?](#how-to-declare-an-entity)
   5. [How to assign components to entities?](#how-to-assign-components-to-entities)

3. [Component](#component)
   1. [Signature](#signature)
   2. [How to make a signature?](#how-to-make-a-signature)
   3. [How to make a component?](#how-to-make-a-component)

4. [System](#system)
   1. [How to declare a system?](#how-to-declare-a-system)
   2. [How to use the system?](#how-to-use-the-system)


<br>

---

## Introduction

Entity Component System (ECS) is the entity/gameObject architecture that we will be using for our game engine. It is chosen because it is fast and intuitive to use. 

This documentation will explain the usage and implementation of the system. After reading this documentation, you should be able to use the system to manage entities, components and systems.

To use the ECS, include "ECSManager.h" in your files.

<br>
<br>

### Why is ECS Fast?
Because it allows CPU to access information efficiently. To understand this, you need to know these two facts:

1. A cache is a small memory storage space that contains temporary information. 
2. It takes more performance power for CPU to access information outside the cache.

<br>

In ECS, all the entity and component data are tightly packed (eg. arrays). When CPU needs to do some operation on all component data, the cache is efficiently filled with it, since the data is tightly packed. In the example below, the CPU can access and operate with all 5 Component A swiftly.

> Component A | Component A | Component A | Component A | Component A 

<br>

In contrast to other methods which might take useless information and leave gaps. This fills up the cache, and only allow 3 Component A to be stored. As a result, the CPU has to spend additional processing power to access the other 2 Component A outside this cache.

> Component A | Data B | Component A | Component C | Component A 

<br>
<br>

### The ECS Structure

Here is a brief architechture explanation of ECS.

[Entity](#entity) is a struct containing an unique ID, which represents each entity.

[Component](#component) is a struct of data (eg. some floats, vec2, int), that an entity possesses. Each Entity contain 0 or more components which user can either add or remove from the entity. *Component is not stored inside Entity, it's managed by a manager somewhere else.*

[System](#system) is a global [smart pointer] to a class, encapsulating some function, perhaps data too. They operate on all entity's components that they are responsible for, all in one go. *Systems can also be selective in entities they operator on, it's up to you.*


<br>
<br>

### What is different about this ECS?

There are many ways to make ECS, but I have settled with this ECS that is based off [Austin Morlan's ECS](https://austinmorlan.com/posts/entity_component_system/) because of it's simplicity. There are some usage changes I made, as follows:

1. Entity is a struct that contains an EntityID and some member functions. 

    This makes the usage of my ECS more intuitive.

<br>

---

## Entity
Entity is a struct that contains the following. I will first explain each part of entity, then provide example of how to use it.

> EntityID id
>
> Destroy
>
> AddComponent 
>
> GetComponent

<br>
<br>

### EntityID id
This is just an unsigned 32-bits integer representing each Entities. Each entities have a unique id, assigned by the Entity Manager.

<br>

### Destroy

Function: `void Destroy()`

Usage: `entityObject.Destroy()`

Destroying the entity does 3 things:
1. Freeing the space it's components were occupying 
2. Freeing the EntityID so it can be reused. 
3. Removing the entity from all system it was registered to.

<br>

### AddComponent

Function: `template<typename T> void AddComponent(T component) const`

Usage: `entityObject.AddComponent<ComponentName>(componentObject)`

Assign a component to an entity and tell all appropriate systems to take note of this entity.<br>
*So that the systems will operate on it*.

<br>

### RemoveComponent

Function: `template<typename T> T& RemoveComponent() const`

Usage: `entityObject.RemoveComponent<ComponentName>(componentObject)`

Remove a component from an entity and tell the appropriate systems to take note of it.

<br>

### HasComponent
Function: `tmeplate<typename T> bool HasComponent() const`

Usage: `entityObject.HasComponent<ComponentName>()`

Check if an entity has a component and return a boolean value true if there is.

<br>
<br>

### How to declare an entity?

It's as simple as the following function:

`Entity entityObject {ECS::CreateEntity()};`

This is probably your first time encountering ECS namespace, so, what's ECS namespace? It's a namespace containing several useful function for implementing components and managing entities. 

Just declaring an empty entity isn't very useful, so next would be giving it components.

<br>
<br>

### How to assign components to entities?

Firstly, [make your component](#how-to-make-a-component). Then add it into the entity as such:

`ComponentName componentObject {declaration};`<br>
`entityObject.AddComponent<ComponentName>(componentObject);`

**Entities can only contain one of each components.** 

<br>

---

## Component

Component is a struct of data (eg. float, vec2, int), that an entity possesses. Each entity can only hold one of each conponent, thus there is a special id for each component too, we call it signature bit. 

<br>
<br>

### Signature
Signature is a bitset (eg. 01100101001) which determines which component the entity possesses and which components a system can operate on. 0 means a certain component is not used, 1 means it is being used, for an entity. 

For example, a bitset of 10100 represents an entity is using component with signature bit 10000 and 100. 

<br>

### How to make a signature?

Firstly, declare an object of type Signature. Then set it's bitset with the following function:

`signatureObject.set(ECS::GetComponentType<ComponentName>())`

You can add as many unique components as you like. This signature will be useful for telling systems what components they can operate on.

<br>
<br>

### How to make a component?
To make a component, you have to make the component's struct and register it into the system.

1. Go to ECS_components.h and make your struct of component.
2. Go to ECSManager.cpp > `ECSManager::RegisterAllComponents()` and add one more line following this format to register your component: `ECS::RegisterComponent<ComponentName>();`

<br>

---

## System

System itself is just a **singleton** class with a set of entities. System object is more useful, it is a global smart pointer to a class, encapsulating some function and perhaps data too. They contain a set of Entity which indicates the entity they are allowed to operate on. Needless to say, you CAN have functions that do not use any entity as well. Now, system's declaration and definition is a little complicated since it's usage is vast and flexible, bare with me.

Just a side note, all systems should only be defined once, in ECSManager.cpp
<br>
<br>

### How to declare a system?
To declare a system, you will have to declare its class, shared_ptr, make a register manager and register it. Let me walk you through it. 

1. Declare and define a class for your system.
   
   Go to ECS_systems.h and make a class inheriting from `class System`. 
   Make another .cpp file and inherit from ECS_system.h. Define everything your class has in that file.

<br>

2. Declare and define your class's global [shared pointer](#smart-pointer).
   
   Go to ECSManager.cpp and define an object for your system as such:

   `std::shared_ptr<SystemName> systemObject{ nullptr };`

   Go to ECSManager.h and declare a global [shared pointer](#smart-pointer) as such:

   `extern std::shared_ptr<SystemName> systemObject;`

<br>

3. Make a system register function for your system.
   
   Go to ECSManager.h and declare a static member function for registering your manager as such:
   
   `static void RegisterSystemName();`

   Then define it in ECSManager.cpp. The definition should do two things.

   Firstly, [define the signature](#how-to-make-a-signature). Then assign it to the system with the following function:

   `ECS::SetSystemSignature<SystemName>(signatureObject);`

   Secondly, assign the registered system to it's global shared pointer as such:

   `systemObject = ECS::RegisterSystem<SystemName>();`

<br>

4. Register the system.

   Go to ECSManager.cpp > `ECSManager::RegisterAllSystems()`, add in the following code:<br>
   `RegisterSystemName();`

<br>
<br>

### How to use the system?

With all the member functions in system, you can call them individually anywhere. For example, I declared `lifespanManager` and I want to use it's member functions. I can do `lifespanManager->Update();`

Treat these functions as normal functions encapsulated in a class. The only benefit it bring is it contain a set of entities it can operate on. So a useful usage would be looping through each entity and operating on them using `for (Entity e : mEntities) {}`

You can keep member variables in the class too, for data tracking. In conclusion, **treat it like a normal singleton with added benefit** that it contains bunch of relevant entities.

<br>

---

## Extra
Here are some additional materials that are useful but does not fit any of the category above.

<br>
<br>

### Smart pointer
The smart pointer this ECS would be using are both unique_ptr and shared_ptr. You will only need to briefly touch on shared_ptr.

<br>

<u>What is shared_ptr?</u>

I have a shared pointer pointing at an object. I can declare multiple other shared pointer to point at it as well. That is shared pointer, difference from unique pointer is, unique pointer can only have one pointer pointing at the object.

The benefit of shared pointer is we do not have to worry about memory leak. When the object goes out of scope, shared pointer will auto deconstruct the object, freeing the memory. This is especially useful for global objects, we just declare it, and forget about deallocating it, stress-free worry-free. 

**Still don't know what Shared pointer is?**

It's alright, just follow the steps in this documentations and you will be fine.

<br>
<br>

### Additional Functions in ECS Namespace

Here are some useful ECS functions I have made. There is probably more, you can request it from me and I'll see to it.

<br>

`void DestroyAllEntities()`
Basically destroy all entities

`void DestroySomeEntites(const std::vector<EntityID>& dontDestroy)` Destroys all entities except specified in the vector.

<br>

---

## Warning

<br>

### Avoid using static function in system
Since all systems are singletons, there should be no need for static functions. Furthermore, static functions cannot access mEntities from class System because mEntities is a non-static member variable.




