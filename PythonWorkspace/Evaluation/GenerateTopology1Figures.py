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

data = pd.read_pickle('data/Top1ResultsMean.pkl')

for i in range(0,len(data)):
        Configtitle = "Topology " + data.Topology[i] + ": "+ data.ProxyCount[i]+" Reverseproxys. Admission Strategy: " + data.Admission[i]+ (" with parameters " + data.AdmissionParams[i] if(data.AdmissionParams[i] != " ") else "") + ". Eviction Strategy: " + data.Eviction[i] + (" with parameters " + data.EvictionParams[i] if (data.EvictionParams[i] != " ") else "") + ". Storage size of the Network: "+ data.Size[i]
        #Hitrates = {'Hitrate1':data.Hitrate[i][0],'Hitrate2':data.Hitrate[i][1],'Hitrate3':data.Hitrate[i][2],'Hitrate4':data.Hitrate[i][3],'Hitrate5':data.Hitrate[i][4],'Hitrate6':data.Hitrate[i][5],'Hitrate7':data.Hitrate[i][6]}    
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
        #ticks = my_plot.set_xticklabels([d.strftime('%a') for d in df.index])
        my_plot.set_xlabel("Time", fontsize=20)
        my_plot.set_ylabel("Hitrate", fontsize=20)
        ymax = max(data.Hitrate[i][0]+data.Hitrate[i][1]+data.Hitrate[i][2]+data.Hitrate[i][3]+data.Hitrate[i][4]+data.Hitrate[i][5]+data.Hitrate[i][6])
        ymax = (ymax * 1.2)
        my_plot.set_ylim([0,ymax])
        nlines = len(data.Hitrate[i])
        ncol = int(math.ceil(nlines/2.))
        #area = trapz(data.Hitrate[i], dx=170)
        my_plot.legend(["Proxy 1", "Proxy 2","Proxy ","Proxy 4","Proxy 5","Proxy 6","Proxy 7"],ncol = 4, facecolor='white',prop={'size':15},frameon=True)
        fig = my_plot.get_figure()
        fig.autofmt_xdate()
        fig.savefig('figures/Top1/'+data.Run[i]+'.png', bbox_inches='tight')
        
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append((sum(data.Hitrate[j][3])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][4])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][5])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][6])/168)*100)
        
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
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=0)
    #myplot.xaxis.set_tick_params(labelsize=15)
    #myplot.yaxis.set_tick_params(labelsize=15)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append((sum(data.Hitrate[j][1])/168)*100)
        
        daylyAvg.append((sum(data.Hitrate[j][2])/168)*100)
        
        TotalHits['Hits'].append(sum(daylyAvg)/2)
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
a4_dims = (11.7, 8.27)
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 20})
sns.set(font_scale=1.7)
level=2
typ = "hitrate"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.3f', vmin=0)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')


data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append((sum(data.Hitrate[j][0])/168)*100)
        
        TotalHits['Hits'].append(sum(daylyAvg))
        TotalHits['Admission'].append(data.Admission[j])
        TotalHits['AdmissionParams'].append(data.AdmissionParams[j])
        TotalHits['Eviction'].append(data.Eviction[j])
        TotalHits['EvictionParams'].append(data.EvictionParams[j])
        TotalHits['Size'].append(data.Size[j])
    
sns.set_style("darkgrid")
df = pd.DataFrame(TotalHits)
df["EvictionP"] = df["Eviction"].map(str) + df["EvictionParams"]
df["AdmissionP"] = df["Admission"].map(str) + df["AdmissionParams"]
matplotlib.rcParams.update({'font.size': 22})
sns.set(font_scale=1.7)
level=3
typ = "hitrate"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=0)
    #myplot.xaxis.set_tick_params(labelsize=15)
    #myplot.yaxis.set_tick_params(labelsize=15)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.ReadOperations[j][3])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][4])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][5])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][6])/168)
        
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
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.ReadOperations[j][1])/168)
        
        daylyAvg.append(sum(data.ReadOperations[j][2])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg)/2)
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
level=2
typ = "Read"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=5.5, vmax=5.9)
    #myplot.xaxis.set_tick_params(labelsize=15)
    #myplot.yaxis.set_tick_params(labelsize=15)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.ReadOperations[j][0])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg))
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
level=3
typ = "Read"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=5.7, vmax=6.2)
    #myplot.xaxis.set_tick_params(labelsize=15)
    #myplot.yaxis.set_tick_params(labelsize=15)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.WriteOperations[j][3])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][4])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][5])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][6])/168)
        
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
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.WriteOperations[j][1])/168)
        
        daylyAvg.append(sum(data.WriteOperations[j][2])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg)/2)
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
level=2
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
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    
data = pd.read_pickle('data/Top1ResultsMean.pkl')
secs = mdate.epoch2num(data.Time[0])
TotalHits = {'Admission':[],'AdmissionParams':[],'Eviction':[],'EvictionParams':[],'Size':[],'Hits':[]}

for j in range(0,len(data.Hitrate)):
        daylyAvg = []
        daylyAvg.append(sum(data.WriteOperations[j][0])/168)
        
        TotalHits['Hits'].append(sum(daylyAvg))
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
level=3
typ = "Write"

for n in df.Size.unique():
    df2 = df[df['Size']==n]
    df2 = df2.pivot("EvictionP","AdmissionP","Hits")
    fig, ax = plt.subplots()
    ax.set_title("Size of the Network = " + n)
    myplot = sns.heatmap(df2, annot=True, cmap="YlGnBu", fmt='.2f', vmin=2, vmax=6.5)
    ax.set_xlabel("Admission Strategy", fontsize=20)
    ax.set_ylabel("Eviction Strategy", fontsize=20)
    figur = myplot.get_figure()
    figur.set_size_inches(10, 8)
    title="Heatmap_Top" + str(data.Topology[0][0]) + "_Level" + str(level)+"_"+typ+"_"+str(n)
    figur.savefig('figures/Top1/Heatmaps/'+title+'.png', bbox_inches='tight')
    