#include <trk/Demo.hpp>
#include <trk/PrivateDemo.hpp>

auto trk::Demo::message() -> char * {
    return PrivateDemo::message();
}
