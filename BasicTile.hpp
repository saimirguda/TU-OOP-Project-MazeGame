//---------------------------------------------------------------------------------------------------------------------
// BaiscTile.hpp
//
// HPP File containing the BasicTile class, methods and attributes of the class.
// Derived class of the Tile class, describing plain tiles without extra functionality.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef BASICTILE_HPP
#define BASICTILE_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

class BasicTile : public Tile
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// The constructor initializes the members with the parameters' values.
  /// @param tile_type Initial value for the member tile_type_
  /// @param tile_in_room Pointer to the room this tile belongs to
  /// @param stay_allowed Initial value for the member stay_allowed_
  /// @param go_through_allowed Initial value for the member go_through_allowed_
  //
  BasicTile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed);

};

#endif