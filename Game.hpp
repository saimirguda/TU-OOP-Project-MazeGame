//---------------------------------------------------------------------------------------------------------------------
// Game.hpp
//
// Definition of the Game class.
// The Game class represents the game state, and responsible for running the game.
// It's implemented as a singleton class, and can be accessed by the various other classes
// for general game information.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAME_HPP
#define GAME_HPP

#include "Character.hpp"
#include "Room.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include <memory>
#include <string>
#include <vector>

class Game
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gets a reference to the only instance of the singleton Game object.
  //
  static Game &getInstance()
  {
    static Game instance_;
    return instance_;
  }
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Destructor that frees dynamic memory.
  ///
  //
  ~Game();

  static const std::string UP;
  static const std::string DOWN;
  static const std::string LEFT;
  static const std::string RIGHT;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Fill rooms_ and tile_map_ members with the rooms given as rows of room IDs
  ///
  /// @param rooms rows of room IDs that represent the game map
  //
  void updateGameMap(const std::vector<std::string> &rooms);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Prepares the game by shuffling the direction deck, greeting the player and printing the initial game map.
  //
  void prepareGame();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Runs the game by reading and executing commands from the user until the game is won or user exits.
  //
  void runGame();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Updates the member current_direction_ with the first element of member cards_order_. The first element of
  /// cards_order_ becomes the last element of cards_order_
  //
  void flipCard();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Decreases the member flip_counter_ by 5, or sets it to 0 if it was smaller than 5.
  //
  void rollBackFlipCounter();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Prints out the game map on the console
  //
  void printGameMap() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member map_activated_.
  ///
  /// @return The value of map_activated_
  //
  bool getMapActivated() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Setter for member map_activated_.
  ///
  /// @param map_activated value to set
  //
  void setMapActivated(const bool map_activated);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member tile_map_.
  ///
  /// @return The raw pointer from tile_map_
  //
  TileMap *getTileMap() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member current_direction_.
  ///
  /// @return The value of current_direction_
  //
  std::string getCurrentDirection() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member flip_counter_.
  ///
  /// @return The value of flip_counter_
  //
  int getFlipCounter() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gives a pointer to a specific member character based on a character string
  ///
  /// @param character_string The string to select the character
  ///
  /// @return Pointer to the character selected by the parameter
  //
  Character *selectCharacter(const std::string character_string);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member fighter_.
  ///
  /// @return Pointer to fighter_
  //
  Character *getFighter();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member thief_.
  ///
  /// @return Pointer to of thief_
  //
  Character *getThief();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for member seer_.
  ///
  /// @return Pointer to of seer_
  //
  Character *getSeer();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Finds a room by its ID in rooms_ member if it is part of it
  ///
  /// @param room_id The room_id of the room to find
  ///
  /// @return Position of the room in rooms_ member
  //
  Position findRoomById(const std::string room_id) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Gives back the room at given position in the rooms_ member.
  ///
  /// @param position The position of the room
  ///
  /// @return Room at the given position
  //
  Room *getRoomAt(const Position position) const;

  void  secretUnlock();

  bool getSecret();

  Position getStartPosition(Character* charchar);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Set AI activated operation. Game will not print the map if AI is activated.
  /// @param enable Selects if AI is activated or not.
  //
  void setAIActivated(bool enable);

private:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Private constructor for Singleton Game object. Initializes member variables to default values.
  ///
  //
  Game();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy constructor is deleted.
  ///
  //
  Game(const Game &) = delete;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy assignment operator is deleted.
  ///
  //
  Game &operator=(const Game &) = delete;

  static const std::vector<std::string> CARDS;
  static const std::string HORIZONTAL_ROOM_SEPARATOR;
  static const std::string HORIZONTAL_ROOM_SEPARATOR_START;
  static const std::string VERTICAL_ROOM_SEPARATOR;
  static const int NUMBER_OF_LINES_IN_ROOM;

  std::string current_direction_;
  std::vector<std::string> cards_order_;
  std::vector<std::vector<Room *>> rooms_;
  std::unique_ptr<TileMap> tile_map_;

  Character fighter_;
  Character seer_;
  Character thief_;

  Position start_fighter_;
  Position start_thief_;
  Position start_seer_;

  int flip_counter_;
  bool map_activated_;
  bool doors_unlocked_ = false;
  bool ai_activated_ = false;
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks whether the rooms satisfy the defined requirements
  ///
  /// @param rooms rooms of the game map as rows of room ID characters
  //
  void checkRoomValidity(const std::vector<std::string> rooms) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Creates the initial order of the direction cards (saved in cards_order_ member).
  //
  void createCardOrder();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Increases the flip counter by 1.
  //
  void increaseFlipCounter();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Greets the player on the console
  //
  void greetPlayer() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Reads the player's input from console
  ///
  /// @return input read from console
  //
  std::string readInput() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the prompt sign " > " on the console
  //
  void printPrompt() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Places the characters into their initial position in the given room
  ///
  /// @param room Pointer to the room where the characters should be placed
  //
  void placeCharacters(Room *room);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Sets all character's initial position in the starting room at the given position in the tile_map_ member.
  ///
  /// @param row_index The row index of the starting room in rooms_ member
  /// @param column_index The column index of the starting room in rooms_ member
  //
  void setCharactersPosition(const int row_index, const int column_index);



};



#endif
