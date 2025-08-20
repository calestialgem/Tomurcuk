#include "PrivateDemo.hpp"

#include <tomurcuk/demo/Demo.hpp>

auto tomurcuk::Demo::message() -> char * {
    return PrivateDemo::message();
}
