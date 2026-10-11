# RapidHuck 🐍

一个使用 **C++17 + [raylib](https://www.raylib.com/)** 编写的跨平台贪吃蛇小游戏，使用 **CMake** 组织工程，可在 **Windows / Linux / macOS** 上编译运行，并配有 GitHub Actions 自动化构建与发布流程。

- 项目名称：RapidHuck
- 一句话简介：面向快速迭代的轻量级跨平台贪吃蛇游戏。
- 状态：可玩版本，持续完善中。

---

## 🎮 游戏玩法

- 方向键（↑ ↓ ← →）或 `W A S D` 控制蛇的移动，吃到食物后蛇身变长、分数 **+10**。
- 撞到墙壁或自己的身体游戏结束。
- 按键说明：

  | 按键 | 功能 |
  | --- | --- |
  | `↑` `↓` `←` `→` / `W` `A` `S` `D` | 移动（在开始界面按任意方向键即可开始） |
  | `Enter` / `Space` | 开始游戏 |
  | `P` | 暂停 / 继续 |
  | `R` | 游戏结束后重新开始 |
  | `Esc` | 退出 |

- 界面顶部 HUD 实时显示当前分数（Score）与本局最高分（Best）。
- 棋盘为 20 × 15 的网格，蛇每 8 帧移动一格（60 FPS 下约为每秒 7.5 格）。

## 🧩 依赖

- 支持 C++17 的编译器：
  - Windows：MSVC 2019+ 或 MinGW-w64
  - Linux：GCC 9+ 或 Clang 10+
  - macOS：Apple Clang（Xcode Command Line Tools）
- [CMake](https://cmake.org/) ≥ 3.16
- [raylib](https://github.com/raysan5/raylib) ≥ 5.0
  - 如果系统未安装 raylib，CMake 会自动通过 `FetchContent` 下载并构建 raylib 5.5，**无需手动安装**。
- Linux 额外系统依赖（Debian/Ubuntu）：

  ```bash
  sudo apt-get install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev \
      libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
      libwayland-dev libxkbcommon-dev
  ```

## 🔨 构建方法

### Linux / macOS

```bash
git clone https://github.com/RapidAI/RapidHuck.git
cd RapidHuck
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/RapidHuck
```

### Windows（PowerShell / CMD）

```powershell
git clone https://github.com/RapidAI/RapidHuck.git
cd RapidHuck
cmake -S . -B build
cmake --build build --config Release
.\build\Release\RapidHuck.exe
```

> 首次配置时会自动拉取并编译 raylib，可能需要几分钟；之后会使用 CMake 缓存。

## 📁 目录结构

```
RapidHuck/
├── CMakeLists.txt              # CMake 工程定义（自动获取 raylib）
├── src/
│   ├── main.cpp                # 程序入口、窗口与 HUD、覆盖层
│   ├── game.h                  # 游戏逻辑与状态声明
│   └── game.cpp                # 游戏逻辑与渲染实现
├── .github/workflows/build.yml # GitHub Actions 构建 / 发布流程
├── LICENSE
└── README.md
```

## ⚙️ 持续集成 / 发布

工作流位于 [`.github/workflows/build.yml`](.github/workflows/build.yml)：

- 在 **push 到 `main`、提交 Pull Request、打 `v*` 标签** 时触发。
- 在 `ubuntu-latest`、`windows-latest`、`macos-latest` 三个 runner 上自动编译。
- 每次构建都会把可执行文件作为 artifact 上传（`RapidHuck-Linux` / `RapidHuck-Windows` / `RapidHuck-macOS`）。
- 当推送形如 `v1.0.0` 的标签时，会额外自动创建 GitHub Release，并把三个平台的构建产物上传到该 Release。

发布一个新版本：

```bash
git tag v1.0.0
git push origin v1.0.0
```

## 📄 许可证

本项目基于 [MIT License](LICENSE) 开源。

## 📝 待补充

- 音效与背景音乐
- 关卡 / 难度选择
- 最高分持久化
- 单元测试与更多玩法
