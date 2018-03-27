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
if(os.listdir('SimulationResults/Top3') != []):
    def parseData(filename):
        Results={'Run':[],'Repetition':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[]}
        with open("SimulationResults/Top3/"+filename, 'r') as f:
            time = []
            serves=[]
            hitrate1 = []
            hitrate2 = []
            hitrate3 = []
            hitrate4 = []
            hitrate5 = []
            readOps1 = []
            readOps2 = []
            readOps3 = []
            readOps4 = []
            readOps5 = []
            writeOps1 = []
            writeOps2 = []
            writeOps3 = []
            writeOps4 = []
            writeOps5 = []
            ACARC = False
            Results['Run'] = filename.split('-', 1)[0][3:]
            Results['Repetition'] = filename.split('-', 1)[1][:-4]
            for line in f:
                if line.strip():
                    splitLine = line.split()
                    if(splitLine[0] == "param"):
                        Results['Topology'] = splitLine[3]
                        Results['ProxyCount'] = splitLine[4]
                        #print(filename)
                        #print(splitLine[4])
                        #print(splitLine[5])
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
                        if(splitLine[0] == "34"):
                            if(splitLine[3] == "-nan"):
                                hitrate2.append(0.0)
                            else:
                                hitrate2.append(float(splitLine[3]))
                        if(splitLine[0] == "35"):
                            readOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "36"):
                            writeOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "53"):
                            if(splitLine[3] == "-nan"):
                                hitrate3.append(0.0)
                            else:
                                hitrate3.append(float(splitLine[3]))
                        if(splitLine[0] == "54"):
                            readOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "55"):
                            writeOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "72"):
                            if(splitLine[3] == "-nan"):
                                hitrate4.append(0.0)
                            else:
                                hitrate4.append(float(splitLine[3]))
                        if(splitLine[0] == "73"):
                            readOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "74"):
                            writeOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "91"):
                            if(splitLine[3] == "-nan"):
                                hitrate5.append(0.0)
                            else:
                                hitrate5.append(float(splitLine[3]))
                        if(splitLine[0] == "92"):
                            readOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "93"):
                            writeOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
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
                        if(splitLine[0] == "10"):
                            if(splitLine[3] == "-nan"):
                                hitrate2.append(0.0)
                            else:
                                hitrate2.append(float(splitLine[3]))
                        if(splitLine[0] == "11"):
                            readOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "12"):
                            writeOps2.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "17"):
                            if(splitLine[3] == "-nan"):
                                hitrate3.append(0.0)
                            else:
                                hitrate3.append(float(splitLine[3]))
                        if(splitLine[0] == "18"):
                            readOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "19"):
                            writeOps3.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "24"):
                            if(splitLine[3] == "-nan"):
                                hitrate4.append(0.0)
                            else:
                                hitrate4.append(float(splitLine[3]))
                        if(splitLine[0] == "25"):
                            readOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "26"):
                            writeOps4.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "31"):
                            if(splitLine[3] == "-nan"):
                                hitrate5.append(0.0)
                            else:
                                hitrate5.append(float(splitLine[3]))
                        if(splitLine[0] == "22"):
                            readOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                        if(splitLine[0] == "23"):
                            writeOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
            Results['Time'] = time
            Results['Serves'] = serves
            Results['Hitrate'].append(hitrate1)
            Results['Hitrate'].append(hitrate2)
            Results['Hitrate'].append(hitrate3)
            Results['Hitrate'].append(hitrate4)
            Results['Hitrate'].append(hitrate5)
            Results['ReadOperations'].append(readOps1)
            Results['ReadOperations'].append(readOps2)
            Results['ReadOperations'].append(readOps3)
            Results['ReadOperations'].append(readOps4)
            Results['ReadOperations'].append(readOps5)
            Results['WriteOperations'].append(writeOps1)
            Results['WriteOperations'].append(writeOps2)
            Results['WriteOperations'].append(writeOps3)
            Results['WriteOperations'].append(writeOps4)
            Results['WriteOperations'].append(writeOps5)
        return Results
    data = []   
    for filename in os.listdir('SimulationResults/Top3'):
            if(filename[-4:]==".vec"):
                data.append(parseData(filename))
    df = pd.DataFrame(data)
    df.to_pickle('data/Top3Results.pkl')

    data = pd.read_pickle('data/Top3Results.pkl')

    Results={'Run':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'ProxyCount':[],'Topology':[],'Time':[],'Hitrate':[], 'ReadOperations':[], 'WriteOperations':[],'Serves':[]}
    for run in data.Run.unique():
        proxyHit = []
        proxyRead = []
        proxyWrite = []
        prox1Hit = np.zeros(170)
        prox2Hit = np.zeros(170)
        prox3Hit = np.zeros(170)
        prox4Hit = np.zeros(170)
        prox5Hit = np.zeros(170)
        prox1Read = np.zeros(170)
        prox2Read = np.zeros(170)
        prox3Read = np.zeros(170)
        prox4Read = np.zeros(170)
        prox5Read = np.zeros(170)
        prox1Write = np.zeros(170)
        prox2Write = np.zeros(170)
        prox3Write = np.zeros(170)
        prox4Write = np.zeros(170)
        prox5Write = np.zeros(170)
        originServes = np.zeros(170)

        for repetition in range(0, len(data.loc[data.Run == str(run)].Hitrate)):
            hits = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Hitrate)
            reads = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].ReadOperations)
            writes = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].WriteOperations)
            serves = (data.loc[data.Run == str(run)].loc[data.Repetition == str(repetition)].Serves)

            prox1Hit = [sum(x) for x in zip(prox1Hit,hits.reset_index().Hitrate[0][0])]
            prox2Hit = [sum(x) for x in zip(prox2Hit,hits.reset_index().Hitrate[0][1])]
            prox3Hit = [sum(x) for x in zip(prox3Hit,hits.reset_index().Hitrate[0][2])]
            prox4Hit = [sum(x) for x in zip(prox4Hit,hits.reset_index().Hitrate[0][3])]
            prox5Hit = [sum(x) for x in zip(prox5Hit,hits.reset_index().Hitrate[0][4])]

            prox1Read = [sum(x) for x in zip(prox1Read,reads.reset_index().ReadOperations[0][0])]
            prox2Read = [sum(x) for x in zip(prox2Read,reads.reset_index().ReadOperations[0][1])]
            prox3Read = [sum(x) for x in zip(prox3Read,reads.reset_index().ReadOperations[0][2])]
            prox4Read = [sum(x) for x in zip(prox4Read,reads.reset_index().ReadOperations[0][3])]
            prox5Read = [sum(x) for x in zip(prox5Read,reads.reset_index().ReadOperations[0][4])]

            prox1Write = [sum(x) for x in zip(prox1Write,writes.reset_index().WriteOperations[0][0])]
            prox2Write = [sum(x) for x in zip(prox2Write,writes.reset_index().WriteOperations[0][1])]
            prox3Write = [sum(x) for x in zip(prox3Write,writes.reset_index().WriteOperations[0][2])]
            prox4Write = [sum(x) for x in zip(prox4Write,writes.reset_index().WriteOperations[0][3])]
            prox5Write = [sum(x) for x in zip(prox5Write,writes.reset_index().WriteOperations[0][4])]

            originServes = [sum(x) for x in zip(originServes,serves.reset_index().Serves[0])]

        prox1Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox1Hit]
        prox2Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox2Hit]
        prox3Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox3Hit]
        prox4Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox4Hit]
        prox5Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox5Hit]

        prox1Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox1Read]
        prox2Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox2Read]
        prox3Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox3Read]
        prox4Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox4Read]
        prox5Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox5Read]

        prox1Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox1Write]
        prox2Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox2Write]
        prox3Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox3Write]
        prox4Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox4Write]
        prox5Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox5Write]

        originServes[:] = [x / len(data.loc[data.Run == str(run)].Serves) for x in originServes]
        proxyHit.append(prox1Hit)
        proxyHit.append(prox2Hit)
        proxyHit.append(prox3Hit)
        proxyHit.append(prox4Hit)
        proxyHit.append(prox5Hit)

        proxyRead.append(prox1Read)
        proxyRead.append(prox2Read)
        proxyRead.append(prox3Read)
        proxyRead.append(prox4Read)
        proxyRead.append(prox5Read)

        proxyWrite.append(prox1Write)
        proxyWrite.append(prox2Write)
        proxyWrite.append(prox3Write)
        proxyWrite.append(prox4Write)
        proxyWrite.append(prox5Write)

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
    df.to_pickle('data/Top3ResultsMean.pkl')

    data = pd.read_pickle('data/Top3ResultsMean.pkl')

    delay = []
    indexList = list(data.index)
    Result = {'AverageDelay':[]}
    for i in indexList:
        avgDelay = []
        prox1Delay = []
        prox2Delay = []
        prox3Delay = []
        prox4Delay = []
        gesamt1 = []
        revProx1 = []

        gesamt2 = []
        revProx2 = []

        gesamt3 = []
        revProx3 = []

        gesamt4 = []
        revProx4 = []
        for g in range(0, len(data.loc[i].Hitrate[0])):
            gesamt1 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[1][g]))*48 
            revProx1 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[1][g]) * 24
            prox1Delay.append(gesamt1+revProx1)

            gesamt2 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[2][g]))*48 
            revProx2 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[2][g]) * 24
            prox2Delay.append(gesamt2+revProx2)

            gesamt3 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[3][g]))*48 
            revProx3 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[3][g]) * 24
            prox3Delay.append(gesamt3+revProx3)

            gesamt4 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[4][g]))*48 
            revProx4 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[4][g]) * 24
            prox4Delay.append(gesamt4+revProx4)
        avgDelay = [(x+y+z+q)/4 for x,y,z,q in zip(prox1Delay,prox2Delay,prox3Delay,prox4Delay)]
        Result['AverageDelay'].append(avgDelay)

    df = pd.DataFrame(Result)
    data['AverageDelay'] = pd.Series(df.AverageDelay, index=data.index)
    data.to_pickle('data/Top3ResultsMean.pkl')

    data = pd.read_pickle('data/Top3Results.pkl')

    delay = []
    indexList = list(data.index)
    Result = {'AverageDelay':[]}
    for i in indexList:
        avgDelay = []
        prox1Delay = []
        prox2Delay = []
        prox3Delay = []
        prox4Delay = []
        gesamt1 = []
        revProx1 = []

        gesamt2 = []
        revProx2 = []

        gesamt3 = []
        revProx3 = []

        gesamt4 = []
        revProx4 = []
        #print(data.loc[i].Run)
        #print(data.loc[i].Repetition)
        #print(len(data.loc[i].Hitrate[0]))
        #print(len(data.loc[i].Hitrate[1]))
        #print(len(data.loc[i].Hitrate[2]))
        #print(len(data.loc[i].Hitrate[3]))
        #print(len(data.loc[i].Hitrate[4]))
        for g in range(0, len(data.loc[i].Hitrate[0])):
            gesamt1 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[1][g]))*48 
            revProx1 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[1][g]) * 24
            prox1Delay.append(gesamt1+revProx1)

            gesamt2 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[2][g]))*48 
            revProx2 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[2][g]) * 24
            prox2Delay.append(gesamt2+revProx2)

            gesamt3 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[3][g]))*48 
            revProx3 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[3][g]) * 24
            prox3Delay.append(gesamt3+revProx3)

            gesamt4 = (1-data.loc[i].Hitrate[0][g]*(1-data.loc[i].Hitrate[4][g]))*48 
            revProx4 = (data.loc[i].Hitrate[0][g])*(1-data.loc[i].Hitrate[4][g]) * 24
            prox4Delay.append(gesamt4+revProx4)
        avgDelay = [(x+y+z+q)/4 for x,y,z,q in zip(prox1Delay,prox2Delay,prox3Delay,prox4Delay)]
        Result['AverageDelay'].append(avgDelay)

    df = pd.DataFrame(Result)
    data['AverageDelay'] = pd.Series(df.AverageDelay, index=data.index)
    data.to_pickle('data/Top3Results.pkl')