#include "./Container.h"

/**
 * The first time we call instance(), we lock the storage location
 */
Container *Container::instance()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_instance == nullptr) {
        _instance = new Container();
    }

    return _instance;
}
