// from stack overflow
// http://stackoverflow.com/questions/3247861/example-of-uuid-generation-using-boost-in-c
#include <string>
#include <sstream>
#include <iostream>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


int main() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::cout << uuid << std::endl;
    long long int x;
    // lets go old school, and copy it over
    memcpy ( &x, &uuid, sizeof (x) );
    std::cout << "as a number " << std::hex << x << std::endl;
    return 0;
}
