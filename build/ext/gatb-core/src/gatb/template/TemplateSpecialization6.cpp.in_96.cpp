
#include <gatb/debruijn/impl/Traversal.cpp>
#include <gatb/debruijn/impl/Terminator.cpp>
#include <gatb/debruijn/impl/Frontline.cpp>
#include <gatb/tools/math/Integer.hpp>

using namespace gatb::core::kmer;
using namespace gatb::core::kmer::impl;

/********************************************************************************/
namespace gatb { namespace core { namespace debruijn { namespace impl  {
/********************************************************************************/

typedef boost::variant<GraphData<96> > GraphDataVariantT;

template class TraversalTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class MonumentTraversalTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class TerminatorTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class MPHFTerminatorTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class BranchingTerminatorTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class FrontlineTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 
template class FrontlineBranchingTemplate <Node_t<Kmer<96>::Type>,Edge_t<Node_t<Kmer<96>::Type > >, GraphDataVariantT>; 

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
