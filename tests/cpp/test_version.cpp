#include "lumen/version.hpp"

#include <cassert>

int main() {
    assert(lumen::version_major() == 0);
    return 0;
}