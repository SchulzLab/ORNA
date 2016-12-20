
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/kmer/impl/SortingCountAlgorithm.cpp>
#include <gatb/kmer/impl/PartitionsCommand.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace kmer { namespace impl  {
/********************************************************************************/

template class SortingCountAlgorithm        <32>;
template class PartitionsCommand            <32>;
template class PartitionsByHashCommand      <32>;
template class PartitionsByVectorCommand    <32>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
