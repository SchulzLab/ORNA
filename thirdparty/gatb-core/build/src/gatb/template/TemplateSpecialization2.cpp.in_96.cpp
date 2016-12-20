
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/kmer/impl/SortingCountAlgorithm.cpp>
#include <gatb/kmer/impl/PartitionsCommand.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace kmer { namespace impl  {
/********************************************************************************/

template class SortingCountAlgorithm        <96>;
template class PartitionsCommand            <96>;
template class PartitionsByHashCommand      <96>;
template class PartitionsByVectorCommand    <96>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
