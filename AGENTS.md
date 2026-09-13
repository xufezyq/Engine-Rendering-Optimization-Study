# Repository Guidelines

## Project Structure & Module Organization

This repository is a Windows/C++ rendering study. The active sample is in `Render/GLMDemo/`: `GLMDemo.slnx` contains the Visual Studio solution, and `GLMDemo/GLMDemo/main.cpp` is the demo source. The header-only GLM dependency is vendored in `Render/glm-1.0.3/glm`. Learning notes and supporting material live in `docs/`, the root Markdown files, and root `.obj` assets. Visual Studio output folders (`Debug/`, `Release/`, and `x64/`) are generated artifacts; do not edit or commit them.

## Build, Test, and Development Commands

Open `Render/GLMDemo/GLMDemo.slnx` in Visual Studio 2022+ and select `Debug|x64` (preferred) or `Release|x64`, then **Build Solution** (`Ctrl+Shift+B`). From a Developer PowerShell, the equivalent is:

```powershell
msbuild Render/GLMDemo/GLMDemo.slnx /p:Configuration=Debug /p:Platform=x64
```

Run the resulting console program from the matching output directory, for example `Render/GLMDemo/GLMDemo/x64/Debug/GLMDemo.exe`. There is no separate test project or package-manager bootstrap; a successful build and a clean demo run are the current validation steps.

## Coding Style & Naming Conventions

Use C++20, UTF-8 source files, four-space indentation, braces on the same line, and readable section comments for math demonstrations. Prefer `snake_case` for local variables and helper functions, `PascalCase` for types, and descriptive GLM types such as `glm::mat4`. Keep includes minimal and use the existing relative GLM include path; avoid absolute machine-specific paths. Match the warning level and `/utf-8` settings already defined in `GLMDemo.vcxproj`.

## Testing Guidelines

No automated framework or coverage target is configured. For each change, build both Debug and (when practical) Release, run the console demo, and inspect output for expected vector/matrix values. If adding tests, keep them near the affected sample and name them after the behavior they verify (for example, `test_euler_rotation`).

## Commit & Pull Request Guidelines

Use short imperative commits with the existing Conventional Commit pattern: `refactor(GLMDemo): ...`, `build(vs project): ...`, or `chore: ...`. Keep commits focused. Pull requests should explain the rendering/math change, list configurations tested, link related issues when applicable, and include console output or screenshots for visible behavior changes. Do not include generated binaries, PDBs, or IDE user files.

## Configuration & Dependency Notes

Keep GLM under `Render/glm-1.0.3`; the project currently resolves it through a relative include directory. Update project settings consistently for all four configurations and preserve UTF-8 handling for Chinese notes and comments.
