//---------------------------------------------------------------------------------------------------------------------
// TileMap.cpp
//
// Implementation of the TileMap class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//
#include "TileMap.hpp"
#include "Game.hpp"

//---------------------------------------------------------------------------------------------------------------------
TileMap::TileMap(const std::vector<std::vector<Room *>> rooms)
{
  auto room_row_size = rooms.size();
  auto room_col_size = rooms.at(0).size();

  tile_map_.insert(tile_map_.end(), room_row_size * Room::TILE_ROW_IN_ROOM, std::vector<Tile *>());

  for (size_t room_row = 0; room_row < room_row_size; room_row++)
  {
    for (size_t room_col = 0; room_col < room_col_size; room_col++)
    {
      auto tiles_in_room = rooms.at(room_row).at(room_col)->getRoomMap();
      for (size_t tile_room_row = 0; tile_room_row < Room::TILE_ROW_IN_ROOM; tile_room_row++)
      {
        auto &tile_row_vec = tiles_in_room.at(tile_room_row);
        size_t tile_map_row = (room_row * Room::TILE_ROW_IN_ROOM) + tile_room_row;
        auto &tile_map_row_vec = tile_map_.at(tile_map_row);
        tile_map_row_vec.insert(tile_map_row_vec.end(), tile_row_vec.begin(), tile_row_vec.end());
      }
    }
  }
  tile_map_row_size_ = tile_map_.size();
  tile_map_col_size_ = tile_map_.at(0).size();
  for(int i = 0; i < tile_map_row_size_; i++)
  {
    for(int j = 0; j < tile_map_col_size_; j++)
    {
      Position pos = {i,j};
      getTileAt(pos)->setPosition(pos);
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
int TileMap::getTileMapRowSize() const
{
  return tile_map_row_size_;
}

//---------------------------------------------------------------------------------------------------------------------
int TileMap::getTileMapColumnSize() const
{
  return tile_map_col_size_;
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<Tile *> TileMap::getNeighborTiles(const Position position) const
{
  std::vector<Tile *> neighbors;

  std::string neighbor_directions[] = {Game::UP, Game::DOWN, Game::LEFT, Game::RIGHT};

  for (auto direction : neighbor_directions)
  {
    auto neighbor_position = calculatePosition(1, position, direction);
    try
    {
      auto tile = getTileAt(neighbor_position);
      // TODO: remove this check if we can fix getTileAt, so it throws if no tile at position
      if(!(tile == NULL))
        neighbors.push_back(tile);
    }
    catch (const std::out_of_range &)
    {
    }
  }
  return neighbors;
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<Tile *> TileMap::calculatePath(const int distance, const Position start_position,
                                           const std::string direction) const
{
  std::vector<Tile *> path;

  if (direction == Game::UP)
  {
    for (int step = 1; step <= distance; step++)
    {
      path.push_back(tile_map_.at(start_position.row_ - step).at(start_position.column_));
    }
  }
  else if (direction == Game::DOWN)
  {
    for (int step = 1; step <= distance; step++)
    {
      path.push_back(tile_map_.at(start_position.row_ + step).at(start_position.column_));
    }
  }
  else if (direction == Game::LEFT)
  {
    for (int step = 1; step <= distance; step++)
    {
      path.push_back(tile_map_.at(start_position.row_).at(start_position.column_ - step));
    }
  }
  else if (direction == Game::RIGHT)
  {
    for (int step = 1; step <= distance; step++)
    {
      path.push_back(tile_map_.at(start_position.row_).at(start_position.column_ + step));
    }
  }
  return path;
}

//---------------------------------------------------------------------------------------------------------------------
Position TileMap::calculatePosition(const int distance, const Position start_position,
                                    const std::string direction) const
{
  Position end_position;
  if (direction == Game::UP)
  {
    end_position.row_ = start_position.row_ - distance;
    end_position.column_ = start_position.column_;
  }
  else if (direction == Game::DOWN)
  {
    end_position.row_ = start_position.row_ + distance;
    end_position.column_ = start_position.column_;
  }
  else if (direction == Game::LEFT)
  {
    end_position.column_ = start_position.column_ - distance;
    end_position.row_ = start_position.row_;
  }
  else if (direction == Game::RIGHT)
  {
    end_position.column_ = start_position.column_ + distance;
    end_position.row_ = start_position.row_;
  }
  return end_position;
}

//---------------------------------------------------------------------------------------------------------------------
Tile *TileMap::getTileAt(const Position position) const
{
  Tile* tile;
   try
    {
      tile = tile_map_.at(position.row_).at(position.column_);
    }
    catch (const std::out_of_range &)
    {
      // TODO: try to remove this junk
      return NULL;
    }

  return tile;
}

// TODO: check where is this used
void TileMap::resetVisit()
{
  for(int i = 0; i < tile_map_row_size_; i++)
  {
    for(int j = 0; j < tile_map_col_size_ ;j++)
    {
      Position pos = {i,j};
      getTileAt(pos)->markUnvisit();
    }
  }
  
}

// TODO: try to add new methods instead of returning whole map
std::vector<std::vector<Tile *>> TileMap::getMap()
{
  return tile_map_;
}