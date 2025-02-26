#pragma once

#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_guid_allocator.h"
#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_type.h"

#include <array>
#include <memory>
#include <optional>

namespace Piccolo
{
    class WindowSystem;
    class RHI;
    class RenderResourceBase;
    class RenderPipelineBase;
    class RenderScene;
    class RenderCamera;
    class WindowUI;
    class DebugDrawManager;

    struct RenderSystemInitInfo
    {
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<DebugDrawManager> debugdraw_manager;
    };

    struct EngineContentViewport
    {
        float x{ 0.f };
        float y{ 0.f };
        float width{ 0.f };
        float height{ 0.f };
    };

    class RenderSystem
    {
    public:
        RenderSystem() = default;
        ~RenderSystem();

        /// <summary>
        /// 渲染系统初始化
        /// </summary>
        /// <param name="init_info">渲染的是哪一个窗口的信息</param>
        void initialize(RenderSystemInitInfo init_info);

        void tick(float delta_time);
        void clear();

        void swapLogicRenderData();
        RenderSwapContext& getSwapContext();
        std::shared_ptr<RenderCamera> getRenderCamera() const;
        std::shared_ptr<RHI> getRHI() const;

        void setRenderPipelineType(RENDER_PIPELINE_TYPE pipeline_type);
        void initializeUIRenderBackend(WindowUI* window_ui);
        void updateEngineContentViewport(float offset_x, float offset_y, float width, float height);
        uint32_t getGuidOfPickedMesh(const Vector2& picked_uv);
        GObjectID getGObjectIDByMeshID(uint32_t mesh_id) const;

        EngineContentViewport getEngineContentViewport() const;

        void createAxis(std::array<RenderEntity, 3> axis_entities, std::array<RenderMeshData, 3> mesh_datas);
        void setVisibleAxis(std::optional<RenderEntity> axis);
        void setSelectedAxis(size_t selected_axis);
        GuidAllocator<GameObjectPartId>& getGOInstanceIdAllocator();
        GuidAllocator<MeshSourceDesc>& getMeshAssetIdAllocator();

        void clearForLevelReloading();

    private:
        /// <summary>
        /// 渲染管线类型，一个枚举类型：有前向/延时渲染
        /// </summary>
        RENDER_PIPELINE_TYPE m_render_pipeline_type{ RENDER_PIPELINE_TYPE::DEFERRED_PIPELINE };

        /// <summary>
        /// 管理渲染上下文中的交换数据
        /// </summary>
        RenderSwapContext m_swap_context;

        /// <summary>
        /// 渲染接口    RHI是一个抽象类，可以使用各种图形API来实现（如 DirectX、OpenGL、Vulkan等）
        /// Piccolo使用的是Vulkan
        /// </summary>
        std::shared_ptr<RHI> m_rhi;

        /// <summary>
        /// 管理相机的渲染
        /// </summary>
        std::shared_ptr<RenderCamera> m_render_camera;

        /// <summary>
        /// 管理场景的渲染
        /// </summary>
        std::shared_ptr<RenderScene> m_render_scene;

        /// <summary>
        /// 管理渲染过程中使用的资源
        /// </summary>
        std::shared_ptr<RenderResourceBase> m_render_resource;

        /// <summary>
        /// 渲染管线
        /// </summary>
        std::shared_ptr<RenderPipelineBase> m_render_pipeline;

        void processSwapData();
    };
} // namespace Piccolo
