#include "runtime/function/framework/component/lua/lua_component.h"
#include "runtime/core/base/macro.h"
#include "runtime/function/framework/object/object.h"
namespace Piccolo
{

    void LuaComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;
        m_lua_state.open_libraries(sol::lib::base);
        m_lua_state.set_function("set_float", &LuaComponent::set<float>);
        m_lua_state.set_function("get_bool", &LuaComponent::get<bool>);
        m_lua_state.set_function("invoke", &LuaComponent::invoke);
        m_lua_state["GameObject"] = m_parent_object;
    }

    void LuaComponent::tick(float delta_time)
    {
        // LOG_INFO(m_lua_script);
        m_lua_state.script(m_lua_script);
    }

    /// <summary>
    /// 用于查找并访问 GameObject 中指定组件的字段  下面的set get invoke都会通过find_component_field来使用反射功能
    /// </summary>
    /// <param name="game_object">查找的物体</param>
    /// <param name="field_name">要查找的字段名</param>
    /// <param name="field_accessor">字段类型</param>
    /// <param name="target_instance">返回值，保存字段的地址</param>
    /// <returns></returns>
    bool find_component_field(std::weak_ptr<GObject> game_object,
                              const char *field_name,
                              Reflection::FieldAccessor &field_accessor,
                              void *&target_instance)
    {
        auto components = game_object.lock()->getComponents();

        std::istringstream iss(field_name);
        std::string current_name;
        std::getline(iss, current_name, '.');
        auto component_iter = std::find_if(
            components.begin(), components.end(), [current_name](auto c)
            { return c.getTypeName() == current_name; });
        if (component_iter != components.end())
        {
            auto meta = Reflection::TypeMeta::newMetaFromName(current_name);
            void *field_instance = component_iter->getPtr();

            // find target field
            while (std::getline(iss, current_name, '.'))
            {
                Reflection::FieldAccessor *fields;
                int fields_count = meta.getFieldsList(fields);
                auto field_iter = std::find_if(
                    fields, fields + fields_count, [current_name](auto f)
                    { return f.getFieldName() == current_name; });
                if (field_iter == fields + fields_count) // not found
                {
                    delete[] fields;
                    return false;
                }

                field_accessor = *field_iter;
                delete[] fields;

                target_instance = field_instance;

                // for next iteration
                field_instance = field_accessor.get(target_instance);
                field_accessor.getTypeMeta(meta);
            }
            return true;
        }
        return false;
    }

    /// <summary>
    /// 通过反射设置字段的值
    /// </summary>
    template <typename T>
    void LuaComponent::set(std::weak_ptr<GObject> game_object, const char *name, T value)
    {
        LOG_INFO(name);
        Reflection::FieldAccessor field_accessor;
        void *target_instance;
        if (find_component_field(game_object, name, field_accessor, target_instance))
        {
            field_accessor.set(target_instance, &value);
        }
        else
        {
            LOG_ERROR("Can't find target field.");
        }
    }

    /// <summary>
    /// 通过反射获取字段的值
    /// </summary>
    template <typename T>
    T LuaComponent::get(std::weak_ptr<GObject> game_object, const char *name)
    {

        LOG_INFO(name);

        Reflection::FieldAccessor field_accessor;
        void *target_instance;
        if (find_component_field(game_object, name, field_accessor, target_instance))
        {
            return *(T *)field_accessor.get(target_instance);
        }
        else
        {
            LOG_ERROR("Can't find target field.");
        }
    }

    /// <summary>
    /// 通过反射执行对应的函数
    /// </summary>
    void LuaComponent::invoke(std::weak_ptr<GObject> game_object, const char *name)
    {
        LOG_INFO(name);

        Reflection::TypeMeta meta;
        void *target_instance = nullptr;
        std::string method_name;

        // get target instance and meta
        std::string target_name(name);
        size_t pos = target_name.find_last_of('.');
        method_name = target_name.substr(pos + 1, target_name.size());
        target_name = target_name.substr(0, pos);

        if (target_name.find_first_of('.') == target_name.npos)
        {
            // target is a component
            auto components = game_object.lock()->getComponents();

            auto component_iter = std::find_if(
                components.begin(), components.end(), [target_name](auto c)
                { return c.getTypeName() == target_name; });
            if (component_iter != components.end())
            {
                meta = Reflection::TypeMeta::newMetaFromName(target_name);
                target_instance = component_iter->getPtr();
            }
            else
            {
                LOG_ERROR("Cand find component");
                return;
            }
        }
        else
        {
            Reflection::FieldAccessor field_accessor;
            if (find_component_field(game_object, name, field_accessor, target_instance))
            {
                target_instance = field_accessor.get(target_instance);
                field_accessor.getTypeMeta(meta);
            }
            else
            {
                LOG_ERROR("Can't find target field.");
                return;
            }
        }

        // invoke function
        Reflection::MethodAccessor *methods;
        size_t method_count = meta.getMethodsList(methods);
        auto method_iter = std::find_if(
            methods, methods + method_count, [method_name](auto m)
            { return m.getMethodName() == method_name; });
        if (method_iter != methods + method_count)
        {
            method_iter->invoke(target_instance);
        }
        else
        {
            LOG_ERROR("Cand find method");
        }
        delete[] methods;
    }
} // namespace Piccolo
