#include "RotaryEncoder.h"

int mod (int a, int b)
{
   if(a >= 0 && a < b) return a;

   if(b < 0) return -mod(-a, -b);   
   
   int ret = a % b;
   if(ret < 0) ret+=b;
   
   return ret;
}

char* state_to_string(int state) {
    if      (state == 0b00) { return (char*) "0b00"; }
    else if (state == 0b01) { return (char*) "0b01"; }
    else if (state == 0b10) { return (char*) "0b10"; }
    else if (state == 0b11) { return (char*) "0b11"; }
    return (char*) "0bERR";
}

char* direction_to_string(Direction direction) {
    if      (direction == Direction::CW)      { return (char*) "CW" ;     }
    else if (direction == Direction::CCW)     { return (char*) "CCW";     }
    else if (direction == Direction::STAY)    { return (char*) "STAY"; }
    else if (direction == Direction::UNSURE)  { return (char*) "UNSURE";  }
    else if (direction == Direction::INVALID) { return (char*) "INVALID"; }
    return (char*) "ERR UNKNOWN STATE";
}

int get_pin_state(int pin_ccw, int pin_cw) {
    int left = digitalRead(pin_ccw);
    int right = digitalRead(pin_cw);

    int pin_state = left << 1 | right << 0;
    return pin_state;
}

IFDEBUG(int last_pin_state = -1);

Direction process_pin_state(int pin_state, int* state, int* state_index, int* state_counter) {
    #ifdef DEBUG
        if (pin_state != last_pin_state) {
            last_pin_state = pin_state;
            //DBG("[RotaryEncoder] pin_state: %s", state_to_string(pin_state));
        }
    #endif
    if (*state == pin_state) return Direction::STAY;

    int next_state_index = mod((*state_index + 1), COUNT_STATE_FOLLOWUP);
    int prev_state_index = mod((*state_index - 1), COUNT_STATE_FOLLOWUP);
    //DBG("[RotaryEncoder] %s [%i %i %i] %s", state_to_string(*state), *state_index, next_state_index, prev_state_index, state_to_string(pin_state));

    if (STATE_FOLLOWUP[next_state_index] == pin_state) {
        DBG("[RotaryEncoder] %s --> %s", state_to_string(*state), state_to_string(pin_state));
        *state_index = next_state_index;
        *state = STATE_FOLLOWUP[next_state_index];
        *state_counter = *state_counter - 1;
        
        if (*state_counter % 4 == 0 && *state == 0b00 || *state == 0b11) {
            *state_counter = 0;
            return Direction::CW;
        }

    } else if (STATE_FOLLOWUP[prev_state_index] == pin_state) {
        DBG("[RotaryEncoder] %s <-- %s", state_to_string(pin_state), state_to_string(*state));
        *state_index = prev_state_index;
        *state = STATE_FOLLOWUP[prev_state_index];
        *state_counter = *state_counter + 1;

        if (*state_counter % 4 == 0 && *state == 0b00 || *state == 0b11) {
            *state_counter = 0;
            return Direction::CCW;
        }
    } else {
        //DBG("[RotaryEncoder] INVALID STATE %s --> %s", state_to_string(*state), state_to_string(pin_state));

        return Direction::INVALID;
    }
    return Direction::UNSURE;
}

RotaryEncoder::RotaryEncoder(int pin_ccw, int pin_cw) : pin_cw(pin_cw), pin_ccw(pin_ccw) {}

void RotaryEncoder::setup() {
  pinMode(pin_cw, INPUT_PULLUP);
  pinMode(pin_ccw, INPUT_PULLUP);
}

void RotaryEncoder::process() {
  int pin_state = get_pin_state(pin_ccw, pin_cw);
  //DBG("[RotaryEncoder] Current pin state: %s", state_to_string(pin_state));
  Direction direction = process_pin_state(pin_state, &state, &state_index, &state_counter);
  if (direction == Direction::CW || direction == Direction::CCW) {
    onTurn(direction);
  }
}

void RotaryEncoder::onTurn(Direction direction) {

  auto dir = direction == Direction::CCW ? "Left" : "Right";
  DBG("[RotaryEncoder] Turned %s", dir);
}
