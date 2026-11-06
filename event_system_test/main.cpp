#include <iostream>
#include <string>

class runtime;

void on_create(runtime* obj)
{
    std::cout << "[" << __FUNCTION__ << "] runtime::create was been called...\n";
}

void on_destroy(runtime* obj)
{
    std::cout << "[" << __FUNCTION__ << "] runtime::destroy was been called...\n";
}

void on_set_name(runtime* obj, const std::string& next_name)
{
    std::cout << "[" << __FUNCTION__ << "] runtime::set_name was been called..., next name is '" << next_name.c_str() << "'...\n";
}

#include "events.h"

namespace runtime_events
{
    event_traits(on_create, void(*)(runtime*));
}
event_traits_in(runtime_events, on_destroy, void(*)(runtime*));
event_traits_in(runtime_events, on_set_name, void(*)(runtime*, const std::string&));

class runtime
{
public:
    runtime() = default;
    ~runtime() = default;

    void create()
    {
        if (m_initialized)
        {
            return;
        }

        m_initialized = true;
        mvm::invoke_events<runtime_events::on_create>(this);
    }

    void destroy()
    {
        if (!m_initialized)
        {
            return;
        }

        m_initialized = false;
        mvm::invoke_events<runtime_events::on_destroy>(this);
    }

    void set_name(const std::string& name)
    {
        m_name = name;
        mvm::invoke_events<runtime_events::on_set_name>(this, name);
    }

private:
    bool m_initialized = false;

    std::string m_name = "runtime";

};

int main()
{
    std::cout << "creating 'runtime' events...\n";

    mvm::register_event<runtime_events::on_create>(on_create);
    mvm::register_event<runtime_events::on_destroy>(on_destroy);
    mvm::register_event<runtime_events::on_set_name>(on_set_name);
    
    {
        std::cout << "creating 'runtime' object instance...\n";

        std::unique_ptr<runtime> object = std::make_unique<runtime>();

        std::cout << "calling 'runtime' instance functional...\n";

        object->create();
        object->set_name("new runtime");
        object->destroy();

        std::cout << "destroying 'runtime' instance pointer...\n";

        object.reset();
    }

    std::cout << "destroying 'runtime' events...\n";

    mvm::unregister_event<runtime_events::on_set_name>(on_set_name);
    mvm::unregister_event<runtime_events::on_destroy>(on_destroy);
    mvm::unregister_event<runtime_events::on_create>(on_create);
}