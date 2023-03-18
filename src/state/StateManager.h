#pragma once

#include "common.h"
#include "state/State.h"

class StateManager {
public:

    StateManager() = default;

    void set(Ref<State> state) {
        assert(state != nullptr);

        m_state = state;
    }

    void sequence(std::vector<Ref<State>> states) {

        assert(states.size() > 1);

        for (size_t i = 0; i < states.size() - 1; i++) {
            states[i]->setNext(states[i + 1]);
        }

        this->set(states[0]);

    }

    void update(float time, float dt) {
        assert(m_state != nullptr);

        m_state->update(time, dt);

        if (m_state->isComplete())
            m_state = m_state->next();
    }
    
    void next() {
        assert(m_state != nullptr);

        m_state->complete();
        m_state = m_state->next();
    }

    Ref<State> current() {
        return m_state;
    }

    const char* getCurrentStateName() {
        assert(m_state != nullptr);

        // return typeid(*m_state).name();
        return m_state->getName();
    }

    bool is(const char* name) {
        assert(m_state != nullptr);

        return m_state->getName() == name;
    }

private:
    Ref<State> m_state = nullptr;

};
