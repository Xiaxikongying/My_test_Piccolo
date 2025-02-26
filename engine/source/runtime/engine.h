#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace Piccolo
{
    /// <summary>
    /// 是否处于编辑模式
    /// </summary>
    extern bool g_is_editor_mode;

    extern std::unordered_set<std::string> g_editor_tick_component_types;

    class PiccoloEngine
    {
        friend class PiccoloEditor;
        static const float s_fps_alpha;

    public:
        /// <summary>
        /// 启动引擎
        /// </summary>
        /// <param name="config_file_path">配置文件路径</param>
        void startEngine(const std::string& config_file_path);

        /// <summary>
        /// 关闭引擎
        /// </summary>
        void shutdownEngine();

        /// <summary>
        /// 运行函数
        /// </summary>
        void run();

        /// <summary>
        /// 帧更新函数
        /// </summary>
        /// <param name="delta_time"></param>
        /// <returns></returns>
        bool tickOneFrame(float delta_time);

        /// <summary>
        /// 返回当前是否退出状态
        /// </summary>
        bool isQuit() const { return m_is_quit; }

        /// <summary>
        /// 获取当前帧数
        /// </summary>
        int getFPS() const { return m_fps; }

        /// <summary>
        /// initialize和clear是空实现
        /// </summary>
        void initialize();
        void clear();

    protected:
        /// <summary>
        /// 逻辑更新
        /// </summary>
        void logicalTick(float delta_time);

        /// <summary>
        /// 渲染更新
        /// </summary>
        /// <returns></returns>
        bool rendererTick(float delta_time);

        /// <summary>
        /// 计算帧率
        /// </summary>
        void calculateFPS(float delta_time);

   
        /// <summary>
        /// 获取增量时间，即每帧的时间差
        /// </summary>
        float calculateDeltaTime();

    protected:
        /// <summary>
        /// 是否退出
        /// </summary>
        bool m_is_quit {false};

        /// <summary>
        /// 获取引擎运行时的时间
        /// </summary>
        std::chrono::steady_clock::time_point m_last_tick_time_point {std::chrono::steady_clock::now()};

        /// <summary>
        /// 一帧所占有的时间（平均值）
        /// </summary>
        float m_average_duration {0.f};

        /// <summary>
        /// 运行的总帧数
        /// </summary>
        int m_frame_count {0};

        /// <summary>
        /// 帧率
        /// </summary>
        int m_fps {0};
    };
} // namespace Piccolo
