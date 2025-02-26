#include "runtime/function/input/input_system.h"

#include "core/base/macro.h"

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <GLFW/glfw3.h>

namespace Piccolo
{
    unsigned int k_complement_control_command = 0xFFFFFFFF;

    void InputSystem::onKey(int key, int scancode, int action, int mods)
    {
        if (!g_is_editor_mode) //是否处于编辑状态
        {
            onKeyInGameMode(key, scancode, action, mods);
        }
    }

    /// <summary>
    /// 处理按键输出
    /// </summary>
    void InputSystem::onKeyInGameMode(int key, int scancode, int action, int mods)
    {
        //确保在处理新的按键之前清除 jump 状态。
        m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::jump);

        if (action == GLFW_PRESS) //按下按键时
        {
            switch (key) //根据对应的按键设置m_game_command，将对应按键设置为1
            {
                case GLFW_KEY_ESCAPE:
                    // close();
                    break;
                case GLFW_KEY_R:
                    break;
                case GLFW_KEY_A:
                    m_game_command |= (unsigned int)GameCommand::left; 
                    break;
                case GLFW_KEY_S:
                    m_game_command |= (unsigned int)GameCommand::backward;
                    break;
                case GLFW_KEY_W:
                    m_game_command |= (unsigned int)GameCommand::forward;
                    break;
                case GLFW_KEY_D:
                    m_game_command |= (unsigned int)GameCommand::right;
                    break;
                case GLFW_KEY_SPACE:
                    m_game_command |= (unsigned int)GameCommand::jump;
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    m_game_command |= (unsigned int)GameCommand::squat;
                    break;
                case GLFW_KEY_LEFT_ALT: {
                    std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
                    window_system->setFocusMode(!window_system->getFocusMode());
                }
                break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_game_command |= (unsigned int)GameCommand::sprint;
                    break;
                case GLFW_KEY_F:
                    m_game_command ^= (unsigned int)GameCommand::free_carema;
                    break;
                default:
                    break;
            }
        }
        else if (action == GLFW_RELEASE) //按键释放时
        {
            switch (key)//根据对应的按键设置m_game_command，将对应按键设置为0
            {
                case GLFW_KEY_ESCAPE:
                    // close();
                    break;
                case GLFW_KEY_R:
                    break;
                case GLFW_KEY_W:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::forward);
                    break;
                case GLFW_KEY_S:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::backward);
                    break;
                case GLFW_KEY_A:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::left);
                    break;
                case GLFW_KEY_D:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::right);
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::squat);
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::sprint);
                    break;
                default:
                    break;
            }
        }
    }

    /// <summary>
    /// 计算鼠标的移动
    /// </summary>
    /// <param name="current_cursor_x"></param>
    /// <param name="current_cursor_y"></param>
    void InputSystem::onCursorPos(double current_cursor_x, double current_cursor_y)
    {
        if (g_runtime_global_context.m_window_system->getFocusMode())
        {
            m_cursor_delta_x = m_last_cursor_x - current_cursor_x;
            m_cursor_delta_y = m_last_cursor_y - current_cursor_y;
        }
        m_last_cursor_x = current_cursor_x;
        m_last_cursor_y = current_cursor_y;
    }

    /// <summary>
    /// 清理鼠标移动距离
    /// </summary>
    void InputSystem::clear()
    {
        m_cursor_delta_x = 0;
        m_cursor_delta_y = 0;
    }


    /// <summary>
    /// 根据鼠标位移距离计算移动弧度
    /// </summary>
    void InputSystem::calculateCursorDeltaAngles()
    {
        //获取窗口大小
        std::array<int, 2> window_size = g_runtime_global_context.m_window_system->getWindowSize();
        if (window_size[0] < 1 || window_size[1] < 1) //窗口未完全初始化
        {
            return;
        }

        //获取渲染相机
        std::shared_ptr<RenderCamera> render_camera = g_runtime_global_context.m_render_system->getRenderCamera();
        //获取摄像机的视野FOV
        const Vector2& fov = render_camera->getFOV();

        Radian cursor_delta_x(Math::degreesToRadians(m_cursor_delta_x));
        Radian cursor_delta_y(Math::degreesToRadians(m_cursor_delta_y));

        m_cursor_delta_yaw   = (cursor_delta_x / (float)window_size[0]) * fov.x;
        m_cursor_delta_pitch = -(cursor_delta_y / (float)window_size[1]) * fov.y;
    }

    void InputSystem::initialize()
    {
        //获取窗口系统对象
        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
        ASSERT(window_system);

        //注册按键函数  所以按键输入检测是在window_system中处理的
        window_system->registerOnKeyFunc(std::bind(&InputSystem::onKey,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3,
                                                   std::placeholders::_4));
        //注册鼠标光标位置回调
        window_system->registerOnCursorPosFunc(
            std::bind(&InputSystem::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
    }

    void InputSystem::tick()
    {
        //计算弧度
        calculateCursorDeltaAngles();
        //清空鼠标移动记录
        clear();

        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
        if (window_system->getFocusMode()) //如果是聚焦模式，则可以移动相机
        {
            m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::invalid);
        }
        else
        {
            m_game_command |= (unsigned int)GameCommand::invalid;
        }
    }
} // namespace Piccolo
