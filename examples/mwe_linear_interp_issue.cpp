/**
 * @file mwe_linear_interp_issue.cpp
 * @brief Minimum working example of a linear interpolation issue in
 * grid_map_core, where the interpolation results in negative values even when
 * the map contains non-negative values.
 * @author Amro Al-Baali
 * @version
 * @date 2024-05-26
 */
#include <iostream>

#include "grid_map_core/grid_map_core.hpp"

int main() {
  grid_map::GridMap map{};
  map.setGeometry({3, 3}, 1.0);
  const std::string layer = "distance";
  map.add(layer, 0.0);
  // clang-format off
  map[layer] << 
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 3.0;
  // clang-format on

  const grid_map::Position pos{-1.49, -1.0};
  if (!map.isInside(pos)) {
    std::cerr << "Position outside map" << std::endl;
    return 1;
  }
  const auto val =
      map.atPosition(layer, pos, grid_map::InterpolationMethods::INTER_LINEAR);
  std::cout << "Val: " << val << std::endl;
}
