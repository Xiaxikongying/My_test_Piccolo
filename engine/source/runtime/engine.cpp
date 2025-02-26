#include "runtime/engine.h"

#include "runtime/core/base/macro.h"
#include "runtime/core/meta/reflection/reflection_register.h"

#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/particle/particle_manager.h"
#include "runtime/function/physics/physics_manager.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"
#include "runtime/function/render/debugdraw/debug_draw_manager.h"

namespace Piccolo 
{
    /// <summary>
    /// 存储编辑器的运行模式（编辑/运行等...）
    /// </summary>
    bool g_is_editor_mode {false};

    /// <summary>
    /// 用于保存编辑器的组件
    /// </summary>
    std::unordered_set<std::string> g_editor_tick_component_types {};


    void PiccoloEngine::startEngine(const std::string& config_file_path)
    {
        //注册反射
        Reflection::TypeMetaRegister::metaRegister();
        //初始化全部的功能系统
        g_runtime_global_context.startSystems(config_file_path);
        LOG_INFO("engine start");
    }

    void PiccoloEngine::shutdownEngine()
    {
        LOG_INFO("engine shutdown");
        //销毁资源
        g_runtime_global_context.shutdownSystems();
        //关闭反射
        Reflection::TypeMetaRegister::metaUnregister();
    }

    void PiccoloEngine::initialize() {}
    void PiccoloEngine::clear() {}

    void PiccoloEngine::run()
    {
        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
        ASSERT(window_system);

        while (!window_system->shouldClose()) //窗口是否关闭
        {
            const float delta_time = calculateDeltaTime();
            tickOneFrame(delta_time);
        }
    }

    /// <summary>
    /// 获取增量时间，即每帧的时间差
    /// </summary>
    float PiccoloEngine::calculateDeltaTime()
    {
        float delta_time;
        {
            using namespace std::chrono;
            // 获取当前时间
            steady_clock::time_point tick_time_point = steady_clock::now();
            //计算与上次获取的时间的差
            duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
            delta_time  = time_span.count();

            m_last_tick_time_point = tick_time_point;
        }
        return delta_time;
    }

    /// <summary>
    /// 帧更新
    /// </summary>
    /// <param name="delta_time">距离上一帧的时间，使得游戏逻辑的更新不依赖于具体的帧率，能够保持一致的表现</param>
    bool PiccoloEngine::tickOneFrame(float delta_time)
    {
        //逻辑帧更新
        logicalTick(delta_time);

        //计算fps
        calculateFPS(delta_time);

        // single thread
        // 交换逻辑与渲染之间的数据
        g_runtime_global_context.m_render_system->swapLogicRenderData();

        //渲染帧更新
        rendererTick(delta_time);

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
        g_runtime_global_context.m_physics_manager->renderPhysicsWorld(delta_time);
#endif

        //处理和检查来自操作系统的所有窗口事件 （鼠标点击、键盘按键等）
        g_runtime_global_context.m_window_system->pollEvents();

        //在窗口上显示帧数
        g_runtime_global_context.m_window_system->setTitle(
            std::string("Piccolo - " + std::to_string(getFPS()) + " FPS").c_str());

        //返回 !(窗口是否关闭)
        const bool should_window_close = g_runtime_global_context.m_window_system->shouldClose();
        return !should_window_close;
    }

    void PiccoloEngine::logicalTick(float delta_time)
    {
        //更新世界
        g_runtime_global_context.m_world_manager->tick(delta_time);
        //更新输入系统
        g_runtime_global_context.m_input_system->tick();
    }

    bool PiccoloEngine::rendererTick(float delta_time)
    {
        //更新渲染系统
        g_runtime_global_context.m_render_system->tick(delta_time);
        return true;
    }
     
    //s_fps_alpha: FPS 平滑的加权系数  防止某些特别的帧导致fps剧烈变化
    const float PiccoloEngine::s_fps_alpha = 1.f / 100;
    void PiccoloEngine::calculateFPS(float delta_time)
    {
        m_frame_count++;

        if (m_frame_count == 1)
        {
            m_average_duration = delta_time;
        }
        else
        {
            m_average_duration = m_average_duration * (1 - s_fps_alpha) + delta_time * s_fps_alpha;
        }
        m_fps = static_cast<int>(1.f / m_average_duration);
    }
} // namespace Piccolo
