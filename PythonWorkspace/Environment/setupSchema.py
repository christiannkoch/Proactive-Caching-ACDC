import pandas as pd
import pymysql
db = pymysql.connect(user='root', password='start123', host='localhost')
cursor = db.cursor();
try:
    cursor.execute("""CREATE SCHEMA `ACDCTestData`;""");
    db.commit();
except:
    db.rollback();
db.close();

db = pymysql.connect(user='root', password='start123', host='localhost',database='ACDCTestData')
cursor = db.cursor();

try:
    cursor.execute("""CREATE TABLE `ACDCTestData`.`YouTubeDataset` (
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
