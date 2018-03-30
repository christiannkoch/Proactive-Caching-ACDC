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
We are not allowed to make our dataset public since it contains confidential information. To this end, we provide scripts to generate exemplary data that can be used to experiment with the system. However, any other dataset in the right format can be used for evaluation. To setup the database scheme and fill it with randomly generated data, execute the following two python scripts. In the *fillDatabase.py*, you can vary the number of unique videos, unique users, overall requests, and the time interval the dataset captures. By default, it fills your database with about 700 requests consisting of 200 unique video IDs performed by 300 users in a time interval of one week. In case you use our simulation with the graphical OMNeT++ interface you should configure less than 100 distinct users or otherwise it may run slow. For usage without the graphical interface, you should create less than 800.000 distinct users, due to a limitation in OMNeT++.
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
Next, open the \emph{Makefile} and manually insert the include paths mentioned below. This needs to be done because of a known issue of OMNeT++ (https://groups.google.com/forum/\#!msg/omnetpp/VEE4oXtuPes/cSQG7vZ-AAAJ). 

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
For the execution, you need to specify a certain configuration by its name, e.g., *Run6*. The existing 1,008 configurations available are listed in the \emph{omnetpp.ini} file and located in the configurations folder. To run the simulation with graphical interface, execute the first command. For larger simulations, we recommend using no GUI by executing the second command.

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
