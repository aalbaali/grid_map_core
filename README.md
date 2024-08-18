# Overview
This is the grid-map core cloned from [grid-map](https://github.com/ANYbotics/grid_map) and has exported targets.

The library is exported as the target `GridMap::GridMapCore`. To use downstream:
```cmake
find_package(GridMapCore REQUIRED)
...
target_link_libraries(... GridMap::GridMapCore)
```

Or, to fetch and use:
```cmake
FetchContent_Declare(
  GridMapCoreTmp
  GIT_REPOSITORY git@github.com:aalbaali/grid_map_core.git
  GIT_TAG main
)
FetchContent_MakeAvailable(GridMapCoreTmp)

...
target_link_libraries(... GridMapCore)
```

Note that the library name is defined as `GridMapCore`, which is the value used when fetching the library. In contrast, when the library is installed, the namespace is added, which prompts the usage of `GridMap::GridMapCore`.
