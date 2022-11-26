//---------------------------------------------------------------------------------------------------------------------
// MagicTile.hpp
//
// HPP File containing the MagicTile class, methods and attributes of the class.
// Derived class of the Tile class, describing magic tiles with extra functionality.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef MAGICTILE_HPP
#define MAGICTILE_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

class MagicTile : public Tile
{
private:
  bool magic_used_;

public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// The constructor initializes the members with the parameters' values.
  /// @param TileType Initial value for the member tile_type_
  /// @param Room Pointer to the room this tile belongs to
  /// @param bool Initial value for the member stay_allowed_
  /// @param bool Initial value for the member go_through_allowed_
  //
  MagicTile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Convert a magic tile to a passage type tile.
  //
  void magicUsed();
};

#endif