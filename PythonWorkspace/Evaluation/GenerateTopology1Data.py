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
    with open("SimulationResults/Top1/"+filename, 'r') as f:
        time = []
        serves=[]
        hitrate1 = []
        hitrate2 = []
        hitrate3 = []
        hitrate4 = []
        hitrate5 = []
        hitrate6 = []
        hitrate7 = []
        readOps1 = []
        readOps2 = []
        readOps3 = []
        readOps4 = []
        readOps5 = []
        readOps6 = []
        readOps7 = []
        writeOps1 = []
        writeOps2 = []
        writeOps3 = []
        writeOps4 = []
        writeOps5 = []
        writeOps6 = []
        writeOps7 = []
        avgDelay1 = []
        avgDelay2 = []
        avgDelay3 = []
        avgDelay4 = []
        avgDelay5 = []
        avgDelay6 = []
        avgDelay7 = []
        ttfs1 = []
        ttfs2 = []
        ttfs3 = []
        ttfs4 = []
        ttfs5 = []
        ttfs6 = []
        ttfs7 = []
        ACARC = False
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
                    if(splitLine[0] == "91"):
                        if(splitLine[3] == "-nan"):
                            hitrate5.append(0.0)
                        else:
                            hitrate5.append(float(splitLine[3]))
                    if(splitLine[0] == "92"):
                        readOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "93"):
                        writeOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "94"):
                        avgDelay5.append(float(splitLine[3]))
                    if(splitLine[0] == "95"):
                        ttfs5.append(float(splitLine[3]))
                    if(splitLine[0] == "110"):
                        if(splitLine[3] == "-nan"):
                            hitrate6.append(0.0)
                        else:
                            hitrate6.append(float(splitLine[3]))
                    if(splitLine[0] == "111"):
                        readOps6.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "112"):
                        writeOps6.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "113"):
                        avgDelay6.append(float(splitLine[3]))
                    if(splitLine[0] == "114"):
                        ttfs6.append(float(splitLine[3]))
                    if(splitLine[0] == "129"):
                        if(splitLine[3] == "-nan"):
                            hitrate7.append(0.0)
                        else:
                            hitrate7.append(float(splitLine[3]))
                    if(splitLine[0] == "130"):
                        readOps7.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "131"):
                        writeOps7.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "132"):
                        avgDelay7.append(float(splitLine[3]))
                    if(splitLine[0] == "133"):
                        ttfs7.append(float(splitLine[3]))
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
                    if(splitLine[0] == "31"):
                        if(splitLine[3] == "-nan"):
                            hitrate5.append(0.0)
                        else:
                            hitrate5.append(float(splitLine[3]))
                    if(splitLine[0] == "32"):
                        readOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "33"):
                        writeOps5.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "34"):
                        avgDelay5.append(float(splitLine[3]))
                    if(splitLine[0] == "35"):
                        ttfs5.append(float(splitLine[3]))
                    if(splitLine[0] == "38"):
                        if(splitLine[3] == "-nan"):
                            hitrate6.append(0.0)
                        else:
                            hitrate6.append(float(splitLine[3]))
                    if(splitLine[0] == "39"):
                        readOps6.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "40"):
                        writeOps6.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "41"):
                        avgDelay6.append(float(splitLine[3]))
                    if(splitLine[0] == "42"):
                        ttfs6.append(float(splitLine[3]))
                    if(splitLine[0] == "45"):
                        if(splitLine[3] == "-nan"):
                            hitrate7.append(0.0)
                        else:
                            hitrate7.append(float(splitLine[3]))
                    if(splitLine[0] == "46"):
                        readOps7.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "47"):
                        writeOps7.append(math.log10(float(splitLine[3])) if (float(splitLine[3]) > 0) else 0)
                    if(splitLine[0] == "48"):
                        avgDelay7.append(float(splitLine[3]))
                    if(splitLine[0] == "49"):
                        ttfs7.append(float(splitLine[3]))
                    
        Results['Time'] = time
        Results['Serves'] = serves
        Results['Hitrate'].append(hitrate1)
        Results['Hitrate'].append(hitrate2)
        Results['Hitrate'].append(hitrate3)
        Results['Hitrate'].append(hitrate4)
        Results['Hitrate'].append(hitrate5)
        Results['Hitrate'].append(hitrate6)
        Results['Hitrate'].append(hitrate7)
        Results['ReadOperations'].append(readOps1)
        Results['ReadOperations'].append(readOps2)
        Results['ReadOperations'].append(readOps3)
        Results['ReadOperations'].append(readOps4)
        Results['ReadOperations'].append(readOps5)
        Results['ReadOperations'].append(readOps6)
        Results['ReadOperations'].append(readOps7)
        Results['WriteOperations'].append(writeOps1)
        Results['WriteOperations'].append(writeOps2)
        Results['WriteOperations'].append(writeOps3)
        Results['WriteOperations'].append(writeOps4)
        Results['WriteOperations'].append(writeOps5)
        Results['WriteOperations'].append(writeOps6)
        Results['WriteOperations'].append(writeOps7)
        Results['AverageDelay'].append(avgDelay1)
        Results['AverageDelay'].append(avgDelay2)
        Results['AverageDelay'].append(avgDelay3)
        Results['AverageDelay'].append(avgDelay4)
        Results['AverageDelay'].append(avgDelay5)
        Results['AverageDelay'].append(avgDelay6)
        Results['AverageDelay'].append(avgDelay7)
        Results['TimeToFirstSegment'].append(ttfs1)
        Results['TimeToFirstSegment'].append(ttfs2)
        Results['TimeToFirstSegment'].append(ttfs3)
        Results['TimeToFirstSegment'].append(ttfs4)
        Results['TimeToFirstSegment'].append(ttfs5)
        Results['TimeToFirstSegment'].append(ttfs6)
        Results['TimeToFirstSegment'].append(ttfs7)
    return Results
data = []   
for filename in os.listdir('SimulationResults/Top1'):
        if(filename[-4:]==".vec"):
            data.append(parseData(filename))
df = pd.DataFrame(data)
df.to_pickle('data/Top1Results.pkl')

data = pd.read_pickle('data/Top1Results.pkl')

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
    prox5Hit = np.zeros(170)
    prox6Hit = np.zeros(170)
    prox7Hit = np.zeros(170)
    prox1Read = np.zeros(170)
    prox2Read = np.zeros(170)
    prox3Read = np.zeros(170)
    prox4Read = np.zeros(170)
    prox5Read = np.zeros(170)
    prox6Read = np.zeros(170)
    prox7Read = np.zeros(170)
    prox1Write = np.zeros(170)
    prox2Write = np.zeros(170)
    prox3Write = np.zeros(170)
    prox4Write = np.zeros(170)
    prox5Write = np.zeros(170)
    prox6Write = np.zeros(170)
    prox7Write = np.zeros(170)
    originServes = np.zeros(170)
    prox1avgD = np.zeros(170)
    prox2avgD = np.zeros(170)
    prox3avgD = np.zeros(170)
    prox4avgD = np.zeros(170)
    prox5avgD = np.zeros(170)
    prox6avgD = np.zeros(170)
    prox7avgD = np.zeros(170)
    prox1ttfs = np.zeros(170)
    prox2ttfs = np.zeros(170)
    prox3ttfs = np.zeros(170)
    prox4ttfs = np.zeros(170)
    prox5ttfs = np.zeros(170)
    prox6ttfs = np.zeros(170)
    prox7ttfs = np.zeros(170)
    
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
        prox5Hit = [sum(x) for x in zip(prox5Hit,hit.reset_index().Hitrate[0][4])]
        prox6Hit = [sum(x) for x in zip(prox6Hit,hit.reset_index().Hitrate[0][5])]
        prox7Hit = [sum(x) for x in zip(prox7Hit,hit.reset_index().Hitrate[0][6])]
        
        prox1avgD = [sum(x) for x in zip(prox1avgD,avgDelay.reset_index().AverageDelay[0][0])]
        prox2avgD = [sum(x) for x in zip(prox2avgD,avgDelay.reset_index().AverageDelay[0][1])]
        prox3avgD = [sum(x) for x in zip(prox3avgD,avgDelay.reset_index().AverageDelay[0][2])]
        prox4avgD = [sum(x) for x in zip(prox4avgD,avgDelay.reset_index().AverageDelay[0][3])]
        prox5avgD = [sum(x) for x in zip(prox5avgD,avgDelay.reset_index().AverageDelay[0][4])]
        prox6avgD = [sum(x) for x in zip(prox6avgD,avgDelay.reset_index().AverageDelay[0][5])]
        prox7avgD = [sum(x) for x in zip(prox7avgD,avgDelay.reset_index().AverageDelay[0][6])]
        
        prox1ttfs = [sum(x) for x in zip(prox1ttfs,ttfs.reset_index().TimeToFirstSegment[0][0])]
        prox2ttfs = [sum(x) for x in zip(prox2ttfs,ttfs.reset_index().TimeToFirstSegment[0][1])]
        prox3ttfs = [sum(x) for x in zip(prox3ttfs,ttfs.reset_index().TimeToFirstSegment[0][2])]
        prox4ttfs = [sum(x) for x in zip(prox4ttfs,ttfs.reset_index().TimeToFirstSegment[0][3])]
        prox5ttfs = [sum(x) for x in zip(prox5ttfs,ttfs.reset_index().TimeToFirstSegment[0][4])]
        prox6ttfs = [sum(x) for x in zip(prox6ttfs,ttfs.reset_index().TimeToFirstSegment[0][5])]
        prox7ttfs = [sum(x) for x in zip(prox7ttfs,ttfs.reset_index().TimeToFirstSegment[0][6])]
        
        prox1Read = [sum(x) for x in zip(prox1Read,reads.reset_index().ReadOperations[0][0])]
        prox2Read = [sum(x) for x in zip(prox2Read,reads.reset_index().ReadOperations[0][1])]
        prox3Read = [sum(x) for x in zip(prox3Read,reads.reset_index().ReadOperations[0][2])]
        prox4Read = [sum(x) for x in zip(prox4Read,reads.reset_index().ReadOperations[0][3])]
        prox5Read = [sum(x) for x in zip(prox5Read,reads.reset_index().ReadOperations[0][4])]
        prox6Read = [sum(x) for x in zip(prox6Read,reads.reset_index().ReadOperations[0][5])]
        prox7Read = [sum(x) for x in zip(prox7Read,reads.reset_index().ReadOperations[0][6])]
        
        prox1Write = [sum(x) for x in zip(prox1Write,writes.reset_index().WriteOperations[0][0])]
        prox2Write = [sum(x) for x in zip(prox2Write,writes.reset_index().WriteOperations[0][1])]
        prox3Write = [sum(x) for x in zip(prox3Write,writes.reset_index().WriteOperations[0][2])]
        prox4Write = [sum(x) for x in zip(prox4Write,writes.reset_index().WriteOperations[0][3])]
        prox5Write = [sum(x) for x in zip(prox5Write,writes.reset_index().WriteOperations[0][4])]
        prox6Write = [sum(x) for x in zip(prox6Write,writes.reset_index().WriteOperations[0][5])]
        prox7Write = [sum(x) for x in zip(prox7Write,writes.reset_index().WriteOperations[0][6])]
        
        originServes = [sum(x) for x in zip(originServes,serves.reset_index().Serves[0])]
        
    prox1Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox1Hit]
    prox2Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox2Hit]
    prox3Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox3Hit]
    prox4Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox4Hit]
    prox5Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox5Hit]
    prox6Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox6Hit]
    prox7Hit[:] = [x / len(data.loc[data.Run == str(run)].Hitrate) for x in prox7Hit]
    
    prox1avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox1avgD]
    prox2avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox2avgD]
    prox3avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox3avgD]
    prox4avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox4avgD]
    prox5avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox5avgD]
    prox6avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox6avgD]
    prox7avgD[:] = [x / len(data.loc[data.Run == str(run)].AverageDelay) for x in prox7avgD]
    
    prox1ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox1ttfs]
    prox2ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox2ttfs]
    prox3ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox3ttfs]
    prox4ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox4ttfs]
    prox5ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox5ttfs]
    prox6ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox6ttfs]
    prox7ttfs[:] = [x / len(data.loc[data.Run == str(run)].TimeToFirstSegment) for x in prox7ttfs]
    
    prox1Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox1Read]
    prox2Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox2Read]
    prox3Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox3Read]
    prox4Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox4Read]
    prox5Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox5Read]
    prox6Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox6Read]
    prox7Read[:] = [x / len(data.loc[data.Run == str(run)].ReadOperations) for x in prox7Read]
    
    prox1Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox1Write]
    prox2Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox2Write]
    prox3Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox3Write]
    prox4Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox4Write]
    prox5Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox5Write]
    prox6Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox6Write]
    prox7Write[:] = [x / len(data.loc[data.Run == str(run)].WriteOperations) for x in prox7Write]
    
    originServes[:] = [x / len(data.loc[data.Run == str(run)].Serves) for x in originServes]
    proxyHit.append(prox1Hit)
    proxyHit.append(prox2Hit)
    proxyHit.append(prox3Hit)
    proxyHit.append(prox4Hit)
    proxyHit.append(prox5Hit)
    proxyHit.append(prox6Hit)
    proxyHit.append(prox7Hit)
    
    proxAvgDelays.append(prox1avgD)
    proxAvgDelays.append(prox2avgD)
    proxAvgDelays.append(prox3avgD)
    proxAvgDelays.append(prox4avgD)
    proxAvgDelays.append(prox5avgD)
    proxAvgDelays.append(prox6avgD)
    proxAvgDelays.append(prox7avgD)
    
    proxTTFSs.append(prox1ttfs)
    proxTTFSs.append(prox2ttfs)
    proxTTFSs.append(prox3ttfs)
    proxTTFSs.append(prox4ttfs)
    proxTTFSs.append(prox5ttfs)
    proxTTFSs.append(prox6ttfs)
    proxTTFSs.append(prox7ttfs)
    
    proxyRead.append(prox1Read)
    proxyRead.append(prox2Read)
    proxyRead.append(prox3Read)
    proxyRead.append(prox4Read)
    proxyRead.append(prox5Read)
    proxyRead.append(prox6Read)
    proxyRead.append(prox7Read)
    
    proxyWrite.append(prox1Write)
    proxyWrite.append(prox2Write)
    proxyWrite.append(prox3Write)
    proxyWrite.append(prox4Write)
    proxyWrite.append(prox5Write)
    proxyWrite.append(prox6Write)
    proxyWrite.append(prox7Write)
    
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
df.to_pickle('data/Top1ResultsMean.pkl')

data = pd.read_pickle('data/Top1ResultsMean.pkl')

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
data.to_pickle('data/Top1ResultsMean.pkl')

data = pd.read_pickle('data/Top1Results.pkl')

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
data.to_pickle('data/Top1Results.pkl')