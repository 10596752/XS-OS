# XS-OSC 说明文档 / XS-OSC README

---

## 简介（Introduction）

XS-OSC 是一个用 C++ 编写的 Windows 桌面端“仿操作系统”体验项目，支持用户注册/登录、音乐播放、文件管理、小游戏（如井字棋人机对战）、意见反馈等功能。  
本项目适合学习 C++ 文件操作、加密、Windows API、多级菜单等编程技巧。

XS-OSC is a Windows desktop "OS simulation" project written in C++. It supports user registration/login, music playback, file management, mini-games (like Tic-Tac-Toe with AI), feedback, and more.  
This project is suitable for learning C++ file operations, encryption, Windows API, multi-level menus, and related programming skills.

---

## 主要功能（Main Features）

- 用户注册与登录（密码 SHA-256 加密存储）
- 多级菜单导航
- 音乐播放与停止（支持多首本地音乐）
- 文件资源管理器（模拟）
- 井字棋人机对战（AI 极小化极大算法，电脑几乎不会输）
- 彩蛋与反馈入口
- 支持中文界面与输入输出

- User registration and login (passwords stored with SHA-256 encryption)
- Multi-level menu navigation
- Music playback and stop (multiple local music files)
- File explorer (simulation)
- Tic-Tac-Toe game (AI with minimax algorithm, almost unbeatable)
- Easter eggs and feedback entry
- Full Chinese interface and I/O support

---

## 运行环境与依赖（Environment & Dependencies）

- Windows 10/11
- 编译器：MinGW-w64 或 MSVC，支持 C++17 及以上
- 依赖库：Windows API（bcrypt, winmm, shellapi, mmsystem）
- 推荐使用 VS Code + C/C++ 插件开发和调试

- Windows 10/11
- Compiler: MinGW-w64 or MSVC, C++17 or above
- Dependencies: Windows API (bcrypt, winmm, shellapi, mmsystem)
- Recommended: VS Code + C/C++ extension for development and debugging

---

## 编译与运行（Build & Run）

1. 克隆本项目到本地
2. 用 VS Code 或其他 IDE 打开
3. 编译主文件 `Snimai.cpp`，确保链接 `bcrypt` 和 `winmm` 库
4. 运行生成的 `Snimai.exe`

**注意：**  
- 首次运行会在 `src` 目录下生成 `name.csv` 和 `password.csv` 文件用于保存账号信息
- 音乐文件需放在 `src/sound/` 目录下，文件名需与代码一致

1. Clone this project to your local machine
2. Open with VS Code or other IDE
3. Build the main file `Snimai.cpp`, make sure to link `bcrypt` and `winmm` libraries
4. Run the generated `Snimai.exe`

**Note:**  
- On first run, `name.csv` and `password.csv` will be created in the `src` directory for account info
- Music files must be placed in `src/sound/` with names matching those in the code

---

## 目录结构（Directory Structure）

```
XS-OSC/
│
├─ Snimai.cpp
├─ src/
│   ├─ header/
│   │   ├─ begin.h
│   │   ├─ yinhebrower.h
│   │   └─ jingziqi.h
│   ├─ sound/
│   │   ├─ 枫丹.mp3
│   │   ├─ 系.mp3
│   │   └─ ...（更多音乐文件）
│   ├─ name.csv
│   └─ password.csv
└─ XS-OSC.exe
```

---

## 注意事项（Notes）

- 项目包含 `XS-OSC.exe` 可执行文件，部分杀毒软件可能会误报，请自行添加信任。
- 仅支持 Windows 平台，Linux/Mac 下无法运行。
- 控制台需支持 UTF-8 编码和中文字体，否则中文可能显示为乱码。
- 如需反馈或建议，请通过菜单“意见反馈”入口填写表单。

- The project includes `XS-OSC.exe` executable. Some antivirus software may give false positives; please add it to your trusted list.
- Windows only. Not supported on Linux/Mac.
- Console must support UTF-8 and Chinese fonts, or Chinese may display as garbled text.
- For feedback or suggestions, use the "Feedback" menu entry to fill out the form.

---

## 联系方式（Contact）

- 作者：SigN_285 / Byteburst Studio
- QQ: 2838679223
- 微信：SigN_285fu

- Author: SigN_285 / Byteburst Studio
- QQ: 2838679223
- WeChat: SigN_285fu