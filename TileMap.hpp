//---------------------------------------------------------------------------------------------------------------------
// TileMap.hpp
//
// Definition of the TileMap class.
// The TileMap class represents the map of the game, and it makes traversing the map, like accessing of neighboring
// tiles and rooms, or calculating a path easy.
//
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "Room.hpp"
#include <vector>

class TileMap
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor that initializes tile_map_ member based on the rooms parameter
  ///
  /// @param rooms Rooms that are used to create the tile map
  //
  TileMap(const std::vector<std::vector<Room *>> rooms);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy construnctor is deleted.
  //
  TileMap(const TileMap &) = delete;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy assignment operator is deleted.
  //
  TileMap &operator=(const TileMap &) = delete;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gives back a path of tile objects based on a starting position in the tile map, a distance and a direction.
  ///
  /// @param distance Distance of the path to calculate
  /// @param start_position Start position of the path in the tile map
  /// @param direction Direction of the path
  ///
  /// @return Path of tile objects
  //
  std::vector<Tile *> calculatePath(const int distance, const Position start_position,
                                    const std::string direction) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Calculates an end position in the tile map, based on a starting position, a distance and a direction.
  ///
  /// @param distance Distance of the postion to calculate
  /// @param start_position Start position in the tile map
  /// @param direction Direction of movement
  ///
  /// @return End position
  //
  Position calculatePosition(const int distance, const Position start_position, const std::string direction) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets a tile at the given position
  ///
  /// @param position Position of the tile in the tile map
  ///
  /// @return The tile at the given position
  //
  Tile *getTileAt(const Position position) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets the row size of the tile map
  ///
  /// @return Row size of the tile map
  //
  int getTileMapRowSize() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets the column size of the tile map
  ///
  /// @return Column size of the tile map
  //
  int getTileMapColumnSize() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Collects all neighbor tiles of the specified position (4-neighborhood).
  ///
  /// @param position Position of the tile which neighborhood to collect
  ///
  /// @return The neighbor tiles
  //
  std::vector<Tile *> getNeighborTiles(const Position position) const;

  void resetVisit();

  std::vector<std::vector<Tile *>> getMap();

private:
  std::vector<std::vector<Tile *>> tile_map_;
  int tile_map_row_size_;
  int tile_map_col_size_;
};

#endif