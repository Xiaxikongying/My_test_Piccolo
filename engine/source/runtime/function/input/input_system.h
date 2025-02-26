#pragma once

#include "runtime/core/math/math.h"

namespace Piccolo
{
    enum class GameCommand : unsigned int
    {
        forward  = 1 << 0,                 // W
        backward = 1 << 1,                 // S
        left     = 1 << 2,                 // A
        right    = 1 << 3,                 // D
        jump     = 1 << 4,                 // SPACE
        squat    = 1 << 5,                 // not implemented yet
        sprint   = 1 << 6,                 // LEFT SHIFT
        fire     = 1 << 7,                 // not implemented yet
        free_carema = 1 << 8,              // F
        invalid  = (unsigned int)(1 << 31) // lost focus
    };

    extern unsigned int k_complement_control_command;

    class InputSystem
    {

    public:
        /// <summary>
        /// 处理键盘事件
        /// </summary>
        void onKey(int key, int scancode, int action, int mods);

        /// <summary>
        /// 计算鼠标位移距离
        /// </summary>
        /// <param name="current_cursor_x"></param>
        /// <param name="current_cursor_y"></param>
        void onCursorPos(double current_cursor_x, double current_cursor_y);

        void initialize();
        void tick();
        void clear();

        /// <summary>
        /// 记录鼠标当前的位移，通过m_last_cursor和current_cursor可计算出
        /// </summary>
        int m_cursor_delta_x {0};
        int m_cursor_delta_y {0};

        /// <summary>
        /// 根据鼠标的位移计算相机应该移动的弧度
        /// </summary>
        Radian m_cursor_delta_yaw {0};
        Radian m_cursor_delta_pitch {0};

        /// <summary>
        /// 重置指令
        /// </summary>
        void resetGameCommand() { m_game_command = 0; }

        /// <summary>
        /// 返回当前的游戏指令
        /// </summary>
        unsigned int getGameCommand() const { return m_game_command; }

    private:
        void onKeyInGameMode(int key, int scancode, int action, int mods);

        /// <summary>
        /// 根据鼠标位移距离计算移动弧度
        /// </summary>
        void calculateCursorDeltaAngles();

        /// <summary>
        /// 记录当前输入的游戏指令
        /// </summary>
        unsigned int m_game_command {0};

        /// <summary>
        /// 记录上一次的光标位置
        /// </summary>
        int m_last_cursor_x {0};
        int m_last_cursor_y {0};
    };
} // namespace Piccolo
