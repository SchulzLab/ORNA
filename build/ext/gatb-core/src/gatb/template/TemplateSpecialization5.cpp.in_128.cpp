
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/debruijn/impl/BranchingAlgorithm.cpp>
#include <gatb/debruijn/impl/IterativeExtensions.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace debruijn { namespace impl  {
/********************************************************************************/

template class BranchingAlgorithm   <128, Node, Edge, GraphDataVariant>;

typedef boost::variant<GraphData<128> > GraphDataVariantT;
template class BranchingAlgorithm <128,Node_t<Kmer<128>::Type>,Edge_t<Node_t<Kmer<128>::Type > >, GraphDataVariantT>;

template class IterativeExtensions  <128>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
