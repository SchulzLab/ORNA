
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/kmer/impl/SortingCountAlgorithm.cpp>
#include <gatb/kmer/impl/PartitionsCommand.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace kmer { namespace impl  {
/********************************************************************************/

template class SortingCountAlgorithm        <64>;
template class PartitionsCommand            <64>;
template class PartitionsByHashCommand      <64>;
template class PartitionsByVectorCommand    <64>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
