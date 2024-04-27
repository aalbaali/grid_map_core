# Overview
This is the grid-map core cloned from [grid-map](https://github.com/ANYbotics/grid_map) and has exported targets.

The library is exported as the target `GridMap::GridMapCore`. To use downstream:
```bash
find_package(GridMapCore REQUIRED)
...
target_link_libraries(... GridMap::GridMapCore)
```
