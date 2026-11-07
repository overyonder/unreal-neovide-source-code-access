# Neovide Source Code Access

Unreal Engine source code accessor plugin that opens projects, classes, and source locations in [Neovide](https://neovide.dev/) on Linux, with the working directory set to the active Unreal project root so project-scoped `direnv` environments and `clangd` compilation databases attach automatically.

## Install

Clone into the project's `Plugins/` directory or the engine's `Engine/Plugins/Marketplace/` directory:

```sh
git clone https://github.com/overyonder/unreal-neovide-source-code-access.git \
  Plugins/NeovideSourceCodeAccess
```

Enable **Neovide Source Code Access** in Unreal's Plugins window (or pass `-EnablePlugins=NeovideSourceCodeAccess` on the command line) and select **Neovide** under **Editor Preferences → General → Source Code → Source Code Editor**.

## License

LicenseRef-MIT-SLOP
