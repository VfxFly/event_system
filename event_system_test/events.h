#pragma once
#include <algorithm>
#include <type_traits>
#include <vector>

#define event_traits(name, ...) \
    struct name { using callback = __VA_ARGS__; }

#define event_traits_in(space, name, ...) \
    namespace space { event_traits(name, __VA_ARGS__); }

namespace mvm
{
    template <typename ev>
    using event_callback_t = typename ev::callback;

    template <typename ev>
    inline std::vector<event_callback_t<ev>> event_list;

    template <typename ev>
    void register_event(event_callback_t<ev> callback)
    {
        if (callback)
        {
            event_list<ev>.push_back(callback);
        }
    }

    template <typename ev>
    void unregister_event(event_callback_t<ev> callback)
    {
        std::erase(event_list<ev>, callback);
    }

    template <typename ev>
    bool has_events() noexcept
    {
        return !event_list<ev>.empty();
    }

    template <typename ev, typename... args>
    auto invoke_events(args&&... a)
    {
        using result_t = std::invoke_result_t<event_callback_t<ev>, args&...>;

        static_assert(std::is_same_v<result_t, bool> || std::is_void_v<result_t>);

        if constexpr (std::is_same_v<result_t, bool>)
        {
            bool output = false;

            for (const auto callback : event_list<ev>)
            {
                if (callback(a...))
                {
                    output = true;
                }
            }

            return output;
        }
        else
        {
            for (const auto callback : event_list<ev>)
            {
                callback(a...);
            }
        }
    }
}