#include "AI.hpp"
#include "Command.hpp"
#include "CommandException.hpp"
#include "Game.hpp"
#include "GameException.hpp"
#include "TileMap.hpp"

AI::AI() :
    game_{Game::getInstance()}
{
}
Position AI::calculatePosition(const int distance, const Position start_position,
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

void AI::playARound()
{
  game_.setAIActivated(true);

  auto tile_map = game_.getTileMap();
  auto direction = game_.getCurrentDirection();

  auto fighter = game_.getFighter();
  auto seer = game_.getSeer();
  auto thief = game_.getThief();

  auto fighter_start_pos = game_.getStartPosition(fighter);
  auto thief_start_pos = game_.getStartPosition(thief);
  auto seer_start_pos = game_.getStartPosition(seer);

  if(thief_start_pos.column_ == fighter->getPosition().column_ && thief_start_pos.row_ == fighter->getPosition().row_)
  {
    thief_start_pos = fighter_start_pos;
  }
  if(seer_start_pos.column_ == fighter->getPosition().column_ && seer_start_pos.row_ == fighter->getPosition().row_)
  {
    seer_start_pos = fighter_start_pos;
  }
  Tile* goal_tile_fighter = chooseGoal(fighter);
  Tile* goal_tile_thief = chooseGoal(thief);
  Tile* goal_tile_seer = chooseGoal(seer);

  if(goal_tile_seer->getRoomReference()->isThereAMonster() && goal_tile_seer->getRoomReference()->getRevealed())
    goal_tile_seer = tile_map->getTileAt(seer_start_pos);
   if( goal_tile_thief->getRoomReference()->getRevealed() && (goal_tile_thief->getRoomReference()->isThereAMonster() && goal_tile_thief->getTileType() != TileType::HORIZONTAL_DOOR && goal_tile_thief->getTileType() != TileType::VERTICAL_DOOR))
    goal_tile_thief = tile_map->getTileAt(thief_start_pos);


  if(goal_tile_thief->getPosition().column_ == goal_tile_seer->getPosition().column_ && goal_tile_thief->getPosition().row_ == goal_tile_seer->getPosition().row_ && goal_tile_seer->getTileType() != TileType::LOOT)
     goal_tile_seer = tile_map->getTileAt(seer_start_pos);

  auto fighter_tile_pos = calculatePath(fighter, goal_tile_fighter);
  auto thief_tile_pos = calculatePath(thief, goal_tile_thief);
  auto seer_tile_pos = calculatePath(seer, goal_tile_seer);

  std::deque<Position> fighter_path;
  std::deque<Position> thief_path;
  std::deque<Position> seer_path;

  fighter_path.push_back(fighter_tile_pos);
  thief_path.push_back(thief_tile_pos);
  seer_path.push_back(seer_tile_pos);

  while(!(fighter_tile_pos.column_ == fighter->getPosition().column_ && fighter_tile_pos.row_ == game_.getFighter()->getPosition().row_))
  {
    fighter_tile_pos = calculatePath(fighter, tile_map->getTileAt(fighter_path.front()));
    fighter_path.push_front(fighter_tile_pos);
  }

  while(!(thief_tile_pos.column_ == thief->getPosition().column_ && thief_tile_pos.row_ == thief->getPosition().row_))
  {
    thief_tile_pos = calculatePath(thief, tile_map->getTileAt(thief_path.front()));
    thief_path.push_front(thief_tile_pos);
  }

  while(!(seer_tile_pos.column_ == seer->getPosition().column_ && seer_tile_pos.row_ == seer->getPosition().row_))
  {
    seer_tile_pos = calculatePath(seer, tile_map->getTileAt(seer_path.front()));
    seer_path.push_front(seer_tile_pos);
  }

  fighter_path.push_back(goal_tile_fighter->getPosition());
  thief_path.push_back(goal_tile_thief->getPosition());
  seer_path.push_back(goal_tile_seer->getPosition());


  if(!(goal_tile_thief->getTileType() == TileType::THIEF && tile_map->getTileAt(fighter->getPosition())->getTileType() != TileType::FIGHTER))
  {
    if(!(tile_map->getTileAt(thief->getPosition())->getTileType() == TileType::LOOT))
      executePath(thief_path, direction, thief);
  }
  if(!(tile_map->getTileAt(fighter->getPosition())->getTileType() == TileType::LOOT))
    executePath(fighter_path, direction, fighter);
  if(!(tile_map->getTileAt(seer->getPosition())->getTileType() == TileType::LOOT))
    executePath(seer_path, direction, seer);

  fighter_path.clear();
  thief_path.clear();
  seer_path.clear();

  executeCommand("flip");

  game_.setAIActivated(false);
}

void AI::executePath(std::deque<Position> champion_path, std::string allowed_direction, Character* selected_champ)
{
  Position active_direction;
  Position move_direction;
  std::string command_string;
  auto tile_map = game_.getTileMap();
  champion_path.pop_front();
  auto current_move = champion_path.front();
  champion_path.pop_front();

  move_direction.column_ = current_move.column_ - selected_champ->getPosition().column_;
  move_direction.row_ = current_move.row_ - selected_champ->getPosition().row_;
  if (allowed_direction == Game::UP)
  {
    active_direction = {-1, 0};
  }
  else if (allowed_direction == Game::DOWN)
  {
    active_direction = {1, 0};
  }
  else if (allowed_direction == Game::LEFT)
  {
    active_direction = {0, -1};
  }
  else if (allowed_direction == Game::RIGHT)
  {
    active_direction = {0, 1};
  }
  while(active_direction.column_ == move_direction.column_ && active_direction.row_ == move_direction.row_ 
          && tile_map->getTileAt(current_move)->getTileType() != WALL && tile_map->getTileAt(current_move)->getTileType() != SECRET_DOOR )
  {
    if((tile_map->getTileAt(current_move)->getTileType() == VERTICAL_DOOR || tile_map->getTileAt(current_move)->getTileType() == HORIZONTAL_DOOR)
                && selected_champ->getCharacterType() == CharacterType::THIEF)
    {
      executeCommand("unlock");
    }
    if(tile_map->getTileAt(current_move)->getRoomReference()->isThereAMonster() && tile_map->getTileAt(current_move)->getTileType() == MONSTER && selected_champ->getCharacterType() == CharacterType::FIGHTER)
    {
      executeCommand("fight");
    }
    else if(tile_map->getTileAt(current_move)->getRoomReference()->isThereAMonster() && selected_champ->getCharacterType() != CharacterType::FIGHTER)
    {
      return;
    }
    
    else if((tile_map->getTileAt(current_move)->getTileType() == VERTICAL_DOOR || tile_map->getTileAt(current_move)->getTileType() == HORIZONTAL_DOOR) 
                && selected_champ->getCharacterType() != CharacterType::THIEF)
    {
      return;
    }
    if(tile_map->getTileAt(current_move)->getCharacter() == NULL)
    {
      command_string = "move ";
      command_string += (char)selected_champ->getCharacterType();
      command_string += " ";
      command_string += allowed_direction;
      executeCommand(command_string);
      current_move = champion_path.front();
      move_direction.column_ = current_move.column_ - selected_champ->getPosition().column_;
      move_direction.row_ = current_move.row_ - selected_champ->getPosition().row_;
      champion_path.pop_front();
    }
    else if(tile_map->getTileAt(current_move)->getCharacter() != NULL)
    {
      auto advanced_move = champion_path.front();
      move_direction.column_ = advanced_move.column_ - current_move.column_;
      move_direction.row_ = advanced_move.row_ - current_move.row_;
      champion_path.pop_front();
      if(active_direction.column_ == move_direction.column_ && active_direction.row_ == move_direction.row_)
      {
        if(tile_map->getTileAt(advanced_move)->getRoomReference()->isThereAMonster() && selected_champ->getCharacterType() != CharacterType::FIGHTER)
        {
          return;
        }
        if(tile_map->getTileAt(advanced_move)->getCharacter() != NULL)
        {
          auto ultimate_move = champion_path.front();
          move_direction.column_ = ultimate_move.column_ - advanced_move.column_;
          move_direction.row_ = ultimate_move.row_ - advanced_move.row_;
          if(active_direction.column_ == move_direction.column_ && active_direction.row_ == move_direction.row_)
          {
            command_string = "move ";
            command_string += (char)selected_champ->getCharacterType();
            command_string += " ";
            command_string += allowed_direction;
            command_string += " 3";
            executeCommand(command_string);
            champion_path.pop_front();
            current_move = champion_path.front();
            move_direction.column_ = current_move.column_ - selected_champ->getPosition().column_;
            move_direction.row_ = current_move.row_ - selected_champ->getPosition().row_;
            champion_path.pop_front();
            continue;
          }
          else 
            return;
        }
        command_string = "move ";
        command_string += (char)selected_champ->getCharacterType();
        command_string += " ";
        command_string += allowed_direction;
        command_string += " 2";
        executeCommand(command_string);
        champion_path.pop_front();
        current_move = champion_path.front();
        move_direction.column_ = current_move.column_ - selected_champ->getPosition().column_;
        move_direction.row_ = current_move.row_ - selected_champ->getPosition().row_;
        champion_path.pop_front();
      }
      else
        return;
    }
  }
}

Tile* AI::chooseGoal(Character* hero)
{
  auto tile_map = game_.getTileMap();
  auto map = tile_map->getMap();
  Tile* goal_tile;
  auto character_pos = hero->getPosition();
  auto char_tile = tile_map->getTileAt(character_pos);
  goal_tile = char_tile;
  for(auto tile_row : map)
  {
    for(auto tile : tile_row)
    {
      if(tile->getRoomReference()->getRevealed() == false && tile->getTileType() != WALL)
      {
        goal_tile = calculateNearestTile(char_tile);
      }
      if(tile->getTileType() == TileType::MONSTER && hero->getCharacterType() == CharacterType::FIGHTER && tile->getRoomReference()->getRevealed())
      {
        return tile;
      }
      else if((tile->getTileType() == TileType::HORIZONTAL_DOOR || tile->getTileType() == TileType::VERTICAL_DOOR) && hero->getCharacterType() == CharacterType::THIEF && tile->getRoomReference()->getRevealed())
      {
        return tile;
      }
      if(hero->getCharacterType() == CharacterType::FIGHTER && tile->getTileType() == TileType::FIGHTER && tile->getRoomReference()->getRevealed())
      {
        goal_tile = tile;
      }
      else if(hero->getCharacterType() == CharacterType::THIEF && tile->getTileType() == TileType::THIEF && tile->getRoomReference()->getRevealed())
      {
        goal_tile = tile;
      }
      else if(hero->getCharacterType() == CharacterType::SEER && tile->getTileType() == TileType::SEER && tile->getRoomReference()->getRevealed() && goal_tile->getPosition().row_ == char_tile->getPosition().row_ && goal_tile->getPosition().column_ == char_tile->getPosition().column_)
      {
        goal_tile = tile;
      }
      if(game_.getSecret() && tile->getTileType() == TileType::LOOT && tile->getCharacter() == NULL)
      {
        return tile;
      }
    }
  }

  //if(goal_tile->getPosition().column_ == char_tile->getPosition().column_)
  return goal_tile;

}

Position AI::calculatePath(Character *someone, Tile* goal_tile)
{
  auto character_position = someone->getPosition();

  auto tile_map = game_.getTileMap();
  std::deque<Position> tiles_to_go;
  std::deque<Position> way_tiles;

  tiles_to_go.push_back(character_position);
  if(goal_tile->getPosition().column_ == character_position.column_ && goal_tile->getPosition().row_ == character_position.row_)
  {
    tile_map->resetVisit();
    return character_position;
  }
  std::string neighbour_directions[] = {Game::DOWN, Game::UP, Game::RIGHT, Game::LEFT};
  tile_map->getTileAt(character_position)->markVisit();
  while(!tiles_to_go.empty())
  {
    Position front = tiles_to_go.front();
    tiles_to_go.pop_front();
    for(auto curr_direction : neighbour_directions)
    {
      Position a = calculatePosition(1, front, curr_direction);

      if(a.row_ < tile_map->getTileMapRowSize() && a.column_ < tile_map->getTileMapColumnSize() && tile_map->getTileAt(a) != NULL
            && !(tile_map->getTileAt(a)->getVisit()) && tile_map->getTileAt(a)->getTileType() != WALL
            && tile_map->getTileAt(a)->getRich() == false && tile_map->getTileAt(a)->getTileType() != SECRET_DOOR )
      {
          if(goal_tile->getPosition().column_ == a.column_ && goal_tile->getPosition().row_ == a.row_)
          {
            tile_map->resetVisit();
            //std::cout << someone->getCharacterName() << ": "<< front.row_ << " and " << front.column_ << std::endl;
            return front;
          }
          tile_map->getTileAt(a)->markVisit();
          tiles_to_go.push_back(a);
      }
    }

  }
  tile_map->resetVisit();
  return character_position;
}

Tile* AI::calculateNearestTile(Tile* char_tile)
{
  auto tile_map = game_.getTileMap();
  std::deque<Tile*> tile_nearby;

  tile_nearby.push_back(char_tile);
  std::string neighbour_directions[] = {Game::DOWN, Game::RIGHT, Game::LEFT, Game::UP};
  char_tile->markVisit();
  while(!tile_nearby.empty())
  {
    Tile* front = tile_nearby.front();
    tile_nearby.pop_front();
    for(auto curr_direction : neighbour_directions)
    {
      Position a = calculatePosition(1, front->getPosition(), curr_direction);
      Tile* tile_a = tile_map->getTileAt(a);
      if(a.row_ < tile_map->getTileMapRowSize() && a.column_ < tile_map->getTileMapColumnSize() && tile_map->getTileAt(a) != NULL
            && !(tile_map->getTileAt(a)->getVisit()) && tile_map->getTileAt(a)->getTileType() != WALL)
      {
          if(tile_a->getRoomReference()->getRevealed() == false && ((front->getTileType() != VERTICAL_DOOR &&
            front->getTileType() != HORIZONTAL_DOOR) ||char_tile->getCharacter()->getCharacterType() == CharacterType::THIEF))
          {
            tile_map->resetVisit();
            return front;
          }
          tile_map->getTileAt(a)->markVisit();
          tile_nearby.push_back(tile_a);
      }
    }
  }
  tile_map->resetVisit();
  return char_tile;
}

void AI::executeCommand(std::string command_string)
{
  Command command(command_string);
  try
  {
    std::cout << "AI: "<< command_string << std::endl;
    command.executeCommand();
  }
  catch (CommandException &exception)
  {
    // Keep silent about misbehaving AI
  }
}
