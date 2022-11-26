//---------------------------------------------------------------------------------------------------------------------
// Tile.hpp
//
// Definition of the Tile class.
// The Tile class represents the tiles of the game.
// It's the base class of MagicTile and BasicTile classes.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef TILE_HPP
#define TILE_HPP

#include "Character.hpp"
#include <string>
#include <vector>

class Room;

enum TileType
{
  WALL = 'W',
  PASSAGE = 'P',
  SECRET_DOOR = 'X',
  MONSTER = 'M',
  SAND_WATCH = 'G',
  CRYSTAL_BALL = 'C',
  HORIZONTAL_DOOR = 'H',
  VERTICAL_DOOR = 'V',
  LOOT = 'L',
  THIEF = 'T',
  FIGHTER = 'F',
  SEER = 'S'
};

class Tile
{

protected:
  static const int TILE_LINES = 3;
  static const int TILE_CHARACTER_POS = 3;
  static const int TILE_REP_CHARACTER_POS = 11;
  Character *character_ = NULL;
  TileType tile_type_;
  Room *tile_in_room_;
  Position tile_position_;
  bool stay_allowed_;
  bool go_through_allowed_;
  std::vector<std::string> tile_represent_;
  bool visited_;
  bool rich_ = false;
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// The constructor initializes the members with the parameters' values.
  /// @param tile_type Initial value for the member tile_type_
  /// @param tile_in_room Pointer to the room this tile belongs to
  /// @param stay_allowed Initial value for the member stay_allowed_
  /// @param go_through_allowed Initial value for the member go_through_allowed_
  //
  Tile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Default destructor as we do not need to free dynamic memory.
  //
  virtual ~Tile() = default;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets the string representing the tile.
  ///
  /// @return string representing the tile.
  //
  std::string getTileString();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Sets the room name to the corresponding Tile
  ///
  /// @param room_name ASCII character representing the room
  //
  void setRoomName(char room_name);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Sets the character to the corresponding Tile
  ///
  /// @param character Character to set
  //
  void setCharacter(Character *character);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for character_ member
  ///
  /// @return Pointer to the member character_
  //
  Character *getCharacter();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets the room which contains this tile.
  ///
  /// @return Pointer to Room object containing this tile.
  //
  Room *getRoomReference() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for stay_allowed_ member
  ///
  /// @return The value of the member stay_allowed_
  //
  bool isStayAllowed() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for go_through_allowed_ member
  ///
  /// @return The value of the member go_through_allowed_
  //
  bool isGoThroughAllowed() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for tile_type_ member
  ///
  /// @return The value of the member tile_type_
  //
  TileType getTileType() const;

  Position getPosition();

  void setPosition(Position pos);

  void markVisit();

  bool getVisit();

  void markUnvisit();

  void gotTreasure();

  bool getRich();
};

#endif
