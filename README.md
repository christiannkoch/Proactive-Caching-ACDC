


# Dependencies
* sudo apt install mysql-server libmysqlcppconn-dev libboost-dev python3 python3-pip git

* sudo pip3 install mysql-connector pandas seaborn

## Install OMNeT++

See the OMNeT++ installation guide in case of any issues: /doc/InstallGuide.pdf
In case you have installed python anaconda3 you have to uncomment the path for conda in ~/.bashrc file during the installation - this is a known issue of a OMNeT++ dependency (QT)

* sudo apt-get install build-essential gcc g++ bison flex perl qt5-default tcl-dev tk-dev libxml2-dev zlib1g-dev default-jre doxygen graphviz libwebkitgtk-3.0-0 libopenscenegraph-dev openscenegraph-plugin-osgearth libosgearth-dev openmpi-bin libopenmpi-dev

* in the terminal
 * execute "gedit ~/.bashrc"
  * add "export PATH=$HOME/omnetpp-5.2.1/bin:$PATH" # change path and folder name accordingly
 * reopen terminal

* in the omnet folder
 * . setenv	
 *  ./configure
 * make 
 * make install 


## OMNeT++ Include Path HotFix
It is a known OMNeT++ issue that the include path is not any more built correctly.
Hence, you have to add the following include paths manually into Testbed/Makefile after executing opp_makemake


 # C++ include paths (with -I)
INCLUDE_PATH =\\
	-I. \\
    -Iconfigurations \\
    -Iconfigurations/ACARCConfigs \\
    -Iconfigurations/categorial \\
    -Iconfigurations/Hybrid \\
    -Iconfigurations/Infinity \\
    -Iconfigurations/Special \\
    -Iconfigurations/Topologies \\
    -Iresults \\
    -Isrc \\
    -Isrc/cache \\
    -Isrc/cache/admissionStretegies \\
    -Isrc/cache/admissionStretegies/lcd \\
    -Isrc/cache/admissionStretegies/lce \\
    -Isrc/cache/admissionStretegies/mcd \\
    -Isrc/cache/admissionStretegies/nhit \\
    -Isrc/cache/admissionStretegies/prob \\
    -Isrc/cache/admissionStretegies/rand \\
    -Isrc/cache/evictionStrategies \\
    -Isrc/cache/evictionStrategies/acdc \\
    -Isrc/cache/evictionStrategies/arc1 \\
    -Isrc/cache/evictionStrategies/arc2 \\
    -Isrc/cache/evictionStrategies/fifo \\
    -Isrc/cache/evictionStrategies/ghostLists \\
    -Isrc/cache/evictionStrategies/infinity \\
    -Isrc/cache/evictionStrategies/lfu \\
    -Isrc/cache/evictionStrategies/lfuda \\
    -Isrc/cache/evictionStrategies/lru \\
    -Isrc/cache/evictionStrategies/nodes \\
    -Isrc/cache/evictionStrategies/probationary \\
    -Isrc/cache/evictionStrategies/rand \\
    -Isrc/cache/evictionStrategies/segment \\
    -Isrc/cache/evictionStrategies/slru \\
    -Isrc/cache/evictionStrategies/ttl \\
    -Isrc/database \\
    -Isrc/simulation
