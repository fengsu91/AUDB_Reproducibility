urange (select name, city, count(*) as ct from real_customers_radb is radb group by name,city);