#include <iostream>

#include "grid_map_core/GridMap.hpp"
#include "grid_map_core/TypeDefs.hpp"
#include "grid_map_core/iterators/GridMapIterator.hpp"
#include "grid_map_core/iterators/LineIterator.hpp"

using namespace grid_map;

int main() {
  // Create grid map
  GridMap map({"layer1"});
  map.setFrameId("map");
  map.setGeometry(
      Length(11., 11.), 1.0,
      Position(0.0, 2.0)); // 1x1 m grid map with 0.1 m/cell resolution.
  map["layer1"].setConstant(0.0);
  std::cout << map.getPosition().transpose() << std::endl;

  // Iterate over a line
  {
    int i = 0;
    const Position start_pos(0.0, 0.0);
    const Position end_pos(3.0, 0.0);
    for (LineIterator it(map, start_pos, end_pos);
         !it.isPastEnd(); ++it) {
      Position position;
      map.getPosition(*it, position);
      std::cout << "Index: " << (*it).transpose()
                << ", position: " << position.transpose()
                << ", value: " << map.at("layer1", *it) << std::endl;
      map.at("layer1", *it) = i++;
    }
  }

  // Print map
  const auto size = map.getSize();
  for (int i = 0; i < size(0); ++i) {
    for (int j = 0; j < size(1); ++j) {
      std::cout << map.at("layer1", Index(i, j)) << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
