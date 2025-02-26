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
        /// 获取当前激活关卡
        /// </summary>
        /// <returns></returns>
        std::weak_ptr<Level> getCurrentActiveLevel() const { return m_current_active_level; }

        /// <summary>
        /// 获取当前活动的物理场景
        /// </summary>
        std::weak_ptr<PhysicsScene> getCurrentActivePhysicsScene() const;

    private:
        bool loadWorld(const std::string& world_url);
        bool loadLevel(const std::string& level_url);

        /// <summary>
        /// 世界是否加载
        /// </summary>
        bool m_is_world_loaded {false}; 

        /// <summary>
        /// 当前世界的url
        /// </summary>
        std::string m_current_world_url;

        /// <summary>
        /// 当前世界资源
        /// </summary>
        std::shared_ptr<WorldRes> m_current_world_resource;

        // 所有加载过的关卡，键是关卡URL(路径)，值是对应的关卡实例
        std::unordered_map<std::string, std::shared_ptr<Level>> m_loaded_levels;

        // 当前激活的关卡，仅支持一个活跃关卡
        std::weak_ptr<Level> m_current_active_level;

        // 用于调试关卡的调试器
        std::shared_ptr<LevelDebugger> m_level_debugger;
    };
} // namespace Piccolo
