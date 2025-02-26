#include "runtime/function/character/character.h"

#include "runtime/engine.h"
#include "runtime/function/framework/component/motor/motor_component.h"
#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"

namespace Piccolo
{
    Character::Character(std::shared_ptr<GObject> character_object) { setObject(character_object); }

    GObjectID Character::getObjectID() const
    {
        if (m_character_object)
        {
            return m_character_object->getID();
        }
        return k_invalid_gobject_id;
    }

    /// <summary>
    /// 设置角色与指定的gobject绑定，并初始化他的transform组件
    /// </summary>
    void Character::setObject(std::shared_ptr<GObject> gobject)
    {
        m_character_object = gobject;
        if (m_character_object)
        {
            const TransformComponent* transform_component =
                m_character_object->tryGetComponentConst(TransformComponent);
            const Transform& transform = transform_component->getTransformConst();
            m_position                 = transform.m_position;
            m_rotation                 = transform.m_rotation;
        }
        else //如果角色不存在，则设为默认值
        {
            m_position = Vector3::ZERO;
            m_rotation = Quaternion::IDENTITY;
        }
    }

    void Character::tick(float delta_time)
    {
        if (m_character_object == nullptr) //角色是否有效
            return;

        //command：从输入系统获取当前的指令
        unsigned int command = g_runtime_global_context.m_input_system->getGameCommand();
        if (command < (unsigned int)GameCommand::invalid) //输入的是有效指令
        {
            //是否是自由相机模式
            if ((((unsigned int)GameCommand::free_carema & command) > 0) != m_is_free_camera)
            {
                //如果是，则开启自由相机模式    若不是，则是控制角色移动
                toggleFreeCamera();
            }
        }

        //获取角色的TransformComponent组件
        TransformComponent* transform_component = m_character_object->tryGetComponent(TransformComponent);

        //是否要旋转
        if (m_rotation_dirty)
        {
            transform_component->setRotation(m_rotation_buffer);
            m_rotation_dirty = false;
        }

        //尝试获取运动组件
        const MotorComponent* motor_component = m_character_object->tryGetComponentConst(MotorComponent);
        if (motor_component == nullptr)
        {
            return;
        }

        if (motor_component->getIsMoving()) //是否正在移动
        {
            m_rotation_buffer = m_rotation;
            transform_component->setRotation(m_rotation_buffer);
            m_rotation_dirty = true;
        }
        const Vector3& new_position = motor_component->getTargetPosition();
        //更新位移
        m_position = new_position;

        //float blend_ratio = std::max(1.f, motor_component->getSpeedRatio());

        //float frame_length = delta_time * blend_ratio;
        //m_position =
        //    (m_position * (s_camera_blend_time - frame_length) + new_position * frame_length) / s_camera_blend_time;
        //m_position =
        //    (m_position * (s_camera_blend_time - frame_length) + new_position * frame_length) / s_camera_blend_time;
    }

    void Character::toggleFreeCamera()
    {
        //获取相机组件
        CameraComponent* camera_component = m_character_object->tryGetComponent(CameraComponent);
        if (camera_component == nullptr) return;

        //更新 是否是相机模式  每次按F切换相机模式的状态
        m_is_free_camera = !m_is_free_camera;

        if (m_is_free_camera)
        {
            m_original_camera_mode = camera_component->getCameraMode();
            camera_component->setCameraMode(CameraMode::free);//设置为自由模式
        }
        else
        {
            camera_component->setCameraMode(m_original_camera_mode);
        }
    }
} // namespace Piccolo