import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import glob
import os
import math
import datetime as dt
from matplotlib.ticker import FuncFormatter
import matplotlib.dates as mdate

pd.__version__
def parseData(filename):
    Results={'Run':[],'Repetition':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[],'AverageDelay':[],'TimeToFirstSegment':[]}
    with open("SimulationResults/Top2/"+filename, 'r') as f:
        time = []
        serves=[]
        hitrate1 = []
        hitrate2 = []
        hitrate3 = []
        hitrate4 = []
        readOps1 = []
        readOps2 = []
        readOps3 = []
        readOps4 = []
        writeOps1 = []
        writeOps2 = []
        writeOps3 = []
        writeOps4 = []
        avgDelay1 = []
        avgDelay2 = []
        avgDelay3 = []
        avgDelay4 = []
        ttfs1 = []
        ttfs2 = []
        ttfs3 = []
        ttfs4 = []
        ACDC = False
        Results['Run'] = filename.split('-', 1)[0][3:]
        Results['Repetition'] = filename.split('-', 1)[1][:-4].replace("#", "")
        for line in f:
            if line.strip():
                splitLine = line.split()
                if(splitLine[0] == "param"):
                    Results['Topology'] = splitLine[3]
                    Results['ProxyCount'] = splitLine[4]
                    
                    if(splitLine[5] == "LCE" or splitLine[5] == "LCD" or splitLine[5] == "MCD"):
                        Results['Admission'] = splitLine[5]
                        Results['AdmissionParams'] = " "
                        if(splitLine[6] != "TTL" and splitLine[6] != "ACDC"):
                            if(splitLine[6] == "ARC2"):
                                Results['Eviction'] = "ARC"
                            else:
                                Results['Eviction'] = splitLine[6]
                            Results['EvictionParams'] = " "
                            Results['Size'] = splitLine[7][:-3]
                        else:
                            if(splitLine[6] == "ACDC"):
                                ACDC = True
                                Results['Eviction'] = "ACDC"
                                Results['EvictionParams'] = " "
                            else:
                                Results['Eviction'] = splitLine[6]
                                Results['EvictionParams'] = splitLine[7]
                            Results['Size'] = splitLine[8][:-3]
                            
                    else:
                        Results['Admission'] = splitLine[5]
                        Results['AdmissionParams'] = splitLine[6].split(',', 1)[0]
                        if(splitLine[7] != "TTL" and splitLine[7] != "ACDC"):
                            if(splitLine[7] == "ARC2"):
                                Results['Eviction'] = "ARC"
                            else:
                                Results['Eviction'] = splitLine[7]
                            Results['EvictionParams'] = " "
                            Results['Size'] = splitLine[8][:-3]
                        else:
                            if(splitLine[7] == "ACDC"):
                                ACDC = True
                                Results['Eviction'] = "ACDC"
                                Results['EvictionParams'] = " "
                            else:
                                Results['Eviction'] = splitLine[7]
                                Results['EvictionParams'] = splitLine[8]
                            Results['Size'] = splitLine[9][:-3]
                            
                if(ACDC == True):
                    if(splitLine[0] == "15"):
                        time.append(float(splitLine[2])+134006400)
                        if(splitLine[3] == "-nan"):
                            hitrate1.append(0.0)
                        else:
                            hitrate1.append(float(splitLine[3]))                        
                    if(splitLine[0] == "0"):
                        serves.append(float(splitLine[3]))
                    if(splitLine[0] == "16"):
                        readOps1.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "17"):
                        writeOps1.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "18"):
                        avgDelay1.append(float(splitLine[3]))
                    if(splitLine[0] == "19"):
                        ttfs1.append(float(splitLine[3]))
                    if(splitLine[0] == "34"):
                        if(splitLine[3] == "-nan"):
                            hitrate2.append(0.0)
                        else:
                            hitrate2.append(float(splitLine[3]))
                    if(splitLine[0] == "35"):
                        readOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "36"):
                        writeOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "37"):
                        avgDelay2.append(float(splitLine[3]))
                    if(splitLine[0] == "38"):
                        ttfs2.append(float(splitLine[3]))
                    if(splitLine[0] == "53"):
                        if(splitLine[3] == "-nan"):
                            hitrate3.append(0.0)
                        else:
                            hitrate3.append(float(splitLine[3]))
                    if(splitLine[0] == "54"):
                        readOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "55"):
                        writeOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "56"):
                        avgDelay3.append(float(splitLine[3]))
                    if(splitLine[0] == "57"):
                        ttfs3.append(float(splitLine[3]))
                    if(splitLine[0] == "72"):
                        if(splitLine[3] == "-nan"):
                            hitrate4.append(0.0)
                        else:
                            hitrate4.append(float(splitLine[3]))
                    if(splitLine[0] == "73"):
                        readOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "74"):
                        writeOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "75"):
                        avgDelay4.append(float(splitLine[3]))
                    if(splitLine[0] == "76"):
                        ttfs4.append(float(splitLine[3]))
                else:
                    if(splitLine[0] == "3"):
                        time.append(float(splitLine[2])+134006400)
                        if(splitLine[3] == "-nan"):
                            hitrate1.append(0.0)
                        else:
                            hitrate1.append(float(splitLine[3]))
                    if(splitLine[0] == "0"):
                        serves.append(float(splitLine[3]))
                    if(splitLine[0] == "4"):
                        readOps1.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "5"):
                        writeOps1.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "6"):
                        avgDelay1.append(float(splitLine[3]))
                    if(splitLine[0] == "7"):
                        ttfs1.append(float(splitLine[3]))
                    if(splitLine[0] == "10"):
                        if(splitLine[3] == "-nan"):
                            hitrate2.append(0.0)
                        else:
                            hitrate2.append(float(splitLine[3]))       
                    if(splitLine[0] == "11"):
                        readOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "12"):
                        writeOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "13"):
                        avgDelay2.append(float(splitLine[3]))
                    if(splitLine[0] == "14"):
                        ttfs2.append(float(splitLine[3]))
                    if(splitLine[0] == "17"):
                        if(splitLine[3] == "-nan"):
                            hitrate3.append(0.0)
                        else:
                            hitrate3.append(float(splitLine[3]))
                    if(splitLine[0] == "18"):
                        readOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "19"):
                        writeOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "20"):
                        avgDelay3.append(float(splitLine[3]))
                    if(splitLine[0] == "21"):
                        ttfs3.append(float(splitLine[3]))
                    if(splitLine[0] == "24"):
                        if(splitLine[3] == "-nan"):
                            hitrate4.append(0.0)
                        else:
                            hitrate4.append(float(splitLine[3]))
                    if(splitLine[0] == "25"):
                        readOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "26"):
                        writeOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "27"):
                        avgDelay4.append(float(splitLine[3]))
                    if(splitLine[0] == "28"):
                        ttfs4.append(float(splitLine[3]))

        Results['Time'] = time
        Results['Serves'] = serves
        Results['Hitrate'].append(hitrate1)
        Results['Hitrate'].append(hitrate2)
        Results['Hitrate'].append(hitrate3)
        Results['Hitrate'].append(hitrate4)
        Results['ReadOperations'].append(readOps1)
        Results['ReadOperations'].append(readOps2)
        Results['ReadOperations'].append(readOps3)
        Results['ReadOperations'].append(readOps4)
        Results['WriteOperations'].append(writeOps1)
        Results['WriteOperations'].append(writeOps2)
        Results['WriteOperations'].append(writeOps3)
        Results['WriteOperations'].append(writeOps4)
        Results['AverageDelay'].append(avgDelay1)
        Results['AverageDelay'].append(avgDelay2)
        Results['AverageDelay'].append(avgDelay3)
        Results['AverageDelay'].append(avgDelay4)
        Results['TimeToFirstSegment'].append(ttfs1)
        Results['TimeToFirstSegment'].append(ttfs2)
        Results['TimeToFirstSegment'].append(ttfs3)
        Results['TimeToFirstSegment'].append(ttfs4)
    return Results
data = []   
for filename in os.listdir('SimulationResults/Top2'):
        if(filename[-4:]==".vec"):
            data.append(parseData(filename))
df = pd.DataFrame(data)
df.to_pickle('data/Top2Results.pkl')

data = pd.read_pickle('data/Top2Results.pkl')

Results={'Run':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[],'AverageDelay':[],'TimeToFirstSegment':[]}
for run in data.Run.unique():
    proxyHit = []
    proxyRead = []
    proxyWrite = []
    proxAvgDelays = []
    proxTTFSs = []
    prox1Hit = np.zeros(170)
    prox2Hit = np.zeros(170)
    prox3Hit = np.zeros(170)
    prox4Hit = np.zeros(170)
    prox1Read = np.zeros(170)
    prox2Read = np.zeros(170)
    prox3Read = np.zeros(170)
    prox4Read = np.zeros(170)
    prox1Write = np.zeros(170)
    prox2Write = np.zeros(170)
    prox3Write = np.zeros(170)
    prox4Write = np.zeros(170)
    originServes = np.zeros(170)
    prox1avgD = np.zeros(170)
    prox2avgD = np.zeros(170)
    prox3avgD = np.zeros(170)
    prox4avgD = np.zeros(170)
    prox1ttfs = np.zeros(170)
    prox2ttfs = np.zeros(170)
    prox3ttfs = np.zeros(170)
    prox4ttfs = np.zeros(170)
    
    for repetition in range(0, len(data.loc[data.Run == str(run)].Hitrate)):
        hit = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Hitrate)
        reads = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].ReadOperations)
        writes = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].WriteOperations)
        serves = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Serves)
        avgDelay = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].AverageDelay)
        ttfs = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].TimeToFirstSegment)
        
        prox1Hit = [sum(x) for x in zip(prox1Hit,hit.reset_index().Hitrate[0][0])]
        prox2Hit = [sum(x) for x in zip(prox2Hit,hit.reset_index().Hitrate[0][1])]
        prox3Hit = [sum(x) for x in zip(prox3Hit,hit.reset_index().Hitrate[0][2])]
        prox4Hit = [sum(x) for x in zip(prox4Hit,hit.reset_index().Hitrate[0][3])]
        
        prox1avgD = [sum(x) for x in zip(prox1avgD,avgDelay.reset_index().AverageDelay[0][0])]
        prox2avgD = [sum(x) for x in zip(prox2avgD,avgDelay.reset_index().AverageDelay[0][1])]
        prox3avgD = [sum(x) for x in zip(prox3avgD,avgDelay.reset_index().AverageDelay[0][2])]
        prox4avgD = [sum(x) for x in zip(prox4avgD,avgDelay.reset_index().AverageDelay[0][3])]
        
        prox1ttfs = [sum(x) for x in zip(prox1ttfs,ttfs.reset_index().TimeToFirstSegment[0][0])]
        prox2ttfs = [sum(x) for x in zip(prox2ttfs,ttfs.reset_index().TimeToFirstSegment[0][1])]
        prox3ttfs = [sum(x) for x in zip(prox3ttfs,ttfs.reset_index().TimeToFirstSegment[0][2])]
        prox4ttfs = [sum(x) for x in zip(prox4ttfs,ttfs.reset_index().TimeToFirstSegment[0][3])]
        
        prox1Read = [sum(x) for x in zip(prox1Read,reads.reset_index().ReadOperations[0][0])]
        prox2Read = [sum(x) for x in zip(prox2Read,reads.reset_index().ReadOperations[0][1])]
        prox3Read = [sum(x) for x in zip(prox3Read,reads.reset_index().ReadOperations[0][2])]
        prox4Read = [sum(x) for x in zip(prox4Read,reads.reset_index().ReadOperations[0][3])]
        
        prox1Write = [sum(x) for x in zip(prox1Write,writes.reset_index().WriteOperations[0][0])]
        prox2Write = [sum(x) for x in zip(prox2Write,writes.reset_index().WriteOperations[0][1])]
        prox3Write = [sum(x) for x in zip(prox3Write,writes.reset_index().WriteOperations[0][2])]
        prox4Write = [sum(x) for x in zip(prox4Write,writes.reset_index().WriteOperations[0][3])]
        
        originServes = [sum(x) for x in zip(originServes,serves.reset_index().Serves[0])]
        
    prox1Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox1Hit]
    prox2Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox2Hit]
    prox3Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox3Hit]
    prox4Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox4Hit]
    
    prox1avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox1avgD]
    prox2avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox2avgD]
    prox3avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox3avgD]
    prox4avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox4avgD]
    
    prox1ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox1ttfs]
    prox2ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox2ttfs]
    prox3ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox3ttfs]
    prox4ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox4ttfs]
    
    prox1Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox1Read]
    prox2Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox2Read]
    prox3Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox3Read]
    prox4Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox4Read]
    
    prox1Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox1Write]
    prox2Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox2Write]
    prox3Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox3Write]
    prox4Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox4Write]
    
    originServes[:] = [x / len(data.loc[data.Run == str(run)].Serves) for x in originServes]
    proxyHit.append(prox1Hit)
    proxyHit.append(prox2Hit)
    proxyHit.append(prox3Hit)
    proxyHit.append(prox4Hit)
    
    proxAvgDelays.append(prox1avgD)
    proxAvgDelays.append(prox2avgD)
    proxAvgDelays.append(prox3avgD)
    proxAvgDelays.append(prox4avgD)
    
    proxTTFSs.append(prox1ttfs)
    proxTTFSs.append(prox2ttfs)
    proxTTFSs.append(prox3ttfs)
    proxTTFSs.append(prox4ttfs)
    
    proxyRead.append(prox1Read)
    proxyRead.append(prox2Read)
    proxyRead.append(prox3Read)
    proxyRead.append(prox4Read)
    
    proxyWrite.append(prox1Write)
    proxyWrite.append(prox2Write)
    proxyWrite.append(prox3Write)
    proxyWrite.append(prox4Write)
    
    Results['Run'].append(data.loc[data.Run == str(run)].Run.iloc[0])
    Results['Admission'].append(data.loc[data.Run == str(run)].Admission.iloc[0])
    Results['AdmissionParams'].append(data.loc[data.Run == str(run)].AdmissionParams.iloc[0])
    Results['Eviction'].append(data.loc[data.Run == str(run)].Eviction.iloc[0])
    Results['EvictionParams'].append(data.loc[data.Run == str(run)].EvictionParams.iloc[0])
    Results['Size'].append(data.loc[data.Run == str(run)].Size.iloc[0])
    Results['ProxyCount'].append(data.loc[data.Run == str(run)].ProxyCount.iloc[0])
    Results['Topology'].append(data.loc[data.Run == str(run)].Topology.iloc[0])
    Results['Time'].append(data.loc[data.Run == str(run)].Time.iloc[0])
    Results['Hitrate'].append(proxyHit)
    Results['ReadOperations'].append(proxyRead)
    Results['WriteOperations'].append(proxyWrite)
    Results['Serves'].append(originServes)
    Results['AverageDelay'].append(proxAvgDelays)
    Results['TimeToFirstSegment'].append(proxTTFSs)
    
    
df = pd.DataFrame(Results)
df.to_pickle('data/Top2ResultsMean.pkl')

data = pd.read_pickle('data/Top2ResultsMean.pkl')

for i in range(0,len(data)):
    if(str(data.loc[i].Size) == "83886080"):
        data.loc[i].Size = "10 TB"
    if(data.loc[i].Size == "8388608"):
        data.loc[i].Size = "1 TB"
    if(data.loc[i].Size == "819200"):
        data.loc[i].Size = "100 GB"
    if(data.loc[i].Size == "81920"):
        data.loc[i].Size = "10 GB"
    if(data.loc[i].Size == "8192"):
        data.loc[i].Size = "1 GB"
data.to_pickle('data/Top2ResultsMean.pkl')

data = pd.read_pickle('data/Top2Results.pkl')

for i in range(0,len(data)):
    if(str(data.loc[i].Size) == "83886080"):
        data.loc[i].Size = "10 TB"
    if(data.loc[i].Size == "8388608"):
        data.loc[i].Size = "1 TB"
    if(data.loc[i].Size == "819200"):
        data.loc[i].Size = "100 GB"
    if(data.loc[i].Size == "81920"):
        data.loc[i].Size = "10 GB"
    if(data.loc[i].Size == "8192"):
        data.loc[i].Size = "1 GB"
data.to_pickle('data/Top2Results.pkl')