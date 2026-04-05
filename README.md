📦 SDL-Game-Engine
├── 📂 assets/                 # Game assets (images, fonts, sounds)
│   └── 📂 sprites/            # Default sprites (e.g., defaultSquare.png)
├── 📂 core/                   # Core Engine Architecture
│   ├── 📂 include/            # Engine Header files (.h)
│   │   ├── 📂 components/
│   │   │   └── 📄 Component.h # Base Component, Transform, RectTransform, SpriteRenderer
│   │   ├── 📂 setting/
│   │   │   └── 📄 Setting.h
│   │   ├── 📄 Core.h
│   │   ├── 📄 Input.h
│   │   ├── 📄 ObjectManager.h # Base Object & GameObject classes
│   │   ├── 📄 Screen.h        # Screen dimensions (SCREEN_WIDTH, SCREEN_HEIGHT)
│   │   ├── 📄 SpriteManager.h # Texture rendering and sorting (Layer & Y-Sort)
│   │   └── 📄 UIManager.h     # UI System (RectTransform, Buttons, Text)
│   └── 📂 src/                # Engine Source files (.cpp)
│       ├── 📂 components/
│       │   └── 📄 Component.cpp
│       ├── 📄 Core.cpp
│       ├── 📄 Input.cpp
│       ├── 📄 ObjectManager.cpp
│       ├── 📄 Screen.cpp
│       ├── 📄 SpriteManager.cpp
│       └── 📄 UIManager.cpp
├── 📂 scripts/                # Gameplay Scripts (User Code)
│   ├── 📂 include/
│   │   └── 📄 player.h        # Player logic header
│   └── 📂 src/
│       └── 📄 player.cpp      # Player logic implementation
├── 📂 test/                   # Unit tests and testing environments
├── 📄 .gitignore              # Files ignored by Git (e.g., .o, .exe)
├── 📄 main.cpp                # Main entry point / Game Loop
└── 📄 main.exe                # Compiled executable