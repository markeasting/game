#pragma once

#include "common_includes.h"
#include "camera/Camera.h"

class Scene {
public:

    Scene();
    // virtual ~Scene() = 0;

    // EntityManager m_entityManager = EntityManager();

    void activate()   { m_isActive = true;  }
    void deactivate() { m_isActive = false; }
    bool isActive()   { return m_isActive;  }

    // Called when scene initially created. Called once.
    virtual void create() = 0;

    // Called when scene destroyed. Called at most once (if a scene
    // is not removed from the game, this will never be called).
    virtual void destroy() = 0;

    // Called whenever a scene is transitioned into. Can be
    // called many times in a typical game cycle.
    virtual void onActivate(){};

    // Called whenever a transition out of a scene occurs.
    // Can be called many times in a typical game cycle.
    virtual void onDeactivate(){};

    // The below functions can be overridden as necessary in our scenes.
    virtual void processInput(){};
    virtual void update(float time, float deltaTime){};
    virtual void draw(){};

    Camera getCamera() { return m_camera; }

protected:
    bool m_isActive = false;

    Camera m_camera;

};
