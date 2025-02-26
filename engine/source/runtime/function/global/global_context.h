#pragma once

#include <memory>
#include <string>

namespace Piccolo
{
    class LogSystem;
    class InputSystem;
    class PhysicsManager;
    class FileSystem;
    class AssetManager;
    class ConfigManager;
    class WorldManager;
    class RenderSystem;
    class WindowSystem;
    class ParticleManager;
    class DebugDrawManager;
    class RenderDebugConfig;
    struct EngineInitParams;

    /// <summary>
    /// 管理引擎的全部系统，负责初始化和关闭
    ///  Manage the lifetime and creation/destruction order of all global system
    /// </summary>
    class RuntimeGlobalContext
    {
    public:
        /// <summary>
        /// 初始化全部的功能系统
        /// create all global systems and initialize these systems
        /// </summary>
        /// <param name="config_file_path">配置文件路径</param>
        void startSystems(const std::string& config_file_path);

        /// <summary>
        /// 关闭全部系统  destroy all global systems
        /// </summary>
        void shutdownSystems();

    public:
        std::shared_ptr<LogSystem>         m_logger_system;     //日志
        std::shared_ptr<InputSystem>       m_input_system;      //输入
        std::shared_ptr<FileSystem>        m_file_system;       //文件
        std::shared_ptr<AssetManager>      m_asset_manager;     //资源
        std::shared_ptr<ConfigManager>     m_config_manager;    //配置
        std::shared_ptr<WorldManager>      m_world_manager;     //世界
        std::shared_ptr<PhysicsManager>    m_physics_manager;   //物理
        std::shared_ptr<WindowSystem>      m_window_system;     //窗口
        std::shared_ptr<RenderSystem>      m_render_system;     //渲染
        std::shared_ptr<ParticleManager>   m_particle_manager;  //粒子
        std::shared_ptr<DebugDrawManager>  m_debugdraw_manager; //调试
        std::shared_ptr<RenderDebugConfig> m_render_debug_config;   //渲染调试配置
    };

    extern RuntimeGlobalContext g_runtime_global_context; //全局的运行上下文
} // namespace Piccolo