#pragma once

#include "runtime/resource/res_type/common/world.h"

#include <filesystem>
#include <string>

namespace Piccolo
{
    class Level;
    class LevelDebugger;
    class PhysicsScene;

    /// Manage all game worlds, it should be support multiple worlds, including game world and editor world.
    /// Currently, the implement just supports one active world and one active level
    class WorldManager
    {
    public:
        virtual ~WorldManager();

        void initialize();
        void clear();

        void reloadCurrentLevel();
        void saveCurrentLevel();

        void tick(float delta_time);

        /// <summary>
        /// ��ȡ��ǰ����ؿ�
        /// </summary>
        /// <returns></returns>
        std::weak_ptr<Level> getCurrentActiveLevel() const { return m_current_active_level; }

        /// <summary>
        /// ��ȡ��ǰ���������
        /// </summary>
        std::weak_ptr<PhysicsScene> getCurrentActivePhysicsScene() const;

    private:
        bool loadWorld(const std::string& world_url);
        bool loadLevel(const std::string& level_url);

        /// <summary>
        /// �����Ƿ����
        /// </summary>
        bool m_is_world_loaded {false}; 

        /// <summary>
        /// ��ǰ�����url
        /// </summary>
        std::string m_current_world_url;

        /// <summary>
        /// ��ǰ������Դ
        /// </summary>
        std::shared_ptr<WorldRes> m_current_world_resource;

        // ���м��ع��Ĺؿ������ǹؿ�URL(·��)��ֵ�Ƕ�Ӧ�Ĺؿ�ʵ��
        std::unordered_map<std::string, std::shared_ptr<Level>> m_loaded_levels;

        // ��ǰ����Ĺؿ�����֧��һ����Ծ�ؿ�
        std::weak_ptr<Level> m_current_active_level;

        // ���ڵ��Թؿ��ĵ�����
        std::shared_ptr<LevelDebugger> m_level_debugger;
    };
} // namespace Piccolo
