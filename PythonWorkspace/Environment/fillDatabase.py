import pandas as pd
import mysql.connector
import string
import random
import pandas as pd
import numpy as np

db = mysql.connector.connect(user='root', password='root', host='localhost',database='student')
cursor = db.cursor();


numberOfUniqueVideos = 20
numberOfUniqueClients = 40
simulationTime = 360
numberOfRequests = 80

CategoryList = ['Music','Entertainment','Pepole','Comedy','Games','Film','Howto','Sports','Autos','News','Education','Tech','Nonprofit','Travel','Animals','Shows','Trailers','Movies']
uniqueUserIdList = []
uniqueVideoIdList = []
videoCategoryTupleList = []



for i in range(0,numberOfUniqueClients):
    uniqueUserIdList.append(''.join(random.choice(string.digits) for _ in range(6)))

for i in range(0,numberOfUniqueVideos):
    uniqueVideoIdList.append(''.join(random.choice(string.ascii_letters + string.digits) for _ in range(10)))

Table = {'row_names':[],'requestTime':[],'userId':[],'videoId':[],
        'sessionDuration':[],'avgChunkDuration':[],'chunks':[],'duration':[],
        'uploaded':[],'uploader':[],'category':[],'bitrate':[],'index':[]}

for i in range(0,numberOfUniqueVideos):
    videoCategoryTupleList.append((uniqueUserIdList[i],CategoryList[random.randint(0,len(CategoryList)-1)]))

for i in range(1,numberOfRequests+1):
    vidCatTuple = videoCategoryTupleList[random.randint(0,len(videoCategoryTupleList)-1)]
    videoId = vidCatTuple[0]
    category = vidCatTuple[1]
    Table['row_names'].append(1)
    Table['requestTime'].append(1397426400.07611+(simulationTime/numberOfRequests)*(i-1))
    Table['userId'].append(uniqueUserIdList[random.randint(0,len(uniqueUserIdList)-1)])
    Table['videoId'].append(videoId)
    Table['sessionDuration'].append(random.randint(7,255))
    Table['avgChunkDuration'].append(1)
    Table['chunks'].append(1)
    Table['duration'].append(1)
    Table['uploaded'].append('null')
    Table['uploader'].append('null')
    Table['category'].append(category)
    Table['bitrate'].append(random.randint(7,255))
    Table['index'].append(i)

df = pd.DataFrame(Table)
for index, row in df.iterrows():
    try:
        executeStatement = """INSERT INTO `student`.`YouTubeDataset` 
            (`row_names`, `requestTime`, `userId`, `videoId`, 
            `sessionDuration`, `avgChunkDuration`, `chunks`, `duration`, 
            `uploaded`, `uploader`, `category`, `bitrate`, `index`) 
            VALUES ('"""+str(row['row_names'])+"""', '"""+str(row['requestTime'])+"""', '"""+str(row['userId'])+"""',
            '"""+str(row['videoId'])+"""', '"""+str(row['sessionDuration'])+"""', '"""+str(row['avgChunkDuration'])+"""',
            '"""+str(row['chunks'])+"""', '"""+str(row['duration'])+"""', '"""+str(row['uploaded'])+"""', '"""+str(row['uploader'])+"""', '"""+str(row['category'])+"""', '"""+str(row['bitrate'])+"""', '"""+str(row['index'])+"""');"""
        cursor.execute(executeStatement);
        db.commit();
    except:
        db.rollback();
db.close();
