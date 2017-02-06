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
int errorcorrection(Graph graph, Kmer<>::ModelCanonical::Iterator itKmer, Kmer<>::ModelCanonical model, int length, int kmer){

	int stretch=0;
	int startkmer = 0;
	int endkmer = 0;
	int flag=1;
	int position = 0;
	int numerror=0;
	for(itKmer.first(); !itKmer.isDone(); itKmer.next())
	{
		std::string ts = model.toString (itKmer->value());
		const char* tsq = ts.c_str();
		Node tnode = graph.buildNode(tsq);
		if(!(graph.contains(tnode)) && (startkmer==1))
		{
			stretch+=1;
			position+=1;
		}
		else
		{
			if(position==0)
			{
				startkmer=1;
			}
			if(position == (length-kmer))
			{
				endkmer=1;
			}
			if(stretch==kmer)
			{
				if((startkmer==1)){
					numerror=1;
				}
			}
			stretch=0;
			position+=1;
		}
	}
	if(numerror==1)
	{
		if(endkmer==1)
		{
			flag=0;
		}
	}
	return flag;
}

int readacceptance(Graph graph, Kmer<>::ModelCanonical::Iterator itKmer, Kmer<>::ModelCanonical model, long *counter, double t){
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
		threshold = ceil((log(abund)/log(t)));
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
        //Variables required by ORNA
        int count=0;
        double t=atof(argv[3]);
	std::cout << t << std::endl;
        int kmer = atoi(argv[4]);
        const char* filename = argv[1];
	const char* out_file= argv[2];

        //Multithreading variables test (last modified 23/11/2016)
        auto nbCores = atoi(argv[5]);
        Dispatcher dispatcher (nbCores);
	ISynchronizer* synchro = System::thread().newSynchronizer();	//Locking a section

        //Variables required for GATB
        IBank* bank = Bank::open (filename);
        ProgressIterator<Sequence> itSeq (*bank);
        IBank* outBank = new BankFasta (out_file);

        //Creating a graph and an iterator. from the parameters. The threshold value is kept as the minimum abundance parameter of the graph. kmer size is the actual kmer+1
        Graph graph = Graph::create (Bank::open(argv[1]), "-kmer-size %s -abundance-min 1", argv[4]);
        Graph::Iterator<Node> it = graph.iterator();

        long node_size= it.size();
        long *counter = new long[node_size];
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
		//Error Correction
		if(gb==1){
			//flag=errorcorrection(graph, itKmer, model, length, kmer);
		}
		//checking the thresholding
		if(flag==1){
			acceptance=readacceptance(graph, itKmer, model, counter, t);
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
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }
    return EXIT_SUCCESS;
}


