#include <tomurcuk/Demo.hpp>
#include <tomurcuk/PrivateDemo.hpp>

auto trk::Demo::message() -> char * {
    return PrivateDemo::message();
}
