
// since we didn't define the functions in a .h file, that trick removes linker errors,
// see http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html

#include <gatb/kmer/impl/BloomAlgorithm.cpp>
#include <gatb/kmer/impl/DebloomAlgorithm.cpp>
#include <gatb/kmer/impl/DebloomMinimizerAlgorithm.cpp>

/********************************************************************************/
namespace gatb { namespace core { namespace kmer { namespace impl  {
/********************************************************************************/

template class BloomAlgorithm               <96>;
template class DebloomAlgorithm             <96>;
template class DebloomMinimizerAlgorithm    <96>;

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/
