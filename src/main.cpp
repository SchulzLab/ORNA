/*
	Optimal Read Normalization Algorithm:
	Developed by : Dilip A Durai and Marcel H Schulz
	ORNA uses GATB library for graph building and k-mer counting. We are thankful for their support
*/


#include <gatb/gatb_core.hpp>
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <thread>
#include <sys/stat.h>

typedef Kmer<>::Type bloom;
static const char* STR_BASE = "-base";
static const char* STR_INPUT = "-input";
static const char* STR_OUTPUT = "-output";
static const char* STR_KMER = "-kmer";
static const char* STR_PAIR1 = "-pair1";
static const char* STR_PAIR2 = "-pair2";
static const char* STR_SORTING = "-sorting";
static const char* STR_KSORTING = "-ksorting";
static const char* STR_BSIZE = "-binsize";
static const char* COLL_STAT = "-cs";
static const char* STR_TYPE = "-type";


unsigned short getlength(IBank* bank)
{
	Iterator<Sequence>* it = bank->iterator();
	unsigned short max=0;
	for (it->first(); !it->isDone(); it->next())
    	{
		Sequence& seq = it->item();
		max = seq.getDataSize();	
		break;
    	}
	return max;		
}

int getnumber(IBank* bank)
{
	Iterator<Sequence>* it = bank->iterator();
	int seq_num=0;
	for (it->first(); !it->isDone(); it->next())
    	{
		seq_num++;
    	}
	return seq_num;
}

int getNodesNumber(GraphIterator<Node> it)
{
	int count=0;
	for (it.first(); !it.isDone(); it.next())
	{
		count++;
	}
	return count;
}

unsigned int getMedian(Graph graph, Sequence& seq, unsigned short kmer, unsigned int *nodeit)
{
	int readabundance = 0;
	int i=0;
	int median;
	short length = seq.getDataSize();	
	unsigned int *abdce = new unsigned int[(length-kmer)+1];
	Kmer<>::ModelCanonical model (kmer);
	Kmer<>::ModelCanonical::Iterator itKmer (model);
	itKmer.setData (seq.getData());
	char* data = seq.getDataBuffer();
	for (itKmer.first(); !itKmer.isDone(); itKmer.next())
	{
		std::string s = model.toString (itKmer->value());
		const char* sq = s.c_str();
		Node node = graph.buildNode(sq);
		auto index = graph.nodeMPHFIndex(node);
		auto abund = nodeit[index];	
		abdce[i] = abund;
		i++;		
	}
	sort(abdce, abdce+i);
	if(i%2==0)
	{
		median = (abdce[(i/2)] + abdce[(i/2)-1])/2; 
	}
	else
	{
		median = abdce[(i/2)];		
	}
	delete [] abdce;
	return median;
}

short calculateQuality(std::string qual)
{
	short score=0;
	for(unsigned int i=0;i<qual.length();i++)
	{
		score=score+(int(qual[i]));
	}
	return score;
}

unsigned long calculateAbundance(Graph graph, Sequence& seq, Kmer<>::ModelCanonical::Iterator itKmer, Kmer<>::ModelCanonical model)
{
	short score=0;
	for (itKmer.first(); !itKmer.isDone(); itKmer.next())
	{
		std::string s = model.toString (itKmer->value());
		const char* sq = s.c_str();
		Node node = graph.buildNode(sq);
		double threshold=0;
		auto index = graph.nodeMPHFIndex(node);
		auto abund = (int)(graph.queryAbundance(node));
	}
	return score;
}

int readacceptance(Graph graph, Kmer<>::ModelCanonical::Iterator itKmer, Kmer<>::ModelCanonical model, unsigned short *counter, double base, unsigned int *nodeit){
	int acceptance=0;
	for (itKmer.first(); !itKmer.isDone(); itKmer.next())
	{
		std::string s = model.toString (itKmer->value());
		const char* sq = s.c_str();
		Node node = graph.buildNode(sq);
		double threshold=0;
		
		auto index = graph.nodeMPHFIndex(node);
		auto abund = nodeit[index];
		
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
			acceptance=acceptance+1;
			break;	
	       	}
	}
	return acceptance;
}

void singleend(Graph graph, const char* filename, const char* out_file, double base, unsigned short kmer, int nbCores, unsigned int *nodeit, const char* type)
{
	int count=0;
	//Multithreading functionality provided by GATB library
	Dispatcher dispatcher(nbCores) ;
	ISynchronizer* sync = System::thread().newSynchronizer();	//Locking a section
	GraphIterator<Node> it = graph.iterator();
	//Variables required for GATB
	IBank* InputDataset = Bank::open (filename);
	ProgressIterator<Sequence> itSeq (*InputDataset);
	IBank* OutDataset;
	std::string tmpfile = out_file;
	std::string out_file1;
	std::string extension;
	const char* finalout;
	
	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		extension=".fq";
		out_file1=tmpfile+extension;
		finalout=out_file1.c_str();
		OutDataset = new BankFasta (finalout, true);	
	}
	else
	{
		extension=".fa";
		out_file1=tmpfile+extension;
		finalout=out_file1.c_str();
		OutDataset = new BankFasta (finalout);
	}
	
	//int node_size= it.size();
	int node_size= it.size();	
	//Creating a graph and an iterator. from the Graph graphparameters. The threshold value is kept as the minimum abundance parameter of the graph. kmer size is the actual kmer+1

	unsigned short *counter = new unsigned short[node_size];

	//Initializing the counter for each node in the de Bruijn graph
	for(int i=0;i<node_size;i++)
	{
	    counter[i]=0;
	}

	//Iterating over sequences the GATB way
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
			//acceptance=readacceptance(graph, itKmer, model, counter, base, nodeit);
			for (itKmer.first(); !itKmer.isDone(); itKmer.next())
			{
				std::string s = model.toString (itKmer->value());
				const char* sq = s.c_str();
				Node node = graph.buildNode(sq);
				double threshold=0;
		
				auto index = graph.nodeMPHFIndex(node);
				auto abund = nodeit[index];
		
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
					acceptance=acceptance+1;
					break;	
			       	}
			}
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
			sync->lock();
			OutDataset->insert(seq);
			count++;
			sync->unlock();
		}
	});
	std::cout << "Kept " << count << " reads" <<  std::endl;

	//Free the memory
	std::string filename1 = string(filename).substr(string(filename).find_last_of("/\\") + 1);
	size_t lindex = filename1.find_last_of(".");
	filename1 = (filename1.substr(0,lindex)) + string(".h5");
	remove(filename1.c_str());
	
	delete [] counter;
	InputDataset->flush();
	OutDataset->flush();	
}

void pairedend(Graph graph, const char* read1, const char* read2, const char* out_file, double base, unsigned short kmer, const char* type, unsigned int *nodeit)
{
	IBank* bank1 = Bank::open (read1);  
	LOCAL (bank1);
	IBank* bank2 = Bank::open (read2);  
	LOCAL (bank2);
	
	int cnt=0;
	int count=0;
	int num_sequence=0;	
	int *pos;	
	int size;
	int index;
	unsigned short length;
	int tmp=0;
	IBank* outBank1;
	IBank* outBank2;
	std::string tmpout=out_file;
	std::string tmp1;
	std::string tmp2;

	const char* out_file1;
	const char* out_file2;
		
	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		tmp1=tmpout+"_1.fq";
		tmp2=tmpout+"_2.fq";
		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();;
		outBank1 = new BankFasta (out_file1, true);
		outBank2 = new BankFasta (out_file2, true);	
	}
	else
	{
		tmp1=tmpout+"_1.fa";
		tmp2=tmpout+"_2.fa";
		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();;
		outBank1 = new BankFasta (out_file1);
		outBank2 = new BankFasta (out_file2);
	}

	GraphIterator<Node> NodeIterator = graph.iterator();	
	long node_size= NodeIterator.size();
	unsigned short *counter = new unsigned short[node_size];

	PairedIterator<Sequence> itPair (bank1->iterator(), bank2->iterator());
        for(itPair.first(); !itPair.isDone(); itPair.next())
        {
        	num_sequence++;
        }
        	
	
	std::vector<int> tempBank;
	
	Kmer<>::ModelCanonical model (kmer);
	Kmer<>::ModelCanonical::Iterator itKmer (model);
	Kmer<>::ModelCanonical::Iterator itKmer1 (model);
            
	//Iteration 1	
	for (itPair.first(); !itPair.isDone(); itPair.next())
        {
            Sequence& s1 = itPair.item().first;
            Sequence& s2 = itPair.item().second;
	    
	    int acceptance1=0;
	    int acceptance2=0;

	    itKmer.setData (s1.getData());
	    itKmer1.setData (s2.getData());
	    	    
	    //checking the thresholding
	    acceptance1 = readacceptance(graph, itKmer, model, counter, base, nodeit);
	    acceptance2 = readacceptance(graph, itKmer1, model, counter, base, nodeit);
	    if(acceptance1>0 && acceptance2>0)
	    {
		for (itKmer.first(); !itKmer.isDone(); itKmer.next())
	    	{
			std::string s = model.toString (itKmer->value());
			const char* sq = s.c_str();
			Node node = graph.buildNode(sq);
			double threshold=0;
			//Checking whether the node exists.
			if(graph.contains(node))
			{
				auto index = graph.nodeMPHFIndex(node);
				auto abund = nodeit[index];
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
					counter[index]=counter[index]+1;
				}
			}
		}
		for (itKmer1.first(); !itKmer1.isDone(); itKmer1.next())
	    	{
			std::string s = model.toString (itKmer1->value());
			const char* sq = s.c_str();
			Node node = graph.buildNode(sq);
			double threshold=0;
			//Checking whether the node exists.
			if(graph.contains(node))
			{
				auto index = graph.nodeMPHFIndex(node);
				auto abund = nodeit[index];
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
					counter[index]=counter[index]+1;
				}
			}
	    	}
	    	outBank1->insert(s1);
		outBank2->insert(s2);
		count++;
	    }
	    else if(acceptance1>0 || acceptance2>0){
	      	tempBank.push_back(tmp);
	    }
	    else{
	    }
	    tmp++;
        }
        
	int coun=0;
	unsigned int tmp_index=0;
	std::cout << "Second Iteration" <<  std::endl;
	
	for (itPair.first(); !itPair.isDone() && tmp_index<tempBank.size(); itPair.next())
	{
        	if(coun==tempBank[tmp_index])
        	{
		    Sequence& s1 = itPair.item().first;
		    Sequence& s2 = itPair.item().second;
		    
		    int acceptance1=0;
		    int acceptance2=0;
		    int acceptance=0;

		    itKmer.setData (s1.getData());
		    itKmer1.setData (s2.getData());
		    	    
		    //checking the thresholding
		    acceptance1 = readacceptance(graph, itKmer, model, counter, base, nodeit);
		    acceptance2 = readacceptance(graph, itKmer1, model, counter, base, nodeit);
		    	    
		    if(acceptance1 > 0 || acceptance2 > 0)
		    {
			for (itKmer1.first(); !itKmer1.isDone(); itKmer1.next())
		    	{
				std::string s = model.toString (itKmer->value());
				const char* sq = s.c_str();
				Node node = graph.buildNode(sq);
				double threshold=0;
				//Checking whether the node exists.
				if(graph.contains(node)){
					auto index = graph.nodeMPHFIndex(node);
					auto abund = nodeit[index];
					threshold = ceil((log(abund)/log(base)));
					if(threshold>abund){
						threshold=abund;
					}
					if(threshold<1){
						threshold=1;
					}
					if(counter[index] < threshold){
						counter[index]=counter[index]+1;
					}
				}
			}
			for (itKmer.first(); !itKmer.isDone(); itKmer.next())
		    	{
				std::string s = model.toString (itKmer->value());
				const char* sq = s.c_str();
				Node node = graph.buildNode(sq);
				double threshold=0;
				//Checking whether the node exists.
				if(graph.contains(node))
				{
					auto index = graph.nodeMPHFIndex(node);
					auto abund = nodeit[index];
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
						counter[index]=counter[index]+1;
					}
				}
		    	}
			outBank1->insert(s1);
			outBank2->insert(s2);
			count++;
		   }
		   tmp_index++;
		}
		coun++;
        }
	std::cout << "kept " << count << " pairs" <<  std::endl;
	bank1->flush();
	bank2->flush();
	delete [] counter;
	outBank1->flush();
	outBank2->flush();
}

void srting(Graph graph, IBank* bank, const char* out_file, double base, unsigned short kmer, int nbCores, unsigned int *nodeit, const char* type)
{
	IBank* outBank;
	std::string s = out_file;
	std::string srted;
	short found;
	std::string outfolder; 
	ProgressIterator<Sequence> itSeq (*bank);
	unsigned int seq_number=getnumber(bank);		
	
	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		srted = "_Sorted.fq";
		s=s+srted;
		remove(s.c_str());
		outBank = new BankFasta (s.c_str(), true);	
	}
	else
	{
		srted = "_Sorted.fa";
		s=s+srted;
		remove(s.c_str());
		outBank = new BankFasta (s.c_str());
	}
	found=s.find_last_of("/\\");
	if(found==-1)
	{
		outfolder="";
	}
	else
	{
  		outfolder =  s.substr(0,found);
		outfolder = outfolder + "/";
	}
	std::string tmpfolder = outfolder+"tmp/";
	mkdir(tmpfolder.c_str(), S_IRWXU);
	
	unsigned short max = getlength(bank);
	map<short, IBank*, greater<short>> bArray;
	map<short, IBank*>::iterator it;
	
	for(itSeq.first();!itSeq.isDone(); itSeq.next())
	{
		Sequence& seq = itSeq.item();
		std::string qual_string = seq.getQuality();
		short qual = calculateQuality(qual_string);	
		std::string tmps = tmpfolder+std::to_string(qual)+".fq";
		it = bArray.find(qual);
		if(it!=bArray.end())
		{
			bArray[qual]->insert(seq);
		}
		else{
			bArray[qual]=new BankFasta(tmps, true);
			bArray[qual]->insert(seq);
		}
	}
	for(it=bArray.begin(); it!=bArray.end(); it++)
	{
		bArray[it->first]->flush();
	}
	for(it=bArray.begin() ; it!=bArray.end() ; it++)
	{
		std::string tmps = tmpfolder+std::to_string(it->first)+".fq";
		BankFasta b (tmps);
    		BankFasta::Iterator its (b);
    		for (its.first(); !its.isDone(); its.next())
    		{
			Sequence& seq = its.item();
			outBank->insert(seq);
    		}
		remove(tmps.c_str());
	}
	rmdir(tmpfolder.c_str());
	outBank->flush();
	bank->flush();
	bArray.clear();
	singleend(graph, s.c_str(), out_file, base, kmer, nbCores, nodeit, type);
}


void pairedEndQualitySorting(Graph graph, const char* read1, const char* read2, const char* out_file, double base, unsigned short kmer, int nbCores, const char* type, unsigned int *nodeit)
{
	IBank* bank1 = Bank::open (read1);  
	LOCAL (bank1);
	IBank* bank2 = Bank::open (read2);  
	LOCAL (bank2);

	std::string outfolder; 
	std::string tmpout=out_file;
	short found;
	found=tmpout.find_last_of("/\\");
	if(found==-1)
	{
		outfolder="";
	}
	else
	{
  		outfolder =  tmpout.substr(0,found);
		outfolder = outfolder + "/";
	}
	std::string tmpfolder = outfolder+"tmp/";
	mkdir(tmpfolder.c_str(), S_IRWXU);

	std::string tmp1;
	std::string tmp2;
			
	IBank* outBank1;
	IBank* outBank2;
	
	const char* out_file1;
	const char* out_file2;
	
	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		tmp1=tmpout+"_Sorted_1.fq";
		tmp2=tmpout+"_Sorted_2.fq";

		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();

		outBank1 = new BankFasta (out_file1, true);
		outBank2 = new BankFasta (out_file2, true);	
	}
	else
	{
		tmp1=tmpout+"_Sorted_1.fa";
		tmp2=tmpout+"_Sorted_2.fa";

		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();

		outBank1 = new BankFasta (out_file1);
		outBank2 = new BankFasta (out_file2);
	}

	const char* tmpf;
	PairedIterator<Sequence> itSeq (bank1->iterator(), bank2->iterator());
	unsigned int seq_number=getnumber(bank1);
	
	unsigned short max = (getlength(bank1))*2;
	map<int, IBank*, greater<int>> bArray;
	map<int, IBank*>::iterator it;		
	
	for(itSeq.first();!itSeq.isDone(); itSeq.next())
	{
		Sequence& seq1 = itSeq.item().first;
		Sequence& seq2 = itSeq.item().second;
		std::string qual_string1 = seq1.getQuality();
		int qual1 = calculateQuality(qual_string1);
		std::string qual_string2 = seq2.getQuality();
		int qual2 = calculateQuality(qual_string2);	
		int qual = (qual1+qual2)/10;
		std::string tmps1 = tmpfolder+"s"+std::to_string(qual)+".fq";
		it = bArray.find(qual);
		if(it!=bArray.end())
		{
			bArray[qual]->insert(seq1);
			bArray[qual]->insert(seq2);
		}
		else{
			bArray[qual]=new BankFasta(tmps1, true);
			bArray[qual]->insert(seq1);
			bArray[qual]->insert(seq2);
		}		
	}
	std::cout << bArray.size() << std::endl;
	for(it=bArray.begin(); it!=bArray.end(); it++)
	{
		bArray[it->first]->flush();
	}
	
	for(it=bArray.begin() ; it!=bArray.end() ; it++)
	{
		std::string tmps = tmpfolder+"s"+std::to_string(it->first)+".fq";
		BankFasta b (tmps);
    		BankFasta::Iterator its (b);
		unsigned int count = 0;
    		for (its.first(); !its.isDone(); its.next())
    		{
			Sequence& seq = its.item();
			if(count%2==0)
			{
				outBank1->insert(seq);
			}
			else{
				outBank2->insert(seq);
			}
			count+=1;
    		}
		remove(tmps.c_str());
	}
	bArray.clear();
	outBank1->flush();
	outBank2->flush();
	bank1->flush();
	bank2->flush();
	pairedend(graph, out_file1, out_file2, out_file, base, kmer, type, nodeit);
}

void ksrting(Graph graph, IBank* bank, const char* out_file, double base, unsigned short kmer, int nbCores, unsigned int *nodeit, short bsize, short cs, const char* type)
{
	std::string s = out_file;
	IBank* outBank;
	std::string srted;
	short found;
	std::string outfolder;
	Dispatcher dispatcher(10) ;

	found=s.find_last_of("/\\");
	if(found==-1)
	{
		outfolder="";
	}
	else
	{
  		outfolder =  s.substr(0,found);
		outfolder = outfolder + "/";
	}
	std::string tmpfolder = outfolder+"tmp/";
	mkdir(tmpfolder.c_str(), S_IRWXU);

	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		srted = "_Sorted.fq";
		s=s+srted;
		outBank = new BankFasta (s.c_str(), true);	
	}
	else
	{
		srted = "_Sorted.fa";
		s=s+srted;
		outBank = new BankFasta (s.c_str());
	}
	unsigned int sqn = getnumber(bank);
	
	//Iterating over nodes of the graphs to get the max abundance
	ProgressIterator<Sequence> itSeq (*bank);
	unsigned int max = 0;
	ISynchronizer* sync = System::thread().newSynchronizer();	//Locking a section
	unsigned int seq_number=getnumber(bank);
	auto idx=0;
	Kmer<>::ModelCanonical model (kmer);
	Kmer<>::ModelCanonical::Iterator itKmer (model);

	map<short, IBank*, greater<short>> bArray;
	map<short, IBank*>::iterator it;
	
	int index =0;	
	unsigned int median ;
	unsigned int count=0;
	unsigned int *med = new unsigned int[seq_number+1];

	std::cout << "Started sorting the dataset" << std::endl;

	dispatcher.iterate (itSeq, [&] (Sequence& seq1)
	{
		med[seq1.getIndex()] = getMedian(graph, seq1, kmer, nodeit);
	});
	count=0;
	for(itSeq.first(); !itSeq.isDone(); itSeq.next())	
	{
		Sequence& seq1 = itSeq.item();
		median = med[count];
		index = median/bsize;
		it=bArray.find(index);
		if(it!=bArray.end())
		{
			bArray[index]->insert(seq1);
		}
		else{
			std::string tmps = tmpfolder+std::to_string(index)+".fa";
			bArray[index]=new BankFasta(tmps, true);
			bArray[index]->insert(seq1);
		}
		count+=1;
	}	

	for(it=bArray.begin(); it!=bArray.end(); it++)
	{
		bArray[it->first]->flush();
	}
	
	std::cout << "Finishing Sorting of dataset" << std::endl;
	for(it=bArray.begin(); it!=bArray.end(); it++)
	{
		std::string tmps = tmpfolder+std::to_string(it->first)+".fa";
		BankFasta b (tmps);
    		BankFasta::Iterator its (b);
    		for (its.first(); !its.isDone(); its.next())
    		{
			Sequence& seq = its.item();
			outBank->insert(seq);
    		}
		remove(tmps.c_str());
	}
	outBank->flush();
	bArray.clear();
	singleend(graph, s.c_str(), out_file, base, kmer, nbCores, nodeit, type);
}

void pairedEndkmerSorting(Graph graph, const char* read1, const char* read2, const char* out_file, double base, unsigned short kmer, int nbCores, const char* type, unsigned int *nodeit, short bsize)
{
	IBank* bank1 = Bank::open (read1);  
	LOCAL (bank1);
	IBank* bank2 = Bank::open (read2);  
	LOCAL (bank2);

	std::string outfolder; 
	std::string tmpout=out_file;
	short found;
	found=tmpout.find_last_of("/\\");
	if(found==-1)
	{
		outfolder="";
	}
	else
	{
  		outfolder =  tmpout.substr(0,found);
		outfolder = outfolder + "/";
	}
	std::string tmpfolder = outfolder+"tmp/";
	mkdir(tmpfolder.c_str(), S_IRWXU);

	std::string tmp1;
	std::string tmp2;
			
	IBank* outBank1;
	IBank* outBank2;
	
	const char* out_file1;
	const char* out_file2;
	
	if((strcmp(type,"fastq")==0) or (strcmp(type,"fq")==0) or (strcmp(type,"FASTQ")==0) or (strcmp(type,"FQ")==0))
	{
		tmp1=tmpout+"_Sorted_1.fq";
		tmp2=tmpout+"_Sorted_2.fq";

		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();

		outBank1 = new BankFasta (out_file1, true);
		outBank2 = new BankFasta (out_file2, true);	
	}
	else
	{
		tmp1=tmpout+"_Sorted_1.fa";
		tmp2=tmpout+"_Sorted_2.fa";

		out_file1 = tmp1.c_str();
		out_file2 = tmp2.c_str();

		outBank1 = new BankFasta (out_file1);
		outBank2 = new BankFasta (out_file2);
	}

	const char* tmpf;
	PairedIterator<Sequence> itSeq (bank1->iterator(), bank2->iterator());
	unsigned int seq_number=getnumber(bank1);
	
	unsigned short max = (getlength(bank1))*2;
	map<int, IBank*, greater<int>> bArray;
	map<int, IBank*>::iterator it;
	
	unsigned int median1;
	unsigned int median2;
	unsigned int median;
	short index;		

	
	for(itSeq.first();!itSeq.isDone(); itSeq.next())
	{
		Sequence& seq1 = itSeq.item().first;
		Sequence& seq2 = itSeq.item().second;
		median1 = getMedian(graph, seq1, kmer, nodeit);
		median2 = getMedian(graph, seq2, kmer, nodeit);
		median = median1 + median2;		
		index = median/bsize;
		it=bArray.find(index);
		if(it!=bArray.end())
		{
			bArray[index]->insert(seq1);
			bArray[index]->insert(seq2);
		}
		else{
			std::string tmps = tmpfolder+std::to_string(index)+".fa";
			bArray[index]=new BankFasta(tmps, true);
			bArray[index]->insert(seq1);
			bArray[index]->insert(seq2);
		}		
		//quality[max*75-qual]+=1;
	}
	for(it=bArray.begin(); it!=bArray.end(); it++)
	{
		bArray[it->first]->flush();
	}
	
	for(it=bArray.begin() ; it!=bArray.end() ; it++)
	{
		std::string tmps = tmpfolder+std::to_string(it->first)+".fa";
		BankFasta b (tmps);
    		BankFasta::Iterator its (b);
		unsigned int count = 0;
    		for (its.first(); !its.isDone(); its.next())
    		{
			Sequence& seq = its.item();
			if(count%2==0)
			{
				outBank1->insert(seq);
			}
			else{
				outBank2->insert(seq);
			}
			count+=1;
    		}
		remove(tmps.c_str());
	}
	bArray.clear();
	outBank1->flush();
	outBank2->flush();
	bank1->flush();
	bank2->flush();
	pairedend(graph, out_file1, out_file2, out_file, base, kmer, type, nodeit);
}

class ORNA : public Tool
{
public:
	ORNA(): Tool("ORNA")
	{
		getParser()->push_front (new OptionOneParam (STR_KMER, "kmer required",  false, "21"));				
	        getParser()->push_front (new OptionOneParam (STR_INPUT, "Input File",  false, "ORNAERROR"));
	        getParser()->push_front (new OptionOneParam (STR_OUTPUT, "Prefix of the output File",  false, "Normalized"));
		getParser()->push_front (new OptionOneParam (STR_BASE, "Base for the logarithmic function",  false, "1.7"));
		getParser()->push_front (new OptionOneParam (STR_PAIR1, "First read of the pair", false, "ORNAERROR" ));
		getParser()->push_front (new OptionOneParam (STR_PAIR2, "Second read of the pair", false, "ORNAERROR" ));	
		getParser()->push_front (new OptionOneParam (STR_SORTING, "Quality Sorting", false, "0" ));
		getParser()->push_front (new OptionOneParam (STR_KSORTING, "Kmer based Sorting", false, "0" ));
		getParser()->push_front (new OptionOneParam (STR_BSIZE, "Bin Size", false, "1000" ));
		getParser()->push_front (new OptionOneParam (STR_TYPE, "Type for the output file (fasta/fastq)", false, "fasta" ));
		getParser()->push_front (new OptionOneParam (COLL_STAT, "Collect stat", false, "0" ));
	}
	void execute()
	{
		const char* filename = (getInput()->get(STR_INPUT))->getString();
		const char* read1= (getInput()->get(STR_PAIR1))->getString(); 
		const char* read2= (getInput()->get(STR_PAIR2))->getString();
		double base=getInput()->getDouble(STR_BASE);
		int user_kmer = getInput()->getInt(STR_KMER);
		short sorting = getInput()->getInt(STR_SORTING);
		short ksorting = getInput()->getInt(STR_KSORTING);		
		const char* out_file= (getInput()->get(STR_OUTPUT))->getString();
		int nbCores = getInput()->getInt(STR_NB_CORES);
		short bsize = getInput()->getInt(STR_BSIZE);
		short cs = getInput()->getInt(COLL_STAT);		
		unsigned short pairflag = 0; 
		unsigned short kmer = user_kmer + 1; 
		unsigned short cores = sysconf(_SC_NPROCESSORS_ONLN);
		const char* type = (getInput()->get(STR_TYPE))->getString();
		IBank* bank;
		IBank* bank1;
		IBank* bank2;	

		if(nbCores==cores){
			nbCores=1;		
		}

		std::cout<<"Given Parameters"<<std::endl;
		std::cout<<"----------------"<<std::endl;
		std::cout<<"Base:	"<<base<<std::endl;
		std::cout<<"kmer size:	"<<user_kmer<<std::endl;
		if(sorting==1)
		{
			std::cout << "Mode: ORNA-Q" << std::endl;
		}
		else if(ksorting==1){
			std::cout << "Mode: ORNA-K" << std::endl;
			std::cout<<"Bin size:	"<<bsize<<std::endl;		
		}
		else{
			std::cout << "Mode: ORNA" << std::endl;
		}
		std::cout<<"Number of cores:	"<<nbCores<<std::endl;
		std::cout<<"----------------"<<std::endl;
  
		if(std::strcmp(filename, "ORNAERROR") == 0)
		{
			if((std::strcmp(read1, "ORNAERROR") == 0) && (std::strcmp(read2, "ORNAERROR") == 0))
			{
				std::cout << "Input File(s) missing. Please refer to the help" << std::endl;
			}
			if (((std::strcmp(read1, "ORNAERROR") != 0) && (std::strcmp(read2, "ORNAERROR") == 0)) || ((std::strcmp(read1, "ORNAERROR") == 0) && (std::strcmp(read2, "ORNAERROR") != 0)))
			{
				std::cout << "One of the pair files is missing. Please refer to the help" << std::endl;	
			}
			if((std::strcmp(read1, "ORNAERROR") != 0) && (std::strcmp(read2, "ORNAERROR") != 0))
			{
				pairflag = 1; 
			}				
		}
		if(sorting == 1 && ksorting == 1)
		{
			std::cout << "Error: ORNA-Q and ORNA-K cant be set to 1 at the same time" << std::endl;
		}
		else
		{	
			if(pairflag==0)
			{
				bank = Bank::open (filename);
				Graph graph = Graph::create (Bank::open(filename), "-kmer-size %d -abundance-min 1", kmer);
				GraphIterator<Node> it = graph.iterator();
				unsigned int *nodeit = new unsigned int[it.size()+1];	

				std::cout << "Populating node abundances" << std::endl;
				int cont=0;
				for (it.first(); !it.isDone(); it.next())
				{
					nodeit[cont]=0;
					cont=cont+1;
				}	

				for (it.first(); !it.isDone(); it.next())
				{
					auto idx = graph.nodeMPHFIndex(it.item());				
					nodeit[idx]=(it.item()).abundance;
				}
				std::cout << "Done populating node abundances" << std::endl;
				if(sorting==1)
				{
					std::cout << "Running ORNA-Q and ORNA in single end mode" << std::endl;
					srting(graph, bank, out_file, base, kmer, nbCores, nodeit, type);			
				}
				else if (ksorting==1)
				{
					std::cout << "Running ORNA-K in single end mode" << std::endl;
					ksrting(graph, bank, out_file, base, kmer, nbCores, nodeit, bsize, cs, type);
				}
				else
				{
					std::cout << "Running ORNA in single end mode" << std::endl;
					singleend(graph, filename, out_file, base, kmer, nbCores, nodeit, type);
				}
			}
			else
			{
				Graph graph = Graph::create ("-in %s,%s -kmer-size %d -abundance-min 1", read1, read2, kmer);
				GraphIterator<Node> it = graph.iterator();
				unsigned int *nodeit = new unsigned int[it.size()+1];	

				std::cout << "Populating node abundances" << std::endl;
				int cont=0;
				for (it.first(); !it.isDone(); it.next())
				{
					nodeit[cont]=0;
					cont=cont+1;
				}	

				for (it.first(); !it.isDone(); it.next())
				{
					auto idx = graph.nodeMPHFIndex(it.item());				
					nodeit[idx]=(it.item()).abundance;
				}				
				if(sorting==1)
				{
					std::cout << "Running ORNA-Q in paired end mode" << std::endl;	
					pairedEndQualitySorting(graph,read1, read2, out_file, base, kmer, nbCores, type, nodeit);
				}
				else if (ksorting==1)
				{
					std::cout << "Running ORNA-K in paired end mode" << std::endl;
					pairedEndkmerSorting(graph, read1, read2, out_file, base, kmer, nbCores, type, nodeit, bsize);
				}
				else{
					std::cout << "Running ORNA in paired end mode" << std::endl;
					pairedend(graph, read1, read2, out_file, base, kmer, type, nodeit);
				}
			}
		}
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
