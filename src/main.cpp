/*
	Optimal Read Normalization Algorithm:
	Developed by : Dilip A Durai and Marcel H Schulz
*/

//File last modified 28/11/2016

#include <gatb/gatb_core.hpp>
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>

typedef Kmer<>::Type bloom;

void helpmessage(){
	//std::cout << "One or more parameter missing" << std::endl;
	std::cout << "Usage : ./ORNA input.fa output.fa log_base kmer_size no_of_threads" << std::endl;
}

void errormessage(){
	std::cout << "Error: One or more parameter missing" << std::endl;
	std::cout << "Usage : ./ORNA input.fa output.fa log_base kmer_size no_of_threads" << std::endl;
}

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

int main (int argc, char* argv[])
{
    try
    {
	if(strcmp(argv[1],"--help") == 0 or strcmp(argv[1],"-h") == 0)
	{
		helpmessage();
	}
	else if(strcmp(argv[1],"--version")==0 or strcmp(argv[1],"-v") == 0)
	{
		std::cout << "v.0.1" << std::endl;
	}
	else if (argc <6)
	{
		errormessage();
	}
	else{
        //Variables required by ORNA
		int count=0;
		double base=atof(argv[3]);
		int user_kmer = atoi(argv[4]);
		int kmer = user_kmer + 1; 
		const char* filename = argv[1];
		const char* out_file= argv[2];

		auto nbCores = atoi(argv[5]);
		Dispatcher dispatcher (nbCores);
		ISynchronizer* synchro = System::thread().newSynchronizer();	//Locking a section

		//Variables required for GATB
		IBank* bank = Bank::open (filename);
		ProgressIterator<Sequence> itSeq (*bank);
		IBank* outBank = new BankFasta (out_file);
	
		//Creating a graph and an iterator. from the parameters. The threshold value is kept as the minimum abundance parameter of the graph. kmer size is the actual kmer+1
		Graph graph = Graph::create (Bank::open(argv[1]), "-kmer-size %d -abundance-min 1", kmer);
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
					const char* sq = s.c_str();
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
    }
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }
    return EXIT_SUCCESS;
}


