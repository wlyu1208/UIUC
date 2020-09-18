CREATE TABLE user (user_id varchar(10) NOT NULL, password VARCHAR(16) NOT NULL, name VARCHAR(40), drink_name VARCHAR(30), gender VARCHAR(1), height FLOAT, weight FLOAT, age INT, is_pregnant BOOL, PRIMARY KEY (user_id);
CREATE TABLE caffeine ( drink_name varchar(30), fl_oz float, mg_caffeine float, mg_floz float);
CREATE TABLE sugar ( drink_name varchar(30), fl_oz float, g_sugar float, g_floz float);

LOAD DATA LOCAL INFILE '/Users/tommycarver/Downloads/caff.csv' INTO TABLE caffeine FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';
LOAD DATA LOCAL INFILE '/Users/tommycarver/Downloads/suge.csv' INTO TABLE sugar FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';

DELETE FROM sugar where drink_name = 'Drink';
DELETE FROM caffeine where drink_name = 'Drink';

DELETE FROM caffeine where drink_name NOT IN (SELECT drink_name FROM sugar);
DELETE FROM sugar where drink_name NOT IN (SELECT drink_name FROM caffeine);

CREATE TABLE Drink AS (SELECT caffeine.drink_name, caffeine.fl_oz, mg_caffeine, mg_floz, g_sugar, g_floz FROM caffeine JOIN sugar ON sugar.drink_name = caffeine.drink_name);
ALTER TABLE Drink ADD id MEDIUMINT NOT NULL AUTO_INCREMENT KEY;

ALTER TABLE sugar ADD id MEDIUMINT NOT NULL AUTO_INCREMENT KEY;
ALTER TABLE caffeine ADD id MEDIUMINT NOT NULL AUTO_INCREMENT KEY;