
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/debruijn/impl/BranchingAlgorithm.cpp>
#include <gatb/debruijn/impl/IterativeExtensions.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace debruijn { namespace impl  {
/********************************************************************************/

template class BranchingAlgorithm   <64, Node, Edge, GraphDataVariant>;

typedef boost::variant<GraphData<64> > GraphDataVariantT;
template class BranchingAlgorithm <64,Node_t<Kmer<64>::Type>,Edge_t<Node_t<Kmer<64>::Type > >, GraphDataVariantT>;

template class IterativeExtensions  <64>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
