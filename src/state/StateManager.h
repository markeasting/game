#pragma once

#include "common.h"
#include "state/State.h"

#include <vector>

class StateManager {
public:

    StateManager() = default;

    void set(Ref<State> state);

    void sequence(std::vector<Ref<State>> states);

    void update(float time, float dt);
    
    void next();

    Ref<State> current();

    const char* getName();

    bool is(const char* name);
    bool inGroup(const char* group);

private:
    Ref<State> m_state = nullptr;

};
