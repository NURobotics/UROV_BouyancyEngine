#include <Arduino.h>

class BuoyancyEngine
{
public:
    enum class State
    {
        Init,
        IdleUp,
        ActuatingDescend,
        Descending,
        IdleDown,
        ActuatingAscend,
        Ascending,
        TransmittingUp
    };

    BuoyancyEngine(uint32_t init_time = 30000,
                   uint32_t descend_time = 30000,
                   uint32_t ascend_time = 30000,
                   uint32_t descend_actuate_time = 1000,
                   uint32_t ascend_actuate_time = 1000,
                   uint32_t transmit_time = 1000) : init_time_{init_time},
                                                    descend_time_{descend_time},
                                                    ascend_time_{ascend_time},
                                                    descend_actuate_time_{descend_actuate_time},
                                                    ascend_actuate_time_{ascend_actuate_time},
                                                    transmit_time_{transmit_time} {};

    void Init()
    {
        // turn on linear actuator
        current_state_ = State::Init;
        state_entry_time_ = millis();
    }

    State GetState()
    {
        return current_state_;
    }

    void Tick()
    {
        switch (current_state_)
        {
        case State::Init:
            if (state_entry_time_ + ascend_actuate_time_ < millis())
            {
                // turn off linear actuators
            }
            if (state_entry_time_ + init_time_ < millis())
            {
                // turn off linear actuators
                current_state_ = State::IdleUp;
                state_entry_time_ = millis();
            }
            break;
        case State::IdleUp:
            if (requested_action_ == Action::Descend)
            {
                // turn on linear actuator to descend
                current_state_ = State::ActuatingDescend;
                state_entry_time_ = millis();
            }
            break;
        case State::ActuatingDescend:
            if (state_entry_time_ + descend_actuate_time_ < millis())
            {
                // turn off linear actuator
                current_state_ = State::Descending;
                state_entry_time_ = millis();
            }
            break;
        case State::Descending:
            if (state_entry_time_ + descend_time_ < millis())
            {
                current_state_ = State::IdleDown;
                state_entry_time_ = millis();
            }
            break;
        case State::IdleDown:
            if (requested_action_ == Action::Ascend)
            {
                // turn on linear actuator to ascend
                current_state_ = State::ActuatingAscend;
                state_entry_time_ = millis();
            }
            break;
        case State::ActuatingAscend:
            if (state_entry_time_ + ascend_actuate_time_ < millis())
            {
                // turn off linear actuator
                current_state_ = State::Descending;
                state_entry_time_ = millis();
            }
            break;
        case State::Ascending:
            if (state_entry_time_ + ascend_time_ < millis())
            {
                current_state_ = State::TransmittingUp;
                state_entry_time_ = millis();
            }
            break;
        case State::TransmittingUp:
            if (state_entry_time_ + transmit_time_ < millis())
            {
                current_state_ = State::IdleUp;
                state_entry_time_ = millis();
            }
            break;
        }
    };

    void Descend() { requested_action_ = Action::Descend; };

    void Ascend() { requested_action_ = Action::Ascend; };

private:
    uint32_t init_time_;
    uint32_t descend_time_; // unit millis
    uint32_t ascend_time_;
    uint32_t descend_actuate_time_;
    uint32_t ascend_actuate_time_;
    uint32_t transmit_time_;

    enum class Action
    {
        Descend,
        Ascend,
        Idle
    };

    State current_state_;
    Action requested_action_;

    uint32_t state_entry_time_;
};