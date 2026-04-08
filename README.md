# SDL3 Game Engine (Zan Engine)

A lightweight, component-based 2D game engine built on top of **SDL3** and **SDL3_image**, written in C++.

---

## Features

- **Component system** — attach/detach components to objects at runtime; components self-configure via `OnAttached()` / `OnDetached()` hooks
- **ZanBehavior** — Unity-style scripting lifecycle (`OnAwake`, `OnStart`, `OnUpdate`, `OnEnable`, `OnDisable`, `OnCollisionEnter`)
- **Physics** — AABB collision detection with impulse-based resolution, positional correction, and mass-weighted separation
- **Rendering** — centralized `RenderManager` with layer-sorted rendering and debug collider outlines
- **UI System** — `UIText` and `UIButton` elements backed by `RectTransform`
- **Zan Coordinate System** — world origin at screen center, Y-axis up; automatic conversion to SDL screen coordinates

---

## Project Structure

```
SDL-Game-Engine/
├── assets/                     # Game assets (sprites, fonts, sounds)
│   └── sprites/
├── core/                       # Engine core (do not modify for gameplay)
│   ├── include/
│   │   ├── components/
│   │   │   ├── Component.h         # Base Component (OnAttached / OnDetached hooks)
│   │   │   ├── Transform.h         # Position, rotation, scale
│   │   │   ├── RectTransform.h     # UI layout transform
│   │   │   ├── SpriteRenderer.h    # Texture rendering (implements IRenderable)
│   │   │   ├── Collider.h          # Abstract collider base
│   │   │   ├── BoxCollider.h       # AABB collider (auto-sizes from SpriteRenderer on attach)
│   │   │   ├── RigidBody.h         # Physics body (Dynamic / Kinematic / Static)
│   │   │   └── IRenderable.h       # Renderable interface for RenderManager
│   │   ├── math/
│   │   │   └── Vector2.h
│   │   ├── BehaviorManager.h       # Drives ZanBehavior lifecycle per frame
│   │   ├── Coordinate.h            # Zan ↔ SDL coordinate conversion
│   │   ├── Core.h                  # Engine entry point (owns all managers)
│   │   ├── Input.h                 # Keyboard / mouse input
│   │   ├── ObjectManager.h         # Object & GameObject; component attach/detach
│   │   ├── PhysicManager.h         # Collision detection & impulse resolution
│   │   ├── RenderManager.h         # Layer-sorted rendering pipeline
│   │   ├── Screen.h                # Global screen dimensions
│   │   ├── UIElements.h            # UIText, UIButton
│   │   └── ZanBehavior.h           # Behavior base class (extends Component)
│   └── src/
│       ├── components/
│       │   ├── BoxCollider.cpp
│       │   ├── RectTransform.cpp
│       │   ├── RigidBody.cpp
│       │   ├── SpriteRenderer.cpp
│       │   └── Transform.cpp
│       ├── BehaviorManager.cpp
│       ├── Coordinate.cpp
│       ├── Core.cpp
│       ├── Input.cpp
│       ├── ObjectManager.cpp
│       ├── PhysicManager.cpp
│       ├── RenderManager.cpp
│       ├── Screen.cpp
│       ├── UIElements.cpp
│       └── ZanBehavior.cpp
├── scripts/                    # Gameplay scripts (user code goes here)
│   ├── include/
│   │   ├── player.h
│   │   └── wall.h
│   └── src/
│       ├── player.cpp
│       └── wall.cpp
├── main.cpp                    # Entry point & game loop
└── main.exe                    # Compiled binary
```

---

## Architecture

### Component System

All engine features are `Component` subclasses. Attach them to any `Object` via `AttachComponent()`:

```cpp
// Components configure themselves — behavior just attaches
collider = new BoxCollider(gameObject, gameObject->transform);
gameObject->AttachComponent(collider);  // OnAttached() → auto-sizes + registers with PhysicsManager

rigidBody = new RigidBody(gameObject, RigidbodyType::Dynamic, 1.0f);
gameObject->AttachComponent(rigidBody);
```

**Rule:** behaviors only *declare* and *use* components. All configuration and registration happens inside the component's constructor or `OnAttached()`.

### ZanBehavior Lifecycle

```
OnAwake()   →  called once on first attach (before Start)
OnStart()   →  called once on the first Update frame
OnEnable()  →  called when behavior becomes enabled
OnUpdate()  →  called every frame
OnDisable() →  called when behavior is disabled
OnDestroy() →  called on detach/destruction
OnCollisionEnter(Object* other)  →  called by PhysicsManager on collision
```

Attach a behavior script to any `Object` like a regular component:

```cpp
GameObject* playerObject = new GameObject(0, 0);
playerObject->AttachComponent(new Player());
```

### Coordinate System (Zan)

- **Origin**: center of the window
- **Y-axis**: up is positive
- All world positions use Zan coordinates; `CoordinateConverter::ZanToSDL()` converts for rendering

### Physics

`BoxCollider` automatically:
- Sizes itself from the attached `SpriteRenderer` (falls back to `100×50`)
- Registers/unregisters with `PhysicsManager` via `OnAttached()` / `OnDetached()`

`RigidBody` types:
| Type | Behavior |
|---|---|
| `Dynamic` | Fully simulated; responds to forces and impulses |
| `Kinematic` | Moved manually; not affected by physics |
| `Static` | Immovable; infinite mass in collision resolution |

---

## Build

**Requirements:** MSYS2 (UCRT64), SDL3, SDL3_image

```bash
g++ -g main.cpp core/src/*.cpp core/src/components/*.cpp core/debug/*.cpp scripts/src/*.cpp \
    -o main.exe \
    -I C:/msys64/ucrt64/include \
    -I core/include \
    -I scripts/include \
    -L C:/msys64/ucrt64/lib \
    -lSDL3 -lSDL3_image
```

```bash
./main
```