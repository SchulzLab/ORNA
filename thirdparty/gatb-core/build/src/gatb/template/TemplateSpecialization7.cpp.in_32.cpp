#include <gatb/debruijn/impl/Simplifications.cpp>
#include <gatb/debruijn/impl/Graph.cpp>

#include <gatb/tools/math/Integer.hpp>

using namespace gatb::core::kmer;
using namespace gatb::core::kmer::impl;

/********************************************************************************/
namespace gatb { namespace core { namespace debruijn { namespace impl  {
/********************************************************************************/

typedef boost::variant<GraphData<32> > GraphDataVariantT;

template struct Node_t<Kmer<32>::Type >; 
template struct Edge_t<Node_t<Kmer<32>::Type > >; 
template class GraphTemplate<Node_t<Kmer<32>::Type>,Edge_t<Node_t<Kmer<32>::Type > >, GraphDataVariantT>; 

template class Simplifications <Node_t<Kmer<32>::Type>,Edge_t<Node_t<Kmer<32>::Type > >, GraphDataVariantT>; 

// special instantiation (or specialization, i never know what's the difference)
// actually not sure if it's ever used
template <> template <> const Kmer<32>::Type& Node_t<Kmer<32>::Type>::getKmer() const
{
    return kmer;
}





/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
