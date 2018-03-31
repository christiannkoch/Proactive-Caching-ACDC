import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import seaborn as sns
import glob
import os
import math
import datetime as dt
from matplotlib.ticker import FuncFormatter
import matplotlib.dates as mdate

pd.__version__
import os

def exec_full(filepath):
    import os
    global_namespace = {
        "__file__": filepath,
        "__name__": "__main__",
    }
    with open(filepath, 'rb') as file:
        exec(compile(file.read(), filepath, 'exec'), global_namespace)

exec_full("GenerateTopology1Data.py")
data = pd.read_pickle('data/Top1ResultsMean.pkl')

Averaged = {'Order':[],'Delay':[],'Topology':[],'Admission':[],'AdmissionParams':[],'Eviction':[],'AdmissionEviction':[],'EvictionParams':[],'Size':[],'Hits':[],'Writes':[],'TimeToFirstSegment':[]}

for j in range(0,len(data.Hitrate)):
        daylyHitAvg = []
        daylyHitAvg.append((sum(data.Hitrate[j][0])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][1])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][2])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][3])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][4])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][5])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][6])/168)*100)
        
        daylyAvgWrites = []
        daylyAvgWrites.append(sum(data.WriteOperations[j][0])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][1])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][2])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][3])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][4])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][5])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][6])/168)
        
        daylyAvgDelay = []
        daylyAvgDelay.append(sum(data.AverageDelay[j][0])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][1])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][2])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][3])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][4])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][5])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][6])/168)
        
        daylyAvgTTFS = []
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][0])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][1])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][2])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][3])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][4])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][5])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][6])/168)
        
        Averaged['Topology'].append("Topology1")
        Averaged['AdmissionEviction'].append(data.Admission[j]+data.AdmissionParams[j]+data.Eviction[j])
        Averaged['Hits'].append((sum(daylyHitAvg)/7))
        Averaged['Writes'].append((sum(daylyAvgWrites)/7))
        Averaged['Delay'].append((sum(daylyAvgDelay)/7))
        Averaged['TimeToFirstSegment'].append((sum(daylyAvgTTFS)/7))
        Averaged['Admission'].append(data.Admission[j])
        Averaged['AdmissionParams'].append(data.AdmissionParams[j])
        Averaged['Eviction'].append(data.Eviction[j])
        Averaged['EvictionParams'].append(data.EvictionParams[j])
        Averaged['Size'].append(data.Size[j])
        
        if(data.Size[j] == "1 GB"):
            Averaged['Order'].append("1")
        if(data.Size[j] == "10 GB"):
            Averaged['Order'].append("2")
        if(data.Size[j] == "100 GB"):
            Averaged['Order'].append("3")
        if(data.Size[j] == "1 TB"):
            Averaged['Order'].append("4")
        if(data.Size[j] == "10 TB"):
            Averaged['Order'].append("5")
        if(data.Size[j] == "100 TB"):
            Averaged['Order'].append("6")
        if(data.Size[j] == "1 PB"):
            Averaged['Order'].append("7")
exec_full("GenerateTopology2Data.py")        
data = pd.read_pickle('data/Top2ResultsMean.pkl')
       
for j in range(0,len(data.Hitrate)):
        daylyHitAvg = []
        daylyHitAvg.append((sum(data.Hitrate[j][0])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][1])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][2])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][3])/168)*100)
        
        daylyAvgWrites = []
        daylyAvgWrites.append(sum(data.WriteOperations[j][0])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][1])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][2])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][3])/168)
        
        daylyAvgDelay = []
        daylyAvgDelay.append(sum(data.AverageDelay[j][0])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][1])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][2])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][3])/168)
        
        daylyAvgTTFS = []
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][0])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][1])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][2])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][3])/168)
        
        Averaged['Topology'].append("Topology2")
        Averaged['AdmissionEviction'].append(data.Admission[j]+data.AdmissionParams[j]+data.Eviction[j])
        Averaged['Hits'].append((sum(daylyHitAvg)/4))
        Averaged['Writes'].append((sum(daylyAvgWrites)/4))
        Averaged['Admission'].append(data.Admission[j])
        Averaged['AdmissionParams'].append(data.AdmissionParams[j])
        Averaged['Eviction'].append(data.Eviction[j])
        Averaged['EvictionParams'].append(data.EvictionParams[j])
        Averaged['Size'].append(data.Size[j])
        Averaged['Delay'].append((sum(daylyAvgDelay)/4))
        Averaged['TimeToFirstSegment'].append((sum(daylyAvgTTFS)/4))
        if(data.Size[j] == "1 GB"):
            Averaged['Order'].append("1")
        if(data.Size[j] == "10 GB"):
            Averaged['Order'].append("2")
        if(data.Size[j] == "100 GB"):
            Averaged['Order'].append("3")
        if(data.Size[j] == "1 TB"):
            Averaged['Order'].append("4")
        if(data.Size[j] == "10 TB"):
            Averaged['Order'].append("5")
        if(data.Size[j] == "100 TB"):
            Averaged['Order'].append("6")
        if(data.Size[j] == "1 PB"):
            Averaged['Order'].append("7")  
exec_full("GenerateTopology3Data.py")        
data = pd.read_pickle('data/Top3ResultsMean.pkl')
        
for j in range(0,len(data.Hitrate)):
        daylyHitAvg = []
        daylyHitAvg.append((sum(data.Hitrate[j][0])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][1])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][2])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][3])/168)*100)
        daylyHitAvg.append((sum(data.Hitrate[j][4])/168)*100)
        
        daylyAvgWrites = []
        daylyAvgWrites.append(sum(data.WriteOperations[j][0])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][1])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][2])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][3])/168)
        daylyAvgWrites.append(sum(data.WriteOperations[j][4])/168)
        
        daylyAvgDelay = []
        daylyAvgDelay.append(sum(data.AverageDelay[j][0])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][1])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][2])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][3])/168)
        daylyAvgDelay.append(sum(data.AverageDelay[j][4])/168)
        
        daylyAvgTTFS = []
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][0])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][1])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][2])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][3])/168)
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][4])/168)
        
        Averaged['Topology'].append("Topology3")
        Averaged['AdmissionEviction'].append(data.Admission[j]+data.AdmissionParams[j]+data.Eviction[j])
        Averaged['Hits'].append((sum(daylyHitAvg)/5))
        Averaged['Writes'].append((sum(daylyAvgWrites)/5))
        Averaged['Admission'].append(data.Admission[j])
        Averaged['AdmissionParams'].append(data.AdmissionParams[j])
        Averaged['Eviction'].append(data.Eviction[j])
        Averaged['EvictionParams'].append(data.EvictionParams[j])
        Averaged['Size'].append(data.Size[j])
        Averaged['Delay'].append((sum(daylyAvgDelay)/5))
        Averaged['TimeToFirstSegment'].append((sum(daylyAvgTTFS)/5))
        if(data.Size[j] == "1 GB"):
            Averaged['Order'].append("1")
        if(data.Size[j] == "10 GB"):
            Averaged['Order'].append("2")
        if(data.Size[j] == "100 GB"):
            Averaged['Order'].append("3")
        if(data.Size[j] == "1 TB"):
            Averaged['Order'].append("4")
        if(data.Size[j] == "10 TB"):
            Averaged['Order'].append("5")
        if(data.Size[j] == "100 TB"):
            Averaged['Order'].append("6")
        if(data.Size[j] == "1 PB"):
            Averaged['Order'].append("7")    
exec_full("GenerateTopology4Data.py")        
data = pd.read_pickle('data/Top4ResultsMean.pkl')        

for j in range(0,len(data.Hitrate)):
        daylyHitAvg = []
        daylyHitAvg.append((sum(data.Hitrate[j][0])/168)*100)
        
        daylyAvgWrites = []
        daylyAvgWrites.append(sum(data.WriteOperations[j][0])/168)
        
        daylyAvgDelay = []
        daylyAvgDelay.append(sum(data.AverageDelay[j][0])/168)
        
        daylyAvgTTFS = []
        daylyAvgTTFS.append(sum(data.TimeToFirstSegment[j][0])/168)
        
        Averaged['Topology'].append("Topology4")
        Averaged['AdmissionEviction'].append(data.Admission[j]+data.AdmissionParams[j]+data.Eviction[j])
        Averaged['Hits'].append((sum(daylyHitAvg)))
        Averaged['Writes'].append((sum(daylyAvgWrites)))
        Averaged['Admission'].append(data.Admission[j])
        Averaged['AdmissionParams'].append(data.AdmissionParams[j])
        Averaged['Eviction'].append(data.Eviction[j])
        Averaged['EvictionParams'].append(data.EvictionParams[j])
        Averaged['Size'].append(data.Size[j])
        Averaged['Delay'].append((sum(daylyAvgDelay)))
        Averaged['TimeToFirstSegment'].append((sum(daylyAvgTTFS)))
        if(data.Size[j] == "1 GB"):
            Averaged['Order'].append("1")
        if(data.Size[j] == "10 GB"):
            Averaged['Order'].append("2")
        if(data.Size[j] == "100 GB"):
            Averaged['Order'].append("3")
        if(data.Size[j] == "1 TB"):
            Averaged['Order'].append("4")
        if(data.Size[j] == "10 TB"):
            Averaged['Order'].append("5")
        if(data.Size[j] == "100 TB"):
            Averaged['Order'].append("6")
        if(data.Size[j] == "1 PB"):
            Averaged['Order'].append("7")
            
df = pd.DataFrame(Averaged)

df = df.sort_values(by=['Order','Eviction'])

df.to_pickle('data/Averaged.pkl')
df.to_csv('data/Averaged.csv')
df.head()

data = pd.read_pickle('data/Averaged.pkl')
data1 = data.loc[data.Eviction == 'ACDC']
bestACDCRuns = data1.groupby(['Topology','Size'])['Hits'].transform(max) == data1['Hits']
#data.groupby(['Topology','Size'], sort=False)['Hits'].max()
data4 = data1[bestACDCRuns]
data2 = data.loc[data.Eviction != 'ACDC']
bestOtherRuns =data2.groupby(['Topology','Size'])['Hits'].transform(max) == data2['Hits']
data3 = data2[bestOtherRuns]


Table = {'EvictionParams':[],'AdmissionParams':[],'Order':[],'Topology':[],'Size':[],'Admission':[],'Eviction':[],'CHR':[],'Writes':[],'Latency':[],'CHRGain':[],'WritesGain':[],'LatencyGain':[]}

for index, i in data4.iterrows():
    Admission = i.Admission
    AdmissionParams = i.AdmissionParams
    EvictionParams = i.EvictionParams
    Size = i.Size
    Topology = i.Topology
    ACDCCHR = i.Hits
    ACDCWrites = i.Writes
    ACDCLatency = i.Delay
    OtherRuns = data3.loc[(data.Size==Size) & (data.Topology == Topology)]
    if(not(OtherRuns.empty)):
        Table['Topology'].append(Topology)
        Table['Size'].append(Size)
        Table['Admission'].append(Admission)
        Table['AdmissionParams'].append(AdmissionParams)
        Table['EvictionParams'].append(EvictionParams)
        Table['Eviction'].append('ACDC')
        Table['CHR'].append(ACDCCHR)
        Table['Writes'].append(ACDCWrites)
        Table['Latency'].append(ACDCLatency)
        Table['CHRGain'].append(((ACDCCHR-OtherRuns.Hits.iloc[0])/OtherRuns.Hits.iloc[0])*100) 
        Table['WritesGain'].append(((ACDCWrites-OtherRuns.Writes.iloc[0])/OtherRuns.Writes.iloc[0])*100)
        Table['LatencyGain'].append(((ACDCLatency-OtherRuns.Delay.iloc[0])/OtherRuns.Delay.iloc[0])*100)
        if(Size == "1 GB"):
            Table['Order'].append("1")
        if(Size == "10 GB"):
            Table['Order'].append("2")
        if(Size == "100 GB"):
            Table['Order'].append("3")
        if(Size == "1 TB"):
            Table['Order'].append("4")
        if(Size == "10 TB"):
            Table['Order'].append("5")
        if(Size == "100 TB"):
            Table['Order'].append("6")
        if(Size == "1 PB"):
            Table['Order'].append("7")
    
df = pd.DataFrame(Table)
df = df.sort_values(by=['Topology','Order','Admission','Eviction'])
df.to_csv('data/ACDCComparison.csv')