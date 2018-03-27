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
if(os.listdir('SimulationResults/Top4') != []):
    def parseData(filename):
        Results={'Run':[],'Repetition':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[]}
        with open("SimulationResults/Top4/"+filename, 'r') as f:
            time = []
            serves=[]
            hitrate1 = []
            readOps1 = []
            writeOps1 = []
            ACARC = False
            Results['Run'] = filename.split('-', 1)[0][3:]
            Results['Repetition'] = filename.split('-', 1)[1][:-4]
            for line in f:
                if line.strip():
                    splitLine = line.split()
                    if(splitLine[0] == "param"):
                        Results['Topology'] = splitLine[3]
                        Results['ProxyCount'] = splitLine[4]

                        if(splitLine[5] == "LCE" or splitLine[5] == "LCD" or splitLine[5] == "MCD"):
                            Results['Admission'] = splitLine[5]
                            Results['AdmissionParams'] = " "
                            if(splitLine[6] != "TTL" and splitLine[6] != "ACARC"):
                                if(splitLine[6] == "ARC2"):
                                    Results['Eviction'] = "ARC"
                                else:
                                    Results['Eviction'] = splitLine[6]
                                Results['EvictionParams'] = " "
                                Results['Size'] = splitLine[7][:-3]
                            else:
                                if(splitLine[6] == "ACARC"):
                                    ACARC = True
                                    Results['Eviction'] = "ACDC"
                                    Results['EvictionParams'] = " "
                                else:
                                    Results['Eviction'] = splitLine[6]
                                    Results['EvictionParams'] = splitLine[7]
                                Results['Size'] = splitLine[8][:-3]

                        else:
                            Results['Admission'] = splitLine[5]
                            Results['AdmissionParams'] = splitLine[6].split(',', 1)[0]
                            if(splitLine[7] != "TTL" and splitLine[7] != "ACARC"):
                                if(splitLine[7] == "ARC2"):
                                    Results['Eviction'] = "ARC"
                                else:
                                    Results['Eviction'] = splitLine[7]
                                Results['EvictionParams'] = " "
                                Results['Size'] = splitLine[8][:-3]
                            else:
                                if(splitLine[7] == "ACARC"):
                                    ACARC = True
                                    Results['Eviction'] = "ACDC"
                                    Results['EvictionParams'] = " "
                                else:
                                    Results['Eviction'] = splitLine[7]
                                    Results['EvictionParams'] = splitLine[8]
                                Results['Size'] = splitLine[9][:-3]

                    if(ACARC == True):
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
            Results['Time'] = time            
            Results['Serves'] =  serves
            Results['Hitrate'].append(hitrate1)
            Results['ReadOperations'].append(readOps1)
            Results['WriteOperations'].append(writeOps1)
        return Results
    data = []   
    for filename in os.listdir('SimulationResults/Top4'):
            if(filename[-4:]==".vec"):
                data.append(parseData(filename))
    df = pd.DataFrame(data)
    df.to_pickle('data/Top4Results.pkl')

    Results={'Run':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[]}
    for run in data.Run.unique():
        proxyHit = []
        proxyRead = []
        proxyWrite = []
        prox1Hit = np.zeros(170)
        prox1Read = np.zeros(170)
        prox1Write = np.zeros(170)
        originServes = np.zeros(170)

        for repetition in range(0, len(data.loc[data.Run == str(run)].Hitrate)):
            hits = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Hitrate)
            reads = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].ReadOperations)
            writes = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].WriteOperations)
            serves = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Serves)

            prox1Hit = [sum(x) for x in zip(prox1Hit,hits.reset_index().Hitrate[0][0])]

            prox1Read = [sum(x) for x in zip(prox1Read,reads.reset_index().ReadOperations[0][0])]

            prox1Write = [sum(x) for x in zip(prox1Write,writes.reset_index().WriteOperations[0][0])]

            originServes = [sum(x) for x in zip(originServes,serves.reset_index().Serves[0])]

        prox1Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox1Hit]

        prox1Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox1Read]

        prox1Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox1Write]

        originServes[:] = [x / len(data.loc[data.Run == str(run)].Serves) for x in originServes]
        proxyHit.append(prox1Hit)

        proxyRead.append(prox1Read)

        proxyWrite.append(prox1Write)

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
    df = pd.DataFrame(Results)
    df.to_pickle('data/Top4ResultsMean.pkl')

    data = pd.read_pickle('data/Top4ResultsMean.pkl')

    delay = []
    indexList = list(data.index)
    Result = {'AverageDelay':[]}
    for i in indexList:
        avgDelay = []
        prox0Delay = []
        for g in range(0, len(data.loc[i].Hitrate[0])):
            prox0Delay.append((1-data.loc[i].Hitrate[0][g])*48)
        avgDelay = prox0Delay
        Result['AverageDelay'].append(avgDelay)

    df = pd.DataFrame(Result)
    data['AverageDelay'] = pd.Series(df.AverageDelay, index=data.index)
    data.to_pickle('data/Top4ResultsMean.pkl')

    data = pd.read_pickle('data/Top4Results.pkl')

    delay = []
    indexList = list(data.index)
    Result = {'AverageDelay':[]}
    for i in indexList:
        avgDelay = []
        prox0Delay = []
        for g in range(0, len(data.loc[i].Hitrate[0])):
            prox0Delay.append((1-data.loc[i].Hitrate[0][g])*48)
        avgDelay = prox0Delay
        Result['AverageDelay'].append(avgDelay)

    df = pd.DataFrame(Result)
    data['AverageDelay'] = pd.Series(df.AverageDelay, index=data.index)
    data.to_pickle('data/Top4Results.pkl')