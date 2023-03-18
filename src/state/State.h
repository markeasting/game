#pragma once

struct State {

    State(const char* name, float duration = 0)
        : m_name(name), m_duration(duration)
    {}

    ~State() = default;

    const char* getName() { return m_name; }
    void setName(const char* name) { m_name = name; }

    Ref<State> next() {
        assert(m_next != nullptr);

        this->complete();

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
