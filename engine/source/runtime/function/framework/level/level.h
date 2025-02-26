#pragma once

#include "runtime/function/framework/object/object_id_allocator.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Piccolo
{
    class Character;
    class GObject;
    class ObjectInstanceRes;
    class PhysicsScene;

    using LevelObjectsMap = std::unordered_map<GObjectID, std::shared_ptr<GObject>>;

    /// The main class to manage all game objects
    class Level
    {
    public:
        virtual ~Level(){};
        /// <summary>
        /// 根据资源路径加载当前level
        /// </summary>
        bool load(const std::string& level_res_url);

        void unload();

        bool save();

        void tick(float delta_time);

        /// <summary>
        /// 获取当前level的资源路径
        /// </summary>
        const std::string& getLevelResUrl() const { return m_level_res_url; }

        /// <summary>
        /// 返回level中的全部object
        /// </summary>
        const LevelObjectsMap& getAllGObjects() const { return m_gobjects; }

        /// <summary>
        /// 根据object_id获取物体
        /// </summary>
        std::weak_ptr<GObject> getGObjectByID(GObjectID go_id) const;

        /// <summary>
        /// 获取当前角色指针
        /// </summary>
        std::weak_ptr<Character> getCurrentActiveCharacter() const { return m_current_active_character; }

        /// <summary>
        /// 创建一个object
        /// </summary>
        /// <param name="object_instance_res">object的name、定义、其包含的全部组件</param>
        /// <returns></returns>
        GObjectID createObject(const ObjectInstanceRes& object_instance_res);

        /// <summary>
        /// 根据id删除object
        /// </summary>
        void deleteGObjectByID(GObjectID go_id);

        /// <summary>
        /// 返回level的物理场景
        /// </summary>
        std::weak_ptr<PhysicsScene> getPhysicsScene() const { return m_physics_scene; }

    protected:
        void clear();

        /// <summary>
        /// 当前场景是否被加载
        /// </summary>
        bool m_is_loaded {false};

        /// <summary>
        /// 当前场景的资源路径
        /// </summary>
        std::string m_level_res_url;

        /// <summary>
        /// 存储当前level中全部object：all game objects in this level, key: object id, value: object instance
        /// 是一个un_map<id,object>
        /// </summary>
        LevelObjectsMap m_gobjects;

        /// <summary>
        /// 当前关卡的角色
        /// </summary>
        std::shared_ptr<Character> m_current_active_character;

        /// <summary>
        /// 当前关卡的物理场景
        /// 管理当前level中全部object的物理碰撞，创建、删除、重力等
        /// </summary>
        std::weak_ptr<PhysicsScene> m_physics_scene;
    };
} // namespace Piccolo
