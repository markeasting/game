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
            this->next();
    }
    
    void next() {
        assert(m_state != nullptr);

        m_state->complete();
        m_state = m_state->next();

        Log(m_state->getName());

        Events::emit(Events::STATE_CHANGE, m_state);
    }

    Ref<State> current() {
        return m_state;
    }

    const char* getName() {
        assert(m_state != nullptr);

        return m_state->getName();
    }

    bool is(const char* name) {
        assert(m_state != nullptr);

        return m_state->is(name);
    }

    bool inGroup(const char* group) {
        assert(m_state != nullptr);

        return m_state->inGroup(group);
    }

private:
    Ref<State> m_state = nullptr;

};
