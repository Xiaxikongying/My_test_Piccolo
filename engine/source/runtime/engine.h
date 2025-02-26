#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace Piccolo
{
    /// <summary>
    /// �Ƿ��ڱ༭ģʽ
    /// </summary>
    extern bool g_is_editor_mode;

    extern std::unordered_set<std::string> g_editor_tick_component_types;

    class PiccoloEngine
    {
        friend class PiccoloEditor;
        static const float s_fps_alpha;

    public:
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="config_file_path">�����ļ�·��</param>
        void startEngine(const std::string& config_file_path);

        /// <summary>
        /// �ر�����
        /// </summary>
        void shutdownEngine();

        /// <summary>
        /// ���к���
        /// </summary>
        void run();

        /// <summary>
        /// ֡���º���
        /// </summary>
        /// <param name="delta_time"></param>
        /// <returns></returns>
        bool tickOneFrame(float delta_time);

        /// <summary>
        /// ���ص�ǰ�Ƿ��˳�״̬
        /// </summary>
        bool isQuit() const { return m_is_quit; }

        /// <summary>
        /// ��ȡ��ǰ֡��
        /// </summary>
        int getFPS() const { return m_fps; }

        /// <summary>
        /// initialize��clear�ǿ�ʵ��
        /// </summary>
        void initialize();
        void clear();

    protected:
        /// <summary>
        /// �߼�����
        /// </summary>
        void logicalTick(float delta_time);

        /// <summary>
        /// ��Ⱦ����
        /// </summary>
        /// <returns></returns>
        bool rendererTick(float delta_time);

        /// <summary>
        /// ����֡��
        /// </summary>
        void calculateFPS(float delta_time);

   
        /// <summary>
        /// ��ȡ����ʱ�䣬��ÿ֡��ʱ���
        /// </summary>
        float calculateDeltaTime();

    protected:
        /// <summary>
        /// �Ƿ��˳�
        /// </summary>
        bool m_is_quit {false};

        /// <summary>
        /// ��ȡ��������ʱ��ʱ��
        /// </summary>
        std::chrono::steady_clock::time_point m_last_tick_time_point {std::chrono::steady_clock::now()};

        /// <summary>
        /// һ֡��ռ�е�ʱ�䣨ƽ��ֵ��
        /// </summary>
        float m_average_duration {0.f};

        /// <summary>
        /// ���е���֡��
        /// </summary>
        int m_frame_count {0};

        /// <summary>
        /// ֡��
        /// </summary>
        int m_fps {0};
    };
} // namespace Piccolo
