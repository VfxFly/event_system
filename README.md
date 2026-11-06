# event_system
single header event system impl. without dependencies | std20+

defination event traits:
```
  ...
  namespace some_object_events
  {
    event_traits(on_action, void(*)(some_object*));
  }
  ...
```
or
```
  ...
  event_traits_in(some_object_events, on_action, void(*)(some_object*));
  ...
```

event invokation:
```
  void some_object::on_action()
  {
    mvm::invoke_events<some_object_events::on_action>(this);
  }
```

event handler add/remove:
```
  void on_action_event(some_object* obj)
  {
    // do handle object event...
  }

  ...
  mvm::register_event<some_object_events::on_action>(::on_action_event);
  ...

  ...
  mvm::unregister_event<some_object_events::on_action>(::on_action_event);
  ...
```
