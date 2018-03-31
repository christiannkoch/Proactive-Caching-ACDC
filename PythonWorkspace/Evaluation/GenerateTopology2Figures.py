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
from scipy.integrate import simps
from numpy import trapz

pd.__version__

data = pd.read_pickle('data/Top2ResultsMean.pkl')

for i in range(0,len(data)):
    Configtitle = "Topology " + data.Topology[i] + ": "+ data.ProxyCount[i]+" Reverse Proxies. Admission Strategy: " + data.Admission[i]+ (" with parameters " + data.AdmissionParams[i] if(data.AdmissionParams[i] != " ") else "") + ". Eviction Strategy: " + data.Eviction[i] + (" with parameters " + data.EvictionParams[i] if (data.EvictionParams[i] != " ") else "") + ". Storage size of the Network: "+ data.Size[i]
    secs = mdate.epoch2num(data.Time[0])
    df = pd.DataFrame(data.Hitrate[i])
    df = df.transpose()
    df = df.set_index(secs)
    sns.set_style("darkgrid")
    my_plot = df.plot(figsize=(9,7),title=Configtitle)
    date_fmt = '%a %d'
    date_formatter = mdate.DateFormatter(date_fmt)
    my_plot.xaxis.set_major_formatter(date_formatter)
    my_plot.xaxis.set_tick_params(labelsize=15)
    my_plot.yaxis.set_tick_params(labelsize=15)
    my_plot.set_xlabel("Time", fontsize=20)
    my_plot.set_ylabel("Hit Rate", fontsize=20)
    ymax = max(data.Hitrate[i][0]+data.Hitrate[i][1]+data.Hitrate[i][2]+data.Hitrate[i][3])
    ymax = (ymax * 1.2)
    my_plot.set_ylim([0,ymax])
    nlines = len(data.Hitrate[i])
    ncol = int(math.ceil(nlines/2.))
    my_plot.legend(["Proxy 1", "Proxy 2","Proxy 3", "Proxy 4"],ncol = 4, facecolor='white',prop={'size':15},frameon=True)
    fig = my_plot.get_figure()
    fig.autofmt_xdate()
    fig.savefig('figures/Top2/'+data.Run[i]+'.png', bbox_inches='tight')

data = pd.read_pickle('data/Top2ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append((sum(data.Hitrate[j][0])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][1])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][2])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][3])/168)*100)
        
        TotalHits['Hits'].append(sum(daylyAvg)/4)
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (9,8)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 22})
sns.set(font_scale=1.7)
level=1
typ = "hitrate"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+"hit_rate"+"_"+str(n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=0)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    figur.savefig('figures/Top2/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top2ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.ReadOperations[j][0])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][1])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][2])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][3])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg)/4)
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (8,5)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 22})
sns.set(font_scale=1.7)
level=1
typ = "Read"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f')
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top2/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top2ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.WriteOperations[j][0])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][1])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][2])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][3])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg)/4)
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (8,5)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 22})
sns.set(font_scale=1.7)
level=1
typ = "Write"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=0, vmax=6)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top2/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top2ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.Serves[j])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg)/1000)
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (8,5)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 15})
sns.set(font_scale=1.7)
level=0
typ = "Load"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n, fontsize=15)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.0f')
    myplot.set_xlabel("Admission Strategies", fontsize=20)
    myplot.set_ylabel("Eviction Strategies", fontsize=20)
    cax = plt.gcf().axes[-1]
    cax.tick_params(labelsize=15)
    plt.xticks(rotation=90)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top2/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top2ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append((sum(data.Hitrate[j][0])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][1])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][2])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][3])/168)*100)
        
        daylyAvgWrites = []
        daylyAvgWrites.append(sum(data.WriteOperations[j][0])/168)
        
        daylyAvgWrites.append(sum(data.WriteOperations[j][1])/168)
        
        daylyAvgWrites.append(sum(data.WriteOperations[j][2])/168)
        
        daylyAvgWrites.append(sum(data.WriteOperations[j][3])/168)
        
        TotalHits['Hits'].append((sum(daylyAvg)/4)/(sum(daylyAvgWrites)/4))
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (9,8)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 22})
sns.set(font_scale=1.7)
level=1
typ = "hitrateDividedByWrites"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=0)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    figur.savefig('figures/Top2/Heatmaps/'+title+'.png', bbox_inches='tight')
    