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
    /// ���������ȫ��ϵͳ�������ʼ���͹ر�
    ///  Manage the lifetime and creation/destruction order of all global system
    /// </summary>
    class RuntimeGlobalContext
    {
    public:
        /// <summary>
        /// ��ʼ��ȫ���Ĺ���ϵͳ
        /// create all global systems and initialize these systems
        /// </summary>
        /// <param name="config_file_path">�����ļ�·��</param>
        void startSystems(const std::string& config_file_path);

        /// <summary>
        /// �ر�ȫ��ϵͳ  destroy all global systems
        /// </summary>
        void shutdownSystems();

    public:
        std::shared_ptr<LogSystem>         m_logger_system;     //��־
        std::shared_ptr<InputSystem>       m_input_system;      //����
        std::shared_ptr<FileSystem>        m_file_system;       //�ļ�
        std::shared_ptr<AssetManager>      m_asset_manager;     //��Դ
        std::shared_ptr<ConfigManager>     m_config_manager;    //����
        std::shared_ptr<WorldManager>      m_world_manager;     //����
        std::shared_ptr<PhysicsManager>    m_physics_manager;   //����
        std::shared_ptr<WindowSystem>      m_window_system;     //����
        std::shared_ptr<RenderSystem>      m_render_system;     //��Ⱦ
        std::shared_ptr<ParticleManager>   m_particle_manager;  //����
        std::shared_ptr<DebugDrawManager>  m_debugdraw_manager; //����
        std::shared_ptr<RenderDebugConfig> m_render_debug_config;   //��Ⱦ��������
    };

    extern RuntimeGlobalContext g_runtime_global_context; //ȫ�ֵ�����������
} // namespace Piccolo