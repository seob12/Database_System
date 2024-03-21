
select lastname, firstname, jobtitle from employees;
select customername from customers where country = 'USA' and city = 'NYC' and creditlimit < 200;
select productcode, productname, buyprice from products where buyprice <= 50 or buyprice >= 100;
select employeenumber, lastname, firstname from employees where lastname like '%am%' and firstname like '%er';
select * from customers order by contactlastname desc, contactfirstname asc;
select productname, buyprice from products order by buyprice desc limit 10;
select productname, priceeach from products order by priceeach asc limit 1;
select city from customers where amount >= 40000;
SELECT DISTINCT c.city FROM customers c JOIN payments p ON c.customerNumber = p.customerNumber WHERE p.amount >= 40000;

SELECT customerNumber AS number, contactlastname AS lastname FROM customers UNION SELECT employeeNumber AS number, lastname FROM employees ORDER BY number ASC;

SELECT status, count(*) AS order_count FROM orders GROUP BY status order by order_count desc;

SELECT orderNumber,COUNT(*) AS totalOrders,SUM(quantityOrdered * priceEach) AS totalAmount FROM orderdetails GROUP BY orderNumber HAVING totalOrders >= 1000 AND totalAmount >= 600;

select p.productname, o.priceeach from products p join orderdetails o on p.productcode = o.productcode where o.priceeach = (select min(priceeach) from orderdetails);
    
select o.ordernumber, o.status, sum(d.quantityordered * d.priceeach) as totalmoney from orders o join orderdetails d on o.ordernumber = d.ordernumber group by o.ordernumber, o.status;

select c.customernumber, c.customername, o.ordernumber, o.status from customers c inner join orders o on c.customernumber = o.customernumber;

select c.customernumber, c.customername from customers c left join orders o on c.customernumber = o.customernumber where o.customernumber is null order by o.customernumber asc;


