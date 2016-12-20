//! [snippet1]

// We include what we need for the test
#include <gatb/gatb_core.hpp>

using namespace std;

/********************************************************************************/
/*                    Multithreaded iteration of an integer range               */
/********************************************************************************/

// We define a functor that will be cloned by the dispatcher
struct Functor { void operator() (int i)
{
    // In this instruction block, we are executing in one of the nbCores threads
    // created by the dispatcher. Note that 'i' is one value of our range
}};

/********************************************************************************/
int main (int argc, char* argv[])
{
    // We get the number of cores to be used.  If we don't give any number,
    // we set to 0 which implies the usage of all available cores
    size_t nbCores = (argc >=2 ? atoi(argv[1]) : 0);

    // We create an iterator over an integer range
    Range<int>::Iterator it (1,1000);

    // We create a dispatcher configured for 'nbCores' cores.
    Dispatcher dispatcher (nbCores);

    // We dispatch the range iteration with the dispatcher.
    // This will create nbCores threads and each thread will be fed with
    // one value of the defined range

    // NOTE: we could also use lambda expression (easing the code readability)
    IDispatcher::Status status = dispatcher.iterate (it, Functor());

    // We dump some information about the dispatching
    cout << "nbCores=" << status.nbCores << "  time=" << status.time << endl;

    // IMPORTANT: usage of Dispatcher has sense only if the iterated items
    // can be processed independently from each other.

    // The point to understand with the Dispatcher is that it can
    // iterate any instance of Iterator class. If you have any set of items
    // that can be enumerated through an Iterator implementation, then you
    // can parallelize the iteration with a Dispatcher instance
}
//! [snippet1]
