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
        /// ������Դ·�����ص�ǰlevel
        /// </summary>
        bool load(const std::string& level_res_url);

        void unload();

        bool save();

        void tick(float delta_time);

        /// <summary>
        /// ��ȡ��ǰlevel����Դ·��
        /// </summary>
        const std::string& getLevelResUrl() const { return m_level_res_url; }

        /// <summary>
        /// ����level�е�ȫ��object
        /// </summary>
        const LevelObjectsMap& getAllGObjects() const { return m_gobjects; }

        /// <summary>
        /// ����object_id��ȡ����
        /// </summary>
        std::weak_ptr<GObject> getGObjectByID(GObjectID go_id) const;

        /// <summary>
        /// ��ȡ��ǰ��ɫָ��
        /// </summary>
        std::weak_ptr<Character> getCurrentActiveCharacter() const { return m_current_active_character; }

        /// <summary>
        /// ����һ��object
        /// </summary>
        /// <param name="object_instance_res">object��name�����塢�������ȫ�����</param>
        /// <returns></returns>
        GObjectID createObject(const ObjectInstanceRes& object_instance_res);

        /// <summary>
        /// ����idɾ��object
        /// </summary>
        void deleteGObjectByID(GObjectID go_id);

        /// <summary>
        /// ����level��������
        /// </summary>
        std::weak_ptr<PhysicsScene> getPhysicsScene() const { return m_physics_scene; }

    protected:
        void clear();

        /// <summary>
        /// ��ǰ�����Ƿ񱻼���
        /// </summary>
        bool m_is_loaded {false};

        /// <summary>
        /// ��ǰ��������Դ·��
        /// </summary>
        std::string m_level_res_url;

        /// <summary>
        /// �洢��ǰlevel��ȫ��object��all game objects in this level, key: object id, value: object instance
        /// ��һ��un_map<id,object>
        /// </summary>
        LevelObjectsMap m_gobjects;

        /// <summary>
        /// ��ǰ�ؿ��Ľ�ɫ
        /// </summary>
        std::shared_ptr<Character> m_current_active_character;

        /// <summary>
        /// ��ǰ�ؿ���������
        /// ����ǰlevel��ȫ��object��������ײ��������ɾ����������
        /// </summary>
        std::weak_ptr<PhysicsScene> m_physics_scene;
    };
} // namespace Piccolo
