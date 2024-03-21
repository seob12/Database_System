select lastname, firstname, jobtitle from employees;

select customername from customers where country = 'USA' and city = 'NYC' and creditlimit < 200;

select productcode, productname, buyprice from products where buyprice <= 50 or buyprice >= 100;

select employeenumber, lastname, firstname from employees where lastname like '%am%' and firstname like '%er';

select * from customers order by contactlastname desc, contactfirstname asc;

select productname, buyprice from products order by buyprice desc limit 10;

select productname, priceeach from products order by priceeach asc limit 1;

select city from customers where amount >= 40000;
SELECT DISTINCT c.city
FROM customers c
JOIN payments p ON c.customerNumber = p.customerNumber
WHERE p.amount >= 40000;

SELECT customerNumber AS number, contactlastname AS lastname
FROM customers
UNION
SELECT employeeNumber AS number, lastname
FROM employees
ORDER BY number ASC;

SELECT status, count(*) AS order_count
FROM orders
GROUP BY status order by order_count desc;

SELECT 
    orderNumber,
    COUNT(*) AS totalOrders,
    SUM(quantityOrdered * priceEach) AS totalAmount
FROM 
    orderdetails
GROUP BY 
    orderNumber
HAVING 
    totalOrders >= 1000 AND totalAmount >= 600;

select p.productname, o.priceeach
from products p
join orderdetails o on p.productcode = o.productcode
where o.priceeach = (
	select min(priceeach)
	from orderdetails
    );
    
select o.ordernumber, o.status, sum(d.quantityordered * d.priceeach) as totalmoney
from orders o
join orderdetails d on o.ordernumber = d.ordernumber
group by o.ordernumber, o.status;

select c.customernumber, c.customername, o.ordernumber, o.status from customers c
inner join orders o on c.customernumber = o.customernumber;

select c.customernumber, c.customername from customers c
left join orders o on c.customernumber = o.customernumber 
where o.customernumber is null
order by o.customernumber asc;

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
    primary key (emp_id) constraint,
    fk_performance foreign key (performance) references merits(performance)
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

use classicmodels;

insert into employees(employeenumber, lastname, firstname, extension, email, officecode,
reportsto, jobtitle) values 
(1901, 'hong', 'jin', 'x5000', 'nike@google.com', 10, null, 'Research');

update offices
set city = 'jeonju'
where officecode in (select officecode from employees where Jobtitle = 'Research');

use empdb;

update employees inner join merit on employees.performance = merit.performance
set salary = salary + (salary * merit.percentage);

use classicmodels;

alter table employees
drop foreign key employees_ibfk_2;

alter table employees
add constraint employees_ibfk_2
foreign key (officecode) references offices(officecode) on delete cascade;

set sql_safe_updates = 0;
delete from employees where officecode = 1;

use empdb;

delete from employees;

delete from rooms where building_no = 2;