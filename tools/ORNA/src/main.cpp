/*
	Optimal Read Normalization Algorithm: 
	Developed by : Dilip A Durai and Marcel H Schulz
*/

//File last modified 28/11/2016

#include <gatb/gatb_core.hpp>
#include <iostream>
#include <string.h>

typedef Kmer<>::Type bloom;

int main (int argc, char* argv[])
{
    try
    {
        //Variables required by ORNA
        int count=0;
        int t=atoi(argv[3]);
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
        Graph graph = Graph::create (Bank::open(argv[1]), "-kmer-size %s -abundance-min 3", argv[4]);
        Graph::Iterator<Node> it = graph.iterator();

        int node_size= it.size();
        int *counter = new int[node_size];
        int *flg= new int[node_size];

        //Initializing the counter for each node in the de Bruijn graph
        for(int i=0;i<node_size;i++)
        {
            counter[i]=0;
            flg[i]=0;
        }
	
	//Iterating over sequences 
        dispatcher.iterate (itSeq, [&] (Sequence& seq)
	{
		auto acceptance=0;
		int stretch=0;
		int position = 0;
		int numerror=0;
		int length = seq.getDataSize();
		int startkmer = 0;
		int endkmer = 0;		
		int flag=1;	
		int ch[5] = {0,0,0,0,0};
		
		Kmer<>::ModelCanonical model (kmer);
        	Kmer<>::ModelCanonical::Iterator itKmer (model);
		itKmer.setData (seq.getData());
		char* data = seq.getDataBuffer();
		//Garbage Removal
		/*for (size_t i=0; i<seq.getDataSize (); i++)
		{
			if (data[i]=='N' ) { 
				ch[0]+=1; 
			}
			else if (data[i]=='A' ) { 
				ch[1]+=1; 
			}
			else if (data[i]=='C' ) { 
				ch[2]+=1; 
			}
			else if (data[i]=='T' ) { 
				ch[3]+=1; 
			}
			else{ 
				ch[4]+=1; 
			}
		}
		for(int i=1;i<=4;i++)
		{
			if(ch[i]>=0.70*length)
			{
				flag=0;
			}
		}		
		if(ch[0]>=10)
		{
			flag=0;
		}*/
		//Error Correction	
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

		//checking the thresholding
		if(flag==1){
			//Iterating over kmers in each sequence
			for (itKmer.first(); !itKmer.isDone(); itKmer.next())
			{
				std::string s = model.toString (itKmer->value());
				const char* sq = s.c_str();
				Node node = graph.buildNode(sq);
				//Checking whether the node exists.
				if(!(graph.contains(node)))
			       	{
					__sync_fetch_and_add (&acceptance, 1);
				}
				else
				{
					auto index = graph.nodeMPHFIndex(node);
					if(counter[index] < t)
				        {
						__sync_fetch_and_add (&acceptance, 1);
						__sync_fetch_and_add (&counter[index], 1);
			       		}
				}
			}
		}
		if(acceptance > 0)
		{
			synchro->lock();
			outBank->insert(seq);
			count++; 
			synchro->unlock();		
		}
			
	});
	std::cout << count << std::endl;
        
	//Free the memory
        delete [] counter;
        delete [] flg;
        bank->flush();
        outBank->flush();
    }	
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }
    return EXIT_SUCCESS;
}
