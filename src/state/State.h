#pragma once

struct State {

    State(const char* name, float duration = 0, const char* group = "")
        : m_name(name), m_duration(duration), m_group(group)
    {}

    ~State() = default;

    /**
     * Reference to the next state in the list - can be NULL!
     */
    Ref<State> next() {
        assert(m_next != nullptr);

        return m_next; 
    }

    State setNext(Ref<State> state) {
        assert(state != nullptr);
        m_next = state; 

        return *this;
    }

    State setDuration(float duration) {
        m_duration = duration;

        return *this;
    }
    
    State setName(const char* name) { m_name = name; return *this; }
    State setGroup(const char* name) { m_group = name; return *this; }

    const char* getName() { return m_name; }
    const char* getGroup() { return m_group; }
    
    bool is(const char* name) { return m_name == name; }
    bool inGroup(const char* group) { return m_group == group; }

    void complete() {
        m_startTime = -1.0f;
        m_completed = true;
    }

    bool isComplete() { 
        return m_completed;
    }

    void update(float time, float dt = 0) {
        if (m_duration > 0) {
            if (m_startTime < 0)
                m_startTime = time;

            m_completed = (time - m_startTime) > m_duration;
        }
    }

protected:

    const char* m_name = "BaseState";
    const char* m_group = "";
    
    Ref<State> m_next = nullptr;

    bool m_completed = false;

    float m_duration = 0.0f;
    float m_startTime = -1.0f;
};


// struct WaitState: public State {

//     const char* getName() override { return "WaitState"; }

// };


// struct TimedState: public State {

//     const char* getName() override { return "TimedState"; }

//     TimedState(const char* name, float duration)
//         : State(name) 
//     {
//         this->setDuration(duration);
//     }

// };
