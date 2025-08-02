#include <tomurcuk/Demo.hpp>
#include <tomurcuk/PrivateDemo.hpp>

auto tomurcuk::Demo::message() -> char * {
    return PrivateDemo::message();
}
