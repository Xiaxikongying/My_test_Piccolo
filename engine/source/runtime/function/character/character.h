#pragma once

#include "runtime/core/math/transform.h"

#include "runtime/function/framework/component/camera/camera_component.h"
#include "runtime/function/framework/object/object.h"

#include <vector>

namespace Piccolo
{
    class Character
    {
        inline static const float s_camera_blend_time {0.3f};

    public:
        Character(std::shared_ptr<GObject> character_object);

        /// <summary>
        /// 返回角色物体id
        /// </summary>
        GObjectID getObjectID() const;

        /// <summary>
        /// 设置角色与指定的gobject绑定，并初始化他的transform组件
        /// </summary>
        void setObject(std::shared_ptr<GObject> gobject);

        /// <summary>
        /// 获取角色指针
        /// </summary>
        std::weak_ptr<GObject> getObject() const { return m_character_object; }

        /// <summary>
        /// 设置/获取角色位置和旋转
        /// </summary>
        void setPosition(const Vector3& position) { m_position = position; }
        void setRotation(const Quaternion& rotation) { m_rotation = rotation; }
        const Vector3& getPosition() const { return m_position; }
        const Quaternion& getRotation() const { return m_rotation; }

        void tick(float delta_time);

    private:
        /// <summary>
        /// 按F切换到自由相机
        /// </summary>
        void toggleFreeCamera();

        /// <summary>
        /// 角色的位置、朝向
        /// </summary>
        Vector3 m_position;
        Quaternion m_rotation;

        /// <summary>
        /// 角色object指针
        /// </summary>
        std::shared_ptr<GObject> m_character_object;

        /// <summary>
        /// 角色旋转的缓冲，用于处理旋转的变化   hack for setting rotation frame buffer
        /// </summary>
        Quaternion m_rotation_buffer;

        /// <summary>
        /// 标记旋转是否有变化
        /// </summary>
        bool m_rotation_dirty {false};

        /// <summary>
        /// 相机组件  相机模式
        /// </summary>
        CameraMode m_original_camera_mode;

        /// <summary>
        /// 是否是相机模式
        /// </summary>
        bool m_is_free_camera{false};
    };
} // namespace Piccolo