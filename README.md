## About
With the advances in deep sequencing technologies, assembling high coverage datasets has become a challenge in terms of memory and runtime requirements. Hence, read normalization, a lossy read filtering approach is gaining a lot of attention. Although current normalization algorithms are efficient, they provide no guarantee to preserve important k-mers that form connections between different genomic regions in the graph. There is a possibility that the resultant assembly is fragmented. In this work, normalization is phrased as a set multicover problem on reads and a linear time approximation algorithm is proposed, named ORNA (Optimized Read Normalization Algorithm). ORNA normalizes to the minimum number of reads required to retain all k-mers and relative kmer abundances from the original dataset. Hence, no connections from the original graph are lost and coverage information is preserved. 

## When to use ORNA
ORNA is a read normalization software developed in spirit of [Diginorm](https://github.com/dib-lab/2012-paper-diginorm). ORNA is computationally inexpensive and it guarantees the preservation of all kmers from the original dataset. It can be used if the user has a high coverage dataset but does not have enough computational power (in particular memory but also limited time) in order to conduct a *de novo* assembly, because it removes the redundancy in your data. It can also be used to merge many sequencing datasets. The user must be aware that using ORNA (or in that case any normalization software) might have a significant impact on the assemblies produced as it is highly dependent on the dataset.

## Algorithm
```
1.  Input : Read set R, LogBase b, kmer size k
2.  Initialization: k'=k+1
3.                  n = NumberOfDistinctK'mers(R)
4.                  counter(0,...,n)=0
5.                  Rout=null
6.  Steps:
7.          for r in R:
8.              flag=0
9.              V'=ObtainK'mers(R)
10.             for v in V':
11.                if(counter(v) < min(abundance(v), log_b(abundance(v)))) then:
12.                  counter(v)++
13.                  flag=1
14.                end if
15.              end for
16.              if flag!=0 then:
17.                Rout = Rout U r
18.              end if
19.          end for
20. Output: Rout
```

*	ORNA uses the GATB version 1.2.2 to build a cascading bloom filter to store the kmer information
*	It reduces the abundance of a kmer to a value which is equal to the logarithmic transformation of the abundance. The base b of the logarithm is provided by the user.
*	ORNA was tested on two de bruijn graph based assemblers namely Oases and TransABySS		

## Points to be noted
*	Currently, as ORNA retains all the kmers from the original dataset, it would also retain erroneous kmers. ORNA assumes that the input dataset is error corrected prior to normalization. In case of RNA-seq or other non-uniform data we suggest to use the [SEECER](http://sb.cs.cmu.edu/seecer/) algorithm that proved to work well with ORNA.
*	ORNA does not currently support paired end reads. It treats paired end reads as single end reads and hence the paired end information is not retained in the normalized dataset. Future versions of ORNA will support paired end information. 

## Version
Version 0.1

## Contact
For questions or suggestions regarding ORNA contact

* Dilip A Durai (ddurai_at_contact.mmci.uni-saarland.de)
* Marcel H Schulz (mschulz_at_mmci.uni-saarland.de)

## Download
The software can be downloaded by using the following command
```
	git clone https://github.com/SchulzLab/ORNA
```

The downloaded folder should contain the following files and folders:
* install.sh
* gatb-core (it will be empty. Files would be copied in once the install script is run)
* src(folder) (contains the source code for ORNA)

## Pre-requisite
gcc version >=4.7

## Installation 
* Run the following command for installation 
```
  bash install.sh
```
* The above command should create a build folder. The executable of ORNA will be in build/bin 

## ORNA parameters

`./bin/ORNA -help`

short | explanation | note
-----------|------------|--------
-help | shows the help message |
-base | Base value for the logarithmic function| Required parameter
-kmer | the value of k for kmer size | Default 21
-input | Input fasta file | Required parameter
-output | Output fasta file| Required parameter 
-nb-cores | number of cores | default=0 (all cores)

kmer value: <br />
This parameter represents the kmer size to be used for reduction. All the analysis in the paper were done using a kmer size of 21 for reads having length of 50bps and 76bps. In general, a lower kmer size will have a higher percent of reads reduced but will require more memory and runtime.

base: <br />
This parameter represents the base of the logarithm function used to decide the new abundance of kmer. For instance if the original abundance of a kmer is 1000 and a base of 10 is selected as a parameter then the new abundance is set to log<sub>10</sub>1000 = 3. According to the analysis done in ORNA paper, a base of 1.7 seems to be a good compromise between data reduction and little loss in assembly quality.

## Running ORNA
* To run ORNA, execute the following command from the installation directory
```
  ./build/bin/ORNA -input Dataset_name -output Output -base LogBase -kmer kmerSize -nb-cores NumberOfThreads
```
For instance, if the dataset to be normalized is named as input.fa, the following command would normalize the dataset using a log base of 10 and a kmer size of 21
```
  ./build/bin/ORNA -input input.fa -output output.fa -base 10 -kmer 21 -nb-cores 1
```

Note that ORNA only accepts input file in fasta format

## Citation
If you use ORNA in your work please cite:

Durai DA, Schulz MH. In-silico read normalization with set multicover optimization. in preparation
