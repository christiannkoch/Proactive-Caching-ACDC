# Adaptive Category-aware Designed Caching (ACDC)

Proactive caching is a recent research discipline. In this project, we provide the source code to setup and run **ACDC** as proposed in our publication: 

<cite>
C. Koch, J. Pfannmüller, A. Rizk, D. Hausheer, R. Steinmetz, "Category-aware Hierarchical Caching for Video-on-Demand Content on YouTube", ACM Multimedia Systems Conference (MMSys), 2018.
</cite>



# Dependencies
## Packages and Libraries 
We built the software from scratch by installing the following packages and libraries on an Ubuntu 16.04 LTS with 2GB RAM and 15GB storage.

```{r, engine='bash'}
sudo apt install mysql-server libmysqlcppconn-dev libboost-dev python3 python3-pip git python3-tk
sudo pip3 install mysql-connector pandas seaborn
```



## OMNeT++
Here, we provide a brief HowTo on installing OMNeT++. You can download OMNeT++ here: https://www.omnetpp.org/. If you tap into any issues, please refer to the the OMNeT++ installation guide located in the folder /doc/InstallGuide.pdf. 
In case you have installed python **anaconda3**, you have to uncomment the path for conda in ~/.bashrc file during the installation - this is a known issue of a OMNeT++ dependency (QT). To build and install OMNeT++, execute:

```{r, engine='bash'}
sudo apt-get install build-essential gcc g++ bison flex perl qt5-default tcl-dev tk-dev libxml2-dev zlib1g-dev default-jre doxygen graphviz libwebkitgtk-3.0-0 libopenscenegraph-dev openscenegraph-plugin-osgearth libosgearth-dev openmpi-bin libopenmpi-dev
gedit ~/.bashrc
```
In the editor, add the line "export PATH=\$HOME/omnetpp-5.2.1/bin:\$PATH" which points to the directory where you have extracted and stored OMNeT++. Next, you have to close the terminal and open it again in order to apply the changes made in the .bashrc file. 
Now, open a terminal, change directory to the OMNeT++ folder and execute the following commands. You may append a -j16 to the make command in order to decrease compilation time. Keep in mind that this might consume more than 2GB RAM.

```{r, engine='bash'}
. setenv
./configure
make
make install
```    


# Database Setup
We are not allowed to make our dataset public since it contains confidential information. To this end, we provide scripts to generate exemplary data that can be used to experiment with the system. However, any other dataset in the right format can be used for evaluation. To setup the database scheme and fill it with randomly generated data, execute the following two python scripts. In the *fillDatabase.py*, you can vary the number of unique videos, unique users, overall requests, and the time interval the dataset captures. By default, it fills your database with about 700 requests consisting of 200 unique video IDs performed by 300 users in a time interval of one week. In addition, you can filter your requests by the *Category* parameter. If it is set to *ALL*, all requests are used in the simulation, otherwise the requests matching the given string are filtered for simulation. In case you use our simulation with the graphical OMNeT++ interface you should configure less than 100 distinct users or otherwise it may run slow. For usage without the graphical interface, you should create less than 800.000 distinct users, due to a limitation in OMNeT++.
```{r, engine='bash'}
cd /PythonWorkspace/Environment
python3 setupSchema
python3 fillDatabase
```    






# Installation
Navigate to the project folder and run:

```{r, engine='bash'}
$ cd Proactive-Caching-ACDC
$ opp_makemake -f --deep -lmysqlcppconn
```    
Next, open the *Makefile* and manually insert the include paths mentioned below. This needs to be done because of a known issue of OMNeT++ (https://groups.google.com/forum/\#!msg/omnetpp/VEE4oXtuPes/cSQG7vZ-AAAJ). 

```{r, engine='bash'}
#C++ include paths (with -I)
INCLUDE_PATH =\
	-I. \
    -Iconfigurations \
    -Iconfigurations/ACARCConfigs \
    -Iconfigurations/categorial \
    -Iconfigurations/Hybrid \
    -Iconfigurations/Infinity \
    -Iconfigurations/Special \
    -Iconfigurations/Topologies \
    -Iresults \
    -Isrc \
    -Isrc/cache \
    -Isrc/cache/admissionStretegies \
    -Isrc/cache/admissionStretegies/lcd \
    -Isrc/cache/admissionStretegies/lce \
    -Isrc/cache/admissionStretegies/mcd \
    -Isrc/cache/admissionStretegies/nhit \
    -Isrc/cache/admissionStretegies/prob \
    -Isrc/cache/admissionStretegies/rand \
    -Isrc/cache/evictionStrategies \
    -Isrc/cache/evictionStrategies/acdc \
    -Isrc/cache/evictionStrategies/arc1 \
    -Isrc/cache/evictionStrategies/arc2 \
    -Isrc/cache/evictionStrategies/fifo \
    -Isrc/cache/evictionStrategies/ghostLists \
    -Isrc/cache/evictionStrategies/infinity \
    -Isrc/cache/evictionStrategies/lfu \
    -Isrc/cache/evictionStrategies/lfuda \
    -Isrc/cache/evictionStrategies/lru \
    -Isrc/cache/evictionStrategies/nodes \
    -Isrc/cache/evictionStrategies/probationary \
    -Isrc/cache/evictionStrategies/rand \
    -Isrc/cache/evictionStrategies/segment \
    -Isrc/cache/evictionStrategies/slru \
    -Isrc/cache/evictionStrategies/ttl \
    -Isrc/database \
    -Isrc/simulation
```  
Finally, build the software using the make command, e.g., with 8 parallel threads by executing:
```{r, engine='bash'}
$ make -j 8
```   


# Execution
For the execution, you need to specify a certain configuration by its name, e.g., *Run6*. The existing 1,008 configurations available are listed in the *omnetpp.ini* file and located in the configurations folder. To run the simulation with graphical interface, execute the first command. For larger simulations, we recommend using no GUI by executing the second command.

```{r, engine='bash'}
./Testbed -c Run6 omnetpp.ini -r 0 (with GUI)
./Testbed -c Run6 omnetpp.ini -r 0  -u Cmdenv ```   
```   
Here, *-r* explicitly denotes the number of repetitions you want to run for a certain configuration. Due to random assignment of clients to the caches, the results may vary in each simulation run. The description belonging to the run number is given in the *omnetpp.ini* file. For Run6, the corresponding entry looks like this: 

```{r, engine='bash'}
[Config Run6]
Internet.SetupfileName = "Topology 1 7 LCD ARC2 8192"
repeat = 30``   
``` 
Here, the *Internet.SetupfileName*  contains information about the topology (1 - as explained in Sec. 2.3 in the paper), followed by the number of caches (7), the admission strategy (LCD), the admission parameter (not used here), the eviction strategy (ARC2), and the overall cache storage size in MBit (8,191 Mbit = 1GB)

Note that Internet.Setupfilename refers to a file contained in Testbed/configurations/. It is easy to create a new configuration as it contains human readable parameters and an adjacency matrix to easily design new cache topologies. A *+* entry in the matrix indicates that two caches are connected. The higher number always refers to the parent cache. For topology 1, the corresponding matrix looks like this:

```{r, engine='bash'}
C 0 1 2 3 4 5 6 7
0 x + - - - - - -
1 + x + + - - - -
2 - + x - + + - -
3 - + - x - - + +
4 - - + - x - - -
5 - - + - - x - -
6 - - - + - - x -
7 - - - + - - - x  
``` 



# Evaluation Plots
The results of the simulation are stored in the project folder under *Testbed/results/*. After the simulation is complete you have to sort the results by name and copy them to their according topology folder. For example, the results belonging to topology 1 are represented by the run numbers 0-293 as depicted by Table below.

| Topology  | 1               | 2                | 3                 | 4                 |
|-----------|-----------------|------------------|-------------------|-------------------|
| Run Range | 0-174,  600-634 | 300-424, 660-684 | 425-599,  685-719 | 300-424,  635-659 |

To process and plot the results, you need to copy them to *PythonWorkspace/Evaluation/SimulationResults/Top1*.
If you want to collect the results of another topology, name the folder accordingly, i.e., Top{1,2,3,4}. You can plot all figures by running:

```{r, engine='bash'}
python3 GenerateDataAndFigures.py
``` 
The results can be found in the figures folder and are sorted according to their topology.



# Creating Customized  Simulation Configurations
Our program builds a Network from a given configuration file. The first few lines in this file are used to specify the database connection used. After that, we design the caches, i.e., reverse proxies. At the end, we specify an adjacency matrix that represents the connection between the caches.

## Database

The configuration file starts with database-specific information, which are mostly self-explaining. The database type can in principle also determine other database systems such as MongoDB or CouchDB. Currently, only a mysql database handler is implemented. 
```{r, engine='bash'}
Database Type = mysql
Database Name = student
Database Table = YouTubeDataset
Database Login = admin
Database Password = start123
Database Address = 127.0.0.1
Database Port = 3306
``` 
## Request Specification
Start and end Time are used to specify the time interval by two unix timestamps from which we take the requests of our request dataset. Thereby, we can simulate parts of the dataset and, thereby, decrease the simulation time.
```{r, engine='bash'}
Start Time = 1397426400.07611
End Time = 1398031200.07611
``` 

The category refers in our use case to a YouTube category. We can specify a single category to get a subset of requests that belong to this category for the simulation. If you choose *ALL*, no filter is applied. 
```{r, engine='bash'}
Category = ALL
``` 


## Reverse Proxies

A Reverse Proxy has several settings and can be designed using the following parameter and adjacency matrix.

-   Admission Strategy – The admission strategy
-   Parameter List – Parameters for the admission strategy, e.g., 2 for NHIT2
-   Eviction Strategy – The eviction strategy
-   Parameter List – Parameters for the eviction strategy
-   Proxy Name – The name of the proxy for evaluation
-   Storage Alterations – A list of dates and sizes that indicate a growth or shrinkage of the storage
-   StorageAlterationStrategy - The strategy with which the storage increases or decreases
-   Leaf – A Boolean value that tells if the proxy is a leaf proxy

### Admission Strategy

We provide you with a number of implemented admission strategies. You can google them if you want to know how they work in detail (or you look in the code). Admission Strategies decide, whether a passing video segment will be saved in the cache. The following are implemented:
-   LCE – Leave Copy Everywhere
-   LCD – Leave Copy Down (only feasible in a topology with multiple levels)
-   MCD – Move Copy Down (only feasible in a topology with multiple levels)
-   NHIT – Cache on N+1th hit
	- An Integer representing +1 that describes when the segment is stored and a double value that describes the time in seconds in which it has to be seen in order to count as a second hit. Example: Parameter List = 1,259200.00;
-   PROB – Cache with a Probability
	- Double Value representing a probability with which a segment will be cached. Example Parameter List = 0.75;

### Eviction Strategy

We provide you with a number of implemented admission strategies. You can google them if you want to know how they work in detail (or you look in the code). Eviction strategies decide which video segment has to be deleted, when a new one is inserted and something has to be deleted to get enough free space to allow inserting the new segment:

-   ARC2 – Adaptive Replacement Caching
-   LFU – Least Frequently Used
-   LFUDA – Least Frequently Used with Dynamic Aging
-   LRU – Least Recently Used
-   SLRU – Segmented Least Recently Used
-   TTL – Time To Live
	- A double value that describes the time in seconds after which a segment will be deleted. Example Parameter List = 259200.00;
-   INFINITY – A Cache with infinite Size – nothing will be deleted
-   RAND – Random Eviction
-   ACDC– Adaptive Content-Aware Designed Cache
	- Described Below

### ACDC Configuration

ACDC is triggered by naming the configuration ACDC). ACDC has many different sub settings. We can configure the ACDC cache by specifying the parameter list with the:

-   Segment Size Adaption Strategy (SSAS)
	- relaitveLargestGhostList
	- relativeSmallesGhostList
	- largestGhostList
	- smallestGhostList
	- probationaryFirst
	- leftFirst
	- rightFirst
-   The segment eviction strategy for the MISC category. The MISC category is the category that includes all not specified categories.
-   The MISC category
-   A List of tuples of segment eviction strategies and categories. A cache segment will be created for each tuple. Example: LRU,Music,LRU,Entertainment
-   A Double value that represents the minimum segment size.

An Example Configuration would look like this:

```{r, engine='bash'}
parameterList = relativeLargestGhostList,FIFO,MISC,LRU,Music,LRU,Entertainment,LRU,People,LRU,Comedy,2.5;
``` 

### Proxy Name

The Proxy name will be used later for identifying the recorded values like cache hit and delay. Use a distinct name for each proxy.

Example: Proxy Name = Proxy1;

### Storage Alterations

We wanted to simulate the increase of storage at given time points. You can provide a list of tuples of a time in seconds and size in Integer form determining at which point in time the storage of the reverse proxy will be expanded. You can leave this empty if you do not want any alterations.  An example for doubling the storage size each day with a base storage size of 1,677,721 Mbit would look like this:

```{r, engine='bash'}
StorageAlterations = 86400:3355442,172800:6710884,259200:13421768,345600:26843536,432000:53687072,518400:107374144;
``` 


### StorageAlterationStrategy

If you chose a cache with an eviction strategy that segments the cache, e.g., ARC or ACDC, you can decide here how the segments should adapt to the new size. You have two options:

1.  **Equal**: The size of each cache segment is set to the new subSize of the segments which is calculated by dividing the new size with the number of cache segments. Hence, each cache segment has an equal storage share.
2.  **Partial**: The size of each cache segment is set to the new subSize of the segments which is calculated as the percentage of the size of the individual segment at the time of alteration and, thereby, assigning it a new size based on its previous size.

For Example: 2 Segments – segment1 has a size of 5GB and segment2 has a size of 1GB. We have a shared storage of 6GB. The new storage size shall be 12gb.
With „**equal**“ the size of segment1 and Segment2 will be 6GB each.
With „**partial**“ the size of segment1 will be 10GB and the size of Segment2 will be 2GB.
If *StorageAlterations* has no values, the *StorageAlterationStrategy* will be ignored.

### Leaf

This Boolean value tells the program if clients will be attached to the proxy. This will play a role in the cache adjacency matrix. 

## Adjacency Matrix

Next we specify the time it takes for a segment to pass through all levels of the topology. The time is denoted in milliseconds. This value is used to assign a delay to the connections of a cache hierarchy. The value calculates with the amount of levels which are specified in the line down. Example:

```{r, engine='bash'}
cdnDelay = 48
Levels = 2
``` 


Next is the cache adjacency matrix. An **x** is the connection to itself, a **+** stands for a connection and a **–** for no connection. A sample topology is shown here and the adjacent matrix is right next to it.
```{r, engine='bash'}
c 0 1 2 3 4 5
0 x + - - - -
1 + x + + + +
2 - + x - - -
3 - + - x - -
4 - + - - x -
5 - + - - - x
``` 
We provide you with several sample configurations in the folder *Testbed/configurations/* so that you can easily create your own configuration.
