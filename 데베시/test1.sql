create database empdb;
use empdb;

create table merit(
    performance int(11) not null,
    percentage float not null,
    primary key(performance)
);

create table employees(
    emp_id int(11) not null auto_increment,
    emp_name varchar(255) not null,
    performance int(11) default null,
    salary float default null,
    primary key (emp_id) constraint fk_performance foreign key (performance) references merits(performance)
);

insert into merit(performance, percentage) values
(1, 0), (2, 0.01), (3, 0.03), (4, 0.05), (5, 0.08);

insert into employees(emp_name, performance, salary) values
('Mary Doe', 1, 50000),
('Cindy Smith', 3, 65000),
('Sue Greenspan', 4, 75000),
('Grace Dell', 5, 125000),
('Nancy Johnson', 3, 85000),
('John Doe', 2, 45000), 
('Lily Bush', 3, 55000);

create table buildings(
    building_no int(11) not null auto_increment,
    building_name varchar(255) not null,
    address varchar(355) not null,
    primary key (building_no)
);

INSERT INTO buildings(building_name, address) 
VALUES('ACME Headquaters', '3950 North 1st Street CA 95134'),
('ACME Sales', '5000 North 1st Street CA 95134');

CREATE TABLE rooms(
room_no INT(11) NOT NULL AUTO_INCREMENT, 
room_name VARCHAR(255) NOT NULL, 
building_no INT(11) NOT NULL,
PRIMARY KEY (room_no),
KEY building_no(building_no), 
CONSTRAINT rooms_ibfk_1 
FOREIGN KEY(building_no) 
REFERENCES buildings (building_no) 
ON DELETE CASCADE);

INSERT INTO rooms(room_name,building_no) VALUES('Amazon',1), ('War Room',1), 
('Office of CEO',1), ('Marketing', 2), ('Showroom',2);