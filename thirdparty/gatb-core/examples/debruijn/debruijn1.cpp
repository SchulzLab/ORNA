//! [snippet1]

// We include what we need for the test
#include <gatb/gatb_core.hpp>

/********************************************************************************/
/*             Graph creation from command line options                         */
/*                                                                              */
/*  This snippet is similar to the dbgh5 software.                              */
/*  Here, we just show the minimal requirement to create a de Bruijn graph.     */
/*                                                                              */
/********************************************************************************/
int main (int argc, char* argv[])
{
    // We get a command line parser for graphs available options.
    IOptionsParser* parser = Graph::getOptionsParser();
    LOCAL (parser);

    // We use a try/catch block in case we have some command line parsing issue.
    try
    {
        // We parse the user options.
        parser->parse (argc, argv);

        // We create the graph with the provided options.
        Graph graph = Graph::create (parser->getProperties());

        // We dump some information about the graph.
        std::cout << graph.getInfo() << std::endl;
    }
    catch (OptionFailure& e)
    {
        return e.displayErrors (std::cout);
    }
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }

    return EXIT_SUCCESS;
}
//! [snippet1]
