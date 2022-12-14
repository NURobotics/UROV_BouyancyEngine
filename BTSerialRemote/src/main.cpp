#include <Arduino.h>
#include "BTTimeTransmitter.h"
#include "BuoyancyEngine.h"
#include <array>
#include <tuple>
#include <functional>

#define SDA 21
#define SCL 22
// uint8_t mac_addr[6] = {0x80, 0x08, 0x1E, 0x54, 0x20, 0x69};

BTTimeTransmitter transmitter{};
BuoyancyEngine buoyance_engine{};

std::array<std::tuple<BuoyancyEngine::State, std::function<void(void)>>, 4> state_action_values{{std::make_tuple(BuoyancyEngine::State::IdleUp, []()
                                                                                                                 { buoyance_engine.Descend(); }),
                                                                                                 std::make_tuple(BuoyancyEngine::State::IdleDown, []()
                                                                                                                 { buoyance_engine.Ascend(); }),
                                                                                                 std::make_tuple(BuoyancyEngine::State::IdleUp, []()
                                                                                                                 { buoyance_engine.Descend(); }),
                                                                                                 std::make_tuple(BuoyancyEngine::State::IdleDown, []()
                                                                                                                 { buoyance_engine.Ascend(); })}};

int current_state = 0;

void setup()
{
  // esp_base_mac_addr_set(mac_addr);
  Serial.begin(9600);
  transmitter.Init(SDA, SCL);
  buoyance_engine.Init();
}

void loop()
{
  transmitter.Tick();
  buoyance_engine.Tick();
  if (current_state < state_action_values.size() && buoyance_engine.GetState() == std::get<0>(state_action_values[current_state]))
  {
    std::get<1>(state_action_values[current_state])();
    current_state++;
  }
  delay(1);
}