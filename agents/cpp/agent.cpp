#include "game_state.hpp"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <random>

const std::vector<std::string> _actions = {"up", "left", "right", "down", "bomb", "detonate"};

class Agent {
private:
  static std::string my_agent_id;
  static std::vector<std::string> my_units;
  static std::string enemy_agent_id;
  static std::vector<std::string> enemy_units;
  static std::vector<std::vector<std::string>> map;

  static int tick;

  static void on_game_tick(int tick, const json& game_state);
public:
  static void run();
};

std::string Agent::my_agent_id;
std::vector<std::string> Agent::my_units;
std::string Agent::enemy_agent_id;
std::vector<std::string> Agent::enemy_units;
std::vector<std::vector<std::string>> Agent::map;

int Agent::tick;

void Agent::run() 
{
  const char* connection_string = getenv ("GAME_CONNECTION_STRING");

  if (connection_string == NULL) 
  {
    connection_string = "ws://127.0.0.1:3000/?role=agent&agentId=agentId&name=defaultName";
  }
  std::cout << "The current connection_string is: " << connection_string << std::endl;

  GameState::connect(connection_string); 
  GameState::set_game_tick_callback(on_game_tick);
  GameState::handle_messages();
}

void Agent::on_game_tick(int tick_nr, const json& game_state) 
{
  DEBUG("*************** on_game_tick");
  DEBUG(game_state.dump());
  TEST(tick_nr, game_state.dump());
  
  tick = tick_nr;
  std::cout << "Tick #" << tick << std::endl;
  if (tick == 1)
  {
    my_agent_id = game_state["connection"]["agent_id"];
    my_units = game_state["agents"][my_agent_id]["unit_ids"].get<std::vector<std::string>>();
    enemy_agent_id = (my_agent_id == "a" ? "b" : "a");
    enemy_units = game_state["agents"][enemy_agent_id]["unit_ids"].get<std::vector<std::string>>();
  }

  //

  srand(1234567 * (my_agent_id == "a" ? 1 : 0) + tick * 100 + 13);
  // srand(1234567 * (enemy_agent_id == "a" ? 1 : 0) + tick * 100 + 13);

  // send each (alive) unit a random action
  for (const auto& unit_id: my_units)
  {
    const json& unit = game_state["unit_state"][unit_id];
    //como o desafio só terá um inimigo, então só pego o primeiro inimigo
    const json& enemy = game_state["unit_state"][enemy_units[0]];
    if (enemy["hp"] != 0){
    }
    if (unit["hp"] <= 0) continue;
    //escolha aleatoria
    std::string action = _actions[rand() % _actions.size()];
    std::cout << "action: " << unit_id << " -> " << action << std::endl;

    //envio do comando para o jogo
    if (action == "up" || action == "left" || action == "right" || action == "down")
    {
      GameState::send_move(action, unit_id);
    }
    else if (action == "bomb")
    {
      GameState::send_bomb(unit_id);
    }
    else if (action == "detonate")
    {
      const json& entities = game_state["entities"];
      for (const auto& entity: entities) 
      {
        if (entity["type"] == "b" && entity["unit_id"] == unit_id) 
        {
          int x = entity["x"], y = entity["y"];
          GameState::send_detonate(x, y, unit_id);
          break;
        }
      }
    }
    else 
    {
      std::cerr << "Unhandled action: " << action << " for unit " << unit_id << std::endl;
    }
  }
}

int main()
{
  Agent::run();
}

