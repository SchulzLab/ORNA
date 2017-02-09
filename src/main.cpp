/*
	Optimal Read Normalization Algorithm:
	Developed by : Dilip A Durai and Marcel H Schulz
*/

#include <gatb/gatb_core.hpp>
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>

typedef Kmer<>::Type bloom;
static const char* STR_BASE = "-base";
static const char* STR_INPUT = "-input";
static const char* STR_OUTPUT = "-output";
static const char* STR_KMER = "-kmer";

int readacceptance(Graph graph, Kmer<>::ModelCanonical::Iterator itKmer, Kmer<>::ModelCanonical model, unsigned int *counter, double base){
	int acceptance=0;
	for (itKmer.first(); !itKmer.isDone(); itKmer.next())
	{
		std::string s = model.toString (itKmer->value());
		const char* sq = s.c_str();
		Node node = graph.buildNode(sq);
		double threshold=0;
		//Checking whether the node exists.
		auto index = graph.nodeMPHFIndex(node);
		auto abund = graph.queryAbundance(node);
		threshold = ceil((log(abund)/log(base)));
		if(threshold>abund)
		{
			threshold=abund;
		}
		if(threshold<1)
		{
			threshold=1;
		}
		if(counter[index] < threshold)
		{
			__sync_fetch_and_add (&acceptance, 1);
			//__sync_fetch_and_add (&counter[index], 1);
	       	}
	}
	return acceptance;
}


class ORNA : public Tool
{
public:
	ORNA(): Tool("ORNA")
	{
		getParser()->push_front (new OptionOneParam (STR_KMER, "kmer required",  false, "21"));
	        getParser()->push_front (new OptionOneParam (STR_INPUT, "Input File",  true));
	        getParser()->push_front (new OptionOneParam (STR_OUTPUT, "Output File",  true));
		getParser()->push_front (new OptionOneParam (STR_BASE, "Base for the logarithmic function",  true));
	
	}
	void execute()
	{
		int count=0;
		double base=getInput()->getDouble(STR_BASE);
		int user_kmer = getInput()->getInt(STR_KMER);
		int kmer = user_kmer + 1; 
		const char* filename = (getInput()->get(STR_INPUT))->getString();
		const char* out_file= (getInput()->get(STR_OUTPUT))->getString();
		
		int nbCores = getInput()->getInt(STR_NB_CORES);

		Dispatcher dispatcher(nbCores) ;
		ISynchronizer* synchro = System::thread().newSynchronizer();	//Locking a section

		//Variables required for GATB
		IBank* bank = Bank::open (filename);
		ProgressIterator<Sequence> itSeq (*bank);
		IBank* outBank = new BankFasta (out_file);
	
		//Creating a graph and an iterator. from the parameters. The threshold value is kept as the minimum abundance parameter of the graph. kmer size is the actual kmer+1
		Graph graph = Graph::create (Bank::open(filename), "-kmer-size %d -abundance-min 1", kmer);
		GraphIterator<Node> it = graph.iterator();

		long node_size= it.size();
		unsigned int *counter = new unsigned int[node_size];

		//Initializing the counter for each node in the de Bruijn graph
		for(int i=0;i<node_size;i++)
		{
		    counter[i]=0;
		}

		//Iterating over sequences
		dispatcher.iterate (itSeq, [&] (Sequence& seq)
		{
			int length = seq.getDataSize();
			int flag=1;
			int gb = 1;
			int acceptance=0;

			Kmer<>::ModelCanonical model (kmer);
			Kmer<>::ModelCanonical::Iterator itKmer (model);
			itKmer.setData (seq.getData());
			char* data = seq.getDataBuffer();
			//checking the thresholding
			if(flag==1){
				acceptance=readacceptance(graph, itKmer, model, counter, base);
			}
			if(acceptance > 0)
			{
				for (itKmer.first(); !itKmer.isDone(); itKmer.next())
				{
					std::string s = model.toString (itKmer->value());
					const char* sq = s.c_str();git commit -a 
					Node node = graph.buildNode(sq);
					//Checking whether the node exists.
					if(graph.contains(node))
		       			{
						auto index = graph.nodeMPHFIndex(node);
						__sync_fetch_and_add (&counter[index], 1);
					}
				}
				synchro->lock();
				outBank->insert(seq);
				count++;
				synchro->unlock();
			}

		});
		std::cout << count << std::endl;

		//Free the memory
		delete [] counter;
		bank->flush();
		outBank->flush();
	}
};

int main (int argc, char* argv[])
{
	try{
		ORNA().run(argc,argv);
		return EXIT_SUCCESS;	
	}
	catch(Exception& e){
		std::cout << "EXCEPTION: " << e.getMessage() << std::endl;
        	return EXIT_FAILURE;
	}
     	
}


