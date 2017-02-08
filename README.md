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
20.    Output: Rout
```
