This project is an attempt to merge a few of my (Willy Jacquet) master's degree (ID3D) class projects into a single one.

# Motivation

I spent a great amount of time developping a [collection of libraries for graphics programming](https://github.com/the-last-willy/abstractgl).

Therefore I believe this to be a win-win situation; I get to continue enhancing my library with tools encountered in classes and I avoid doing things I have already done. This will require more work out of me - as I need to maintain and adapt a larger codebase - but I expect the end results to be greatly enhanced.

When this project is done (as it is limited in time), the emerging modules will be moved to AbstractGL.
If the results are interesting they might be moved into their own projects.

# Projects

- [3D Image Synthesis (iehl)](iehl)
- [Implicit modelling (galin)](galin)
- [Meshes and Computational Geometry (chaine)](chaine)

Upcoming projects:
- Digital terrain modelling (galin2)
- GPU programming (iehl2)
- Animation (meyer/zara) ???

Some of these projects have been made in group.

# Compatibility

Support for OpenGL 4.5 is required.

??? CMake
??? MSVC
??? GCC

# Installation

This project uses git submodules, to clone them as well, you can use the following command:

```
git clone --recursive https://github.com/the-last-willy/sandbox.git
```

If you do not wish to download the full history, you can add the option `--depth=1` to the previous command.
