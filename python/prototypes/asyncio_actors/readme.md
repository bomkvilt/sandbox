# Async-io actors

## Summary

- picklable object method decorator; SA: `ActorMethod`
- passing open file descriptors to a `spawn`-ed subprocess;

## More Details

The prototype is supposed to show how to add an additional abstraction level to the built-in asyncio and connect
    multiple event loops by a uniform and user-transparent way.

The only reason for the abstraction level is avoidong the GIL limitations in cases when you need to enable more
    computational resources in a dynamic way (when you cannot pre-allocate a required precess-pool on an init-time).

However, this implementation has tipical cons of non-composable parallelism:

- as we create process pools in a dynamic and non-coordinated way, we can spawn more parallel brunches than we can efficiently process
- as the task pools are nested, we have an exponential growth of parallel brunches

The implementation-specific cons are:

- the actrs can be used in a fork-join sheme and cannot form arbitrary graphs

### Picklable Properties

```py
@final
class ActorMethod[S: 'Actor', **P, R]:
    def __init__(self, action: Callable[Concatenate[S, P], Coroutine[Any, Any, R]]) -> None:
        # copy action's attributes to mimic the object
        for attr in ("__module__", "__name__", "__qualname__", ):
            setattr(self, attr, getattr(action, attr))
        self.__action: Final = action

    def _update_action(self, cls: type[Actor], field_name: str) -> None:
        assert self.__action.__name__ == field_name, f"'{self.__action.__name__}' vs. '{field_name}'"
        # key moment: update wrapped object's name to prevent errors that the objects in a new process are different
        for attr in ("__name__", "__qualname__"):
            setattr(self.__action, attr, getattr(self.__action, attr) + "_wrapped")
        setattr(cls, self.__action.__name__, self.__action)

    ...


class ActorMetaClass(abc.ABCMeta):
    def __new__(mcls, name: str, bases: tuple[type, ...], namespace: dict[str, Any], /, **kwargs: Any):
        cls = super().__new__(mcls, name, bases, namespace, **kwargs)
        for field_name, field_type in list(cls.__dict__.items()):
            if isinstance(field_type, ActorMethod):
                field_type._update_action(cls, field_name)
        return cls


class Actor(..., metaclass=ActorMetaClass):
    ...
```
