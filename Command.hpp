//---------------------------------------------------------------------------------------------------------------------
// Command.hpp
//
// Definition of the Command class.
// The Command class represents a user command of the game, and responsible for processing and executing the command
// based on the game rules defined by the assignment.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Game.hpp"
#include "Tile.hpp"
#include <string>
#include <vector>

class Command
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor that initializes command_tokens_ with the preprocessed command string and stores a reference
  /// to the singleton game object.
  ///
  /// @param command_string command string representing this command
  //
  Command(const std::string &command_string);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy construnctor is deleted.
  //
  Command(const Command &) = delete;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy assignment operator is deleted.
  //
  Command &operator=(const Command &) = delete;

  ~Command() = default;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the command tokens and executes the corresponding command.
  //
  void executeCommand();

private:
  std::vector<std::string> command_tokens_;
  Game &game_;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Preprocesses the command string creating uppercase tokens for each part separated by whitespace.
  ///
  /// @param command_string The command string
  ///
  /// @return The resulting tokens
  //
  // TODO: check why const was removed
  std::vector<std::string> preprocessCommand(const std::string &command_string);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the move command with the given parameters.
  ///
  /// @param params Parameters for the command
  //
  void executeMove(const std::vector<std::string> params) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the scry command with the given parameters.
  ///
  /// @param params Parameters for the command
  //
  void executeScry(const std::vector<std::string> params) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the fight command with the given parameters.
  ///
  /// @param params Parameters for the command
  //
  void executeFight() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the unlock command with the given parameters.
  ///
  /// @param params Parameters for the command
  //
  void executeUnlock() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Reveals the neighboring rooms of a tile at the given position, if they are not revealed yet.
  ///
  /// @param position The position of the tile whose neighborhood is revealed
  //
  void checkNeighborhoodForReveal(const Position position) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks whether all characters are standing on the corresponding switch at the same time.
  /// If so, the secret doors open.
  ///
  /// @param fighter_tile_type The type of the tile the fighter is standing on
  /// @param seer_tile_type The type of the tile the seer is standing on
  /// @param thief_tile_type The type of the tile the thief is standing on
  //
  void openSecretDoors(const TileType fighter_tile_type, const TileType seer_tile_type,
                       const TileType thief_tile_type) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks whether all three character is standing on a treasure type tile. If yes, the player wins.
  ///
  /// @param fighter_tile_type The type of the tile the fighter is standing on
  /// @param seer_tile_type The type of the tile the seer is standing on
  /// @param thief_tile_type The type of the tile the thief is standing on
  //
  void checkWin(const TileType fighter_tile_type, const TileType seer_tile_type, const TileType thief_tile_type) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks whether the given tile is a sandwatch, and triggers its special function if so.
  ///
  /// @param tile The tile to check
  //
  void checkForSandWatch(Tile *tile) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks the validity of the move parameters given by the player. The first parameter has to be a valid player,
  /// the second a valid direction which is identical with the current_direction member of the Game class.
  /// If there is a third parameter, it has to be a positive integer number.
  ///
  /// @param params The parameters to check.
  /// @param param_size The size of the params vector
  /// @param distance The distance as an output parameter
  //
  void checkMoveParameters(const std::vector<std::string>, const size_t param_size, int &distance) const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Checks the validity of the given path for the playing character, enforcing the various rules
  /// defined by the game description.
  ///
  /// @param path The path of tiles to check
  /// @param playing_character The character for which the path shall be checked
  //
  void checkPath(std::vector<Tile *> path, Character *playing_character) const;

  void checkIfParamsEmpty(const std::vector<std::string> params) const;
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the whoami command printing team name
  //
  void executeWhoami() const;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the play command
  //
  void executePlay() ;

};

#endif