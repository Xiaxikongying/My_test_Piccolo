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
        /// ���ؽ�ɫ����id
        /// </summary>
        GObjectID getObjectID() const;

        /// <summary>
        /// ���ý�ɫ��ָ����gobject�󶨣�����ʼ������transform���
        /// </summary>
        void setObject(std::shared_ptr<GObject> gobject);

        /// <summary>
        /// ��ȡ��ɫָ��
        /// </summary>
        std::weak_ptr<GObject> getObject() const { return m_character_object; }

        /// <summary>
        /// ����/��ȡ��ɫλ�ú���ת
        /// </summary>
        void setPosition(const Vector3& position) { m_position = position; }
        void setRotation(const Quaternion& rotation) { m_rotation = rotation; }
        const Vector3& getPosition() const { return m_position; }
        const Quaternion& getRotation() const { return m_rotation; }

        void tick(float delta_time);

    private:
        /// <summary>
        /// ��F�л����������
        /// </summary>
        void toggleFreeCamera();

        /// <summary>
        /// ��ɫ��λ�á�����
        /// </summary>
        Vector3 m_position;
        Quaternion m_rotation;

        /// <summary>
        /// ��ɫobjectָ��
        /// </summary>
        std::shared_ptr<GObject> m_character_object;

        /// <summary>
        /// ��ɫ��ת�Ļ��壬���ڴ�����ת�ı仯   hack for setting rotation frame buffer
        /// </summary>
        Quaternion m_rotation_buffer;

        /// <summary>
        /// �����ת�Ƿ��б仯
        /// </summary>
        bool m_rotation_dirty {false};

        /// <summary>
        /// ������  ���ģʽ
        /// </summary>
        CameraMode m_original_camera_mode;

        /// <summary>
        /// �Ƿ������ģʽ
        /// </summary>
        bool m_is_free_camera{false};
    };
} // namespace Piccolo