rem Deletes temporary files.

rmdir /s/q .git
rmdir /s/q build

rem Deletes submodules unecessary files (documentation, examples, tests, ...).

rem 'dear_imgui'.
rmdir /s/q lib/dear_imgui/dear_imgui/docs
rmdir /s/q lib/dear_imgui/dear_imgui/examples

rem 'glfw3'.
rmdir /s/q lib/glfw3/glfw3/docs
rmdir /s/q lib/glfw3/glfw3/examples
rmdir /s/q lib/glfw3/glfw3/tests

rem 'range_v3'.
rmdir /s/q lib/range_v3/range_v3/doc
rmdir /s/q lib/range_v3/range_v3/example
rmdir /s/q lib/range_v3/range_v3/perf
rmdir /s/q lib/range_v3/range_v3/test
rmdir /s/q lib/range_v3/range_v3/test_package

rem 'tinygltf'.
rmdir /s/q lib/tinygltf/tinygltf/examples
rmdir /s/q lib/tinygltf/tinygltf/experimental
rmdir /s/q lib/tinygltf/tinygltf/models
rmdir /s/q lib/tinygltf/tinygltf/tests
rmdir /s/q lib/tinygltf/tinygltf/tools

rem 'tinyobjloader'.
rmdir /s/q lib/tinyobjloader/tinyobjloader/examples
rmdir /s/q lib/tinyobjloader/tinyobjloader/experimental
rmdir /s/q lib/tinyobjloader/tinyobjloader/fuzzer
rmdir /s/q lib/tinyobjloader/tinyobjloader/images
rmdir /s/q lib/tinyobjloader/tinyobjloader/models
rmdir /s/q lib/tinyobjloader/tinyobjloader/python
rmdir /s/q lib/tinyobjloader/tinyobjloader/tests
rmdir /s/q lib/tinyobjloader/tinyobjloader/tools
