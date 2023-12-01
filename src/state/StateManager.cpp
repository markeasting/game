
#include "state/StateManager.h"

void StateManager::set(Ref<State> state) {
    assert(state != nullptr);

    m_state = state;
}

void StateManager::sequence(std::vector<Ref<State>> states) {

    assert(states.size() > 1);

    for (size_t i = 0; i < states.size() - 1; i++) {
        states[i]->setNext(states[i + 1]);
    }

    this->set(states[0]);

}

void StateManager::update(float time, float dt) {
    assert(m_state != nullptr);

    m_state->update(time, dt);

    if (m_state->isComplete())
        this->next();
}

void StateManager::next() {
    assert(m_state != nullptr);

    m_state->complete();
    m_state = m_state->next();

    Log(m_state->getName());

    // Events::emit(Events::STATE_CHANGE, m_state);
    this->emit(E::STATE_CHANGE, m_state);
}

Ref<State> StateManager::current() {
    return m_state;
}

const char* StateManager::getName() {
    assert(m_state != nullptr);

    return m_state->getName();
}

bool StateManager::is(const char* name) {
    assert(m_state != nullptr);

    return m_state->is(name);
}

bool StateManager::inGroup(const char* group) {
    assert(m_state != nullptr);

    return m_state->inGroup(group);
}