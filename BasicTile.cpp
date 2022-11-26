//---------------------------------------------------------------------------------------------------------------------
// BasicTile.cpp
//
// Implementation of the BasicTile class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//
#include "BasicTile.hpp"
#include <string>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
BasicTile::BasicTile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed) :
    Tile(tile_type, tile_in_room, stay_allowed, go_through_allowed)
{
  tile_type_ = tile_type;
  std::string line;
  switch (static_cast<char>(tile_type_))
  {
    case TileType::WALL:
      line = "███████";
      break;
    case TileType::PASSAGE:
      line = "       ";
      break;
  }
  for (int tile_line = 0; tile_line < TILE_LINES; tile_line++)
  {
    tile_represent_.push_back(line);
  }
}
