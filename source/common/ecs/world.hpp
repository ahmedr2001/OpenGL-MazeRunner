#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our
{

    // This class holds a set of entities
    class World
    {
        std::unordered_set<Entity *> entities;         // These are the entities held by this world
        std::unordered_set<Entity *> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                       // when deleteMarkedEntities is called
    public:
        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json &data, Entity *parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity *add()
        {
            // TODO: (Req 8) Create a new entity, set its world member variable to this,
            //  and don't forget to insert it in the suitable container.

            // Create a new entity
            Entity *newEntity = new Entity();
            // set its world member variable to this
            newEntity->world = this;
            // insert it in the suitable container
            this->entities.insert(newEntity);

            return newEntity;
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity *> &getEntities()
        {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity *entity)
        {
            // TODO: (Req 8) If the entity is in this world, add it to the "markedForRemoval" set.
            for (auto it = this->entities.begin(); it != this->entities.end(); it++)
            {
                if (*it == entity)
                {
                    // Mark an entity for removal by adding it to the "markedForRemoval" set
                    this->markedForRemoval.insert(entity);
                    // EXPECT_ERROR!: Should i remove it from the entities list here?
                    // this->entities.erase(it);
                    return;
                }
            }
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities()
        {
            // TODO: (Req 8) Remove and delete all the entities that have been marked for removal
            for (auto it = this->markedForRemoval.begin(); it != this->markedForRemoval.end(); it++)
            {
                // Remove the elements in "markedForRemoval" from the "entities" set.
                if (entities.find(*it) != entities.end())
                    this->entities.erase(it);
                // Delete marked entity
                delete *it;
            }
            // clear markedForRemoval entites list
            this->markedForRemoval.clear();
        }

        // This deletes all entities in the world
        void clear()
        {
            // TODO: (Req 8) Delete all the entites and make sure that the containers are empty
            // Delete marked entites
            this->deleteMarkedEntities();
            // Delete entites
            for (auto it = this->entities.begin(); it != this->entities.end(); it++)
            {
                // Delete marked entity
                delete *it;
            }
            // Clear entites list
            this->entities.clear();
        }

        // Since the world owns all of its entities, they should be deleted alongside it.
        ~World()
        {
            clear();
        }

        // The world should not be copyable
        World(const World &) = delete;
        World &operator=(World const &) = delete;
    };

}