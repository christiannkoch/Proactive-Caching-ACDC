import pandas as pd
import mysql.connector
db = mysql.connector.connect(user='root', password='root', host='localhost')
cursor = db.cursor();
try:
    cursor.execute("""CREATE SCHEMA `student`;""");
    db.commit();
except:
    db.rollback();
db.close();

db = mysql.connector.connect(user='root', password='root', host='localhost',database='student')
cursor = db.cursor();

try:
    cursor.execute("""CREATE TABLE `student`.`YouTubeDataset` (
    `row_names` TEXT NULL,
    `requestTime` DOUBLE NULL,
    `userId` BIGINT(20) NULL,
    `videoId` TEXT NULL,
    `sessionDuration` DOUBLE NULL,
    `avgChunkDuration` DOUBLE NULL,
    `chunks` BIGINT(20) NULL,
    `duration` TEXT NULL,
    `uploaded` TEXT NULL,
    `uploader` TEXT NULL,
    `category` TEXT NULL,
    `bitrate` BIGINT(20) NULL,
    `index` INT(11) NOT NULL AUTO_INCREMENT,
    PRIMARY KEY (`index`));""");
    db.commit();
except:
    db.rollback();
db.close();
