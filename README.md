## About
With the advances in deep sequencing technologies, assembling high coverage datasets has become a challenge in terms of memory and runtime requirements. Hence, read normalization, a lossy read filtering approach is gaining a lot of attendtion. Although current normalization algorithms are efficient, they provide no guarantee to preserve important k-mers that form connections between different genomic regions in the graph. There is a possibility that the resultant assembly is fragmented. In this work, normalization is phrased as a set multicover problem on reads and a linear time approximation algorithm is proposed, named ORNA. ORNA normalizes to the minimum number of reads required to retain all k-mers and relative kmer abundances from the original dataset. Hence, no connections from the original graph are lost and coverage information is preserved. 

##Algorithm
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

##Version
Version 0.1

##Contact
For questions or suggestions regarding ORNA contact

* Dilip A Durai (ddurai_at_contact.mmci.uni-saarland.de)
* Marcel H Schulz (mschulz_at_mmci.uni-saarland.de)

##Download
The software can be downloaded by using the following command
```
	git clone https://github.com/SchulzLab/ORNA
```

## Running ORNA
The downloaded folder should contain the following files and folders:
* install.sh
* gatb-core (it will be empty. Files would be copied in once the install script is run)
* src(folder) (contains the source code for ORNA)

#Installation 
* Run the following command for installation 
```
  bash install.sh
```
* The above command should create a folder ORNA in the present working directory

#Running ORNA
* To run ORNA, execute the following command
```
  ./ORNA/build/tools/ORNA Dataset_name Output LogBase kmerSize NumberOfThreads
```
For instance, if the dataset to be normalized is named as input.fa, the following command would normalize the dataset using a log base of 10 and a kmer size of 21
```
  ./ORNA/build/tolls/ORNA input.fa output.fa 10 21 1
```



  
