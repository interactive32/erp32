
## WARNING: THIS PROJECT IS NO LONGER MAINTAINED!

Erp32 - Web-based enterprise resource planning business management php software.

#ERP32
![Erp32](http://interactive32.com/resources/erp32_github.jpg)

# Usage with Docker and docker-compose
Clone:
```
git clone git@github.com:interactive32/erp32.git
```

Start with Docker:
```
docker compose up -d
```

Fix permissions
```
docker exec -i erp32-backend chown -R www-data:www-data /var/www/html
```
Prepare database
```
docker exec -i erp32-db mysql -uroot -pmypass erp32_database < erp32/database/blank_database.sql
```
or import demo database
```
docker exec -i erp32-db mysql -uroot -pmypass erp32_database < erp32/database/demo_database.sql
```

Use:
```
http://localhost:9000
```

Available users:
```
administrator/admin123
user1/user1
user2/user2
```


#CONTENTS:


1. Before you begin
2. Short description
3. Installation
4. Customization
5. Users Manual


#1. Before you begin:

Consult your accountant about this Software and all features before use.

Our Warranties, Liability and Disclaimers

NEITHER INTERACTIVE32 NOR ITS SUPPLIERS OR DISTRIBUTORS MAKE ANY SPECIFIC PROMISES ABOUT THIS SOFTWARE. FOR EXAMPLE, WE DON’T MAKE ANY COMMITMENTS ABOUT DATA AND CONTENT WITHIN THE SOFTWARE, THE SPECIFIC FUNCTION OF THE SERVICES, OR THEIR RELIABILITY, AVAILABILITY, OR ABILITY TO MEET YOUR NEEDS. WE PROVIDE THE SOFTWARE “AS IS”.

INTERACTIVE32 WILL NOT BE RESPONSIBLE FOR LOST PROFITS, REVENUES, OR DATA, LOSS OF GOODWILL OR BUSINESS REPUTATION, FINANCIAL LOSSES OR INDIRECT, SPECIAL, CONSEQUENTIAL, OR EXEMPLARY DAMAGES IN ALL CASES.



#2. Short description

ERP32 is a web-based business management software for small and medium businesses. It allows central management of business processes in your company. 
 


#3a. Installation (old)

Follow these steps to install and configure erp32:

- make sure your have minimum PHP version 5.2 + Apache2
- copy all files to your server (via FTP or similar)
- web server (Apache) must have read/write permissions to all files inside erp32 folder
- import blank_database.sql (or demo_database.sql) from database folder into your mysql server (use phpMyAdmin or similar tool)
- update config.php to reflect your database settings
- you can now log in with initial username and password combination: user1/user1
- default administrator username/password is administrator/admin123. Please change this password inside config.php file



#4. Customization

Only administrator can create and manage users. Administrtor can also add new security groups and change users permissions.

You can customize banner and home-page image inside erp32/banner/ folder.

Language files are located here: erp32/app/atk/languages/ you can add new languages or update current files. 
If you add new language make sure your configure it inside admin panel.


#5. Users Manual

Users manual is located under users_manual folder and it is also accessible through erp32 main menu



## License

[MIT](http://opensource.org/licenses/MIT)

Copyright (c) 2013-present, interactive32
