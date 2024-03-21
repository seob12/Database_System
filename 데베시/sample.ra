\sqlexec_{select lastname, firstname, jobtitle from employees};

\sqlexec_{select customername from customers where country = 'USA' and city = 'NYC' and creditlimit < 200};

\sqlexec_{select productcode, productname, buyprice from products where buyprice <= 50 or buyprice >= 100};

\sqlexec_{select employeenumber, lastname, firstname from employees where lastname like '%am%' and firstname like '%er'};

\sqlexec_{select * from customers order by contactlastname desc, contactfirstname asc};

\sqlexec_{select productname, buyprice from products order by buyprice desc limit 10};

\sqlexec_{select productname, priceeach from products order by priceeach asc limit 1};

\sqlexec_{select city from customers where amount >= 40000;
SELECT DISTINCT c.city
FROM customers c
JOIN payments p ON c.customerNumber = p.customerNumber
WHERE p.amount >= 40000};

\sqlexec_{SELECT customerNumber AS number, contactlastname AS lastname
FROM customers
UNION
SELECT employeeNumber AS number, lastname
FROM employees
ORDER BY number ASC};

\sqlexec_{SELECT status, count(*) AS order_count
FROM orders
GROUP BY status order by order_count desc};

\sqlexec_{SELECT 
    orderNumber,
    COUNT(*) AS totalOrders,
    SUM(quantityOrdered * priceEach) AS totalAmount
FROM 
    orderdetails
GROUP BY 
    orderNumber
HAVING 
    totalOrders >= 1000 AND totalAmount >= 600};

\sqlexec_{select p.productname, o.priceeach
from products p
join orderdetails o on p.productcode = o.productcode
where o.priceeach = (
	select min(priceeach)
	from orderdetails
    )};
    
\sqlexec_{select o.ordernumber, o.status, sum(d.quantityordered * d.priceeach) as totalmoney
from orders o
join orderdetails d on o.ordernumber = d.ordernumber
group by o.ordernumber, o.status
};

\sqlexec_{select c.customernumber, c.customername, o.ordernumber, o.status from customers c
inner join orders o on c.customernumber = o.customernumber
};

\sqlexec_{select c.customernumber, c.customername from customers c
left join orders o on c.customernumber = o.customernumber 
where o.customernumber is null
order by o.customernumber asc
};

\sqlexec_{create table merit(
    performance int(11) not null,
    percentage float not null,
    primary key(performance)
)};

\sqlexec_{CREATE TABLE employees (
    emp_id INTEGER PRIMARY KEY AUTOINCREMENT,
    emp_name VARCHAR(255) NOT NULL,
    performance INTEGER,
    salary FLOAT,
    FOREIGN KEY (performance) REFERENCES merits(performance)
)};


\sqlexec_{insert into merit(performance, percentage) values
(1, 0), (2, 0.01), (3, 0.03), (4, 0.05), (5, 0.08)
};

\sqlexec_{insert into employees(emp_name, performance, salary) values
('Mary Doe', 1, 50000),
('Cindy Smith', 3, 65000),
('Sue Greenspan', 4, 75000),
('Grace Dell', 5, 125000),
('Nancy Johnson', 3, 85000),
('John Doe', 2, 45000), 
('Lily Bush', 3, 55000)
};

\sqlexec_{CREATE TABLE buildings (
    building_no INTEGER PRIMARY KEY AUTOINCREMENT,
    building_name TEXT NOT NULL,
    address TEXT NOT NULL
)};

\sqlexec_{INSERT INTO buildings(building_name, address) 
VALUES('ACME Headquaters', '3950 North 1st Street CA 95134'),
('ACME Sales', '5000 North 1st Street CA 95134')
};

\sqlexec_{CREATE TABLE rooms (
    room_no INTEGER PRIMARY KEY AUTOINCREMENT,
    room_name TEXT NOT NULL,
    building_no INTEGER NOT NULL,
    FOREIGN KEY (building_no) REFERENCES buildings (building_no) ON DELETE CASCADE
)};

\sqlexec_{INSERT INTO rooms(room_name,building_no) VALUES('Amazon',1), ('War Room',1), 
('Office of CEO',1), ('Marketing', 2), ('Showroom',2)
};

\sqlexec_{INSERT INTO employees (employeeNumber, lastName, firstName, extension, email, officeCode, reportsTo, jobTitle)
VALUES (1901, 'hong', 'jin', 'x5000', 'nike@google.com', 10, NULL, 'Research')};

\sqlexec_{update offices
set city = 'jeonju'
where officecode in (select officecode from employees where Jobtitle = 'Research')};

\sqlexec_{UPDATE employees
SET salary = salary + (salary * (SELECT merit.percentage FROM merit WHERE employees.performance = merit.performance))
WHERE EXISTS (
    SELECT 1
    FROM merit
    WHERE employees.performance = merit.performance
)};

\sqlexec_{delete from employees};

\sqlexec_{delete from rooms where building_no = 2};