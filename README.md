The Car Rental System thus coded has three main elements :
1) Customers: Can view only "available" cars that can be rented,Can rent cars , pay dues for it if they exist 
2) Employees: All functionalities that customers can perform with an added perk that they alwats have a 15% concession over renting a car or paying its dues.
3) Managers: The administrative part of the rental system , have access to every customers' innformation , every employees' information , every cars" information.
 Can add , update , delete and view in each of the databases. They can view all the cars that exist in the system , ulike the customers who cant view the already rented cars .
4) Car
5) CSV Files
In this system I have used the CSV(Comma Separated Value) Files for database handling as it is a very easy to access , implemement and understand database management system .
The Four CSV Files used in thr making of this system are :

1) car_database:
   It has 6 columns :
 1.  Column 1: The Model of the Car in the System.
 2.  Cloumn 2: The Average Customer Record required to rent this Car( An integer value).
 3.  Column 3: The current condition , in terms of health of the car(ranging from 1 to 100).
 4.  Column 4: A string , either "false" or "true" which implies that if a car is rented the database reads true and if not , it reads false.
 5.  Column 5: A string which stores the date of return in the DDMMYYYY format , if the car is avilable to be rented(i.e the column 4 is false) it reads "00000000".
 6.  Column 6: A string which either stores "None ", when that correspondig car is not rented or the ID of the customer/employee who has rented that car.

2) customer_database:
  It has 4 columns :
1. Column 1: Name of the Customer
2. Column 2: ID of the Customer. ID of every customer is unique . No two customerrs or employees or employee-customer share the same ID.
3. Column 3: Password of the Customer
4. Customer Record : It keeps a track of how well a customer does at the car rental system ,calculated on the basis of how much damage has he dealt to a car he rented . Upon registration , every customer is granted a customer record of 15 ,and this either decreaes ,if he damages the car , or stays constant, if he makes no harm to the rented cars.

3) employee_database:
  It has 4 columns :
1. Column 1: Name of the Employee
2. Column 2: ID of the Employee. ID of every employee is unique . No two employees or customers or employee-customer share the same ID.
3. Column 3: Password of the Customer
4. Column 4: Employee Record : It keeps a track of how well a customer does at the car rental system ,calculated on the basis of how much damage has he dealt to a car he rented . Upon registration , every employee is granted a customer record of 15 ,and this either decreaes ,if he damages the rented cars , or stays constant, if he makes no harm to the rented cars.

 4) manager _database:
  It has 3 columns :
1. Column 1: Name of the Manager
2. Column 2: ID of the manager
3. Column 3: Password of the Manager


  A User Class is the most precedent class of this code as it has three public attributes:
   1)A Name
   2)An ID
   3)A Password

   
1.The Employees:
A Employee class has been created , which is a child class of the User Class, inherited via Public Inheritance.
Thus it is bound to have the three attributes:
1)A Name
2)An ID
3)A Password
The employee has an autonomy to perform 5 functions:

1)  displayAvailableCarsForEmployees(const Employee employee):
This function enables the employee to view at the possible cars tat can be reted , by taking an input of the employee Class Datatype . The main logic behind this function is whenver any car present in the "car_database.csv"file is available to rent i.e if the column 4 of the corresponding car is "false", it is displayed. If no cars are available to be rented , it displays no cars can be rented as of now.

2) rentCarForEmployee(const string& carModel, const string& inputDate):
   This function enables the employee to rent a car after cross checking if the name the employee has input is correct or not and if it is available to rent(true/false) or not . The date is taken as
   input because when the employee rents a car he/she is expected it to return at a certain duedate , the 6th column of the car_database. In my rental system that date is 10 days ahead of the inpuDate, or the date of renting the car. If the input name i.e model of car input is wrong or does not exist in the database , it displays no such car exists pr input car cannot be rented.

3)seeRentedCarsForEmployees(const Employee& employee):
Enables the employee to see which all cars has he rented , checks for the ID of the corresponding employee for every 6th column of every car,checks if it is rented(true,4th column) and if the ID coincides ,the corresponding cars with some other details is then displayed on the screen. If he doesnt have any rented car , the function outputs that he hasnt rented any cars as of now .

4)fineForRentedCarsforEmployee(const string& inputDate): 
By taking an input date , it helps the employee to show the existing fines , if any . The logic behind this is if the input date exceeds the duedate of cars owned by the owner , then the dues for that corresponding car is displayed. Dues are calculated as : daysOverdue*finePerDay
daysOverDue: (input date- due date) should be >0
finePerDay for customers is set as : 8.5 $ per day. This is because as mentioned earlier, every employee has a 15% concession on any monetary transactions .

5)learDuesForEmployee(const string& inputDate, int damage):
This function helps the corresponding employee to clear his/her existing dues , one car at a time, Ex: If a employee has more 2 cars whose dues are to paid , he would have to call this function twice to do so. Once due are cleared for a car , their availability is set to false(coulmn 4 ) , duedate is set to "00000000" (column 5)and the owner ID is changed to None(column 6).

2.The Customers:
A Customer class has been created , which is a child class of the User Class, inherited via Public Inheritance.
Thus it is bound to have the three attributes:
1)A Name
2)An ID
3)A Password
The customer has an autonomy to perform 5 functions:

1)  displayAvailableCarsForCustomers(const Customer customer):
This function enables the customer to view at the possible cars tat can be reted , by taking an input of the Customer Class Datatype . The main logic behind this function is whenver any car present in the "car_database.csv"file is available to rent i.e if the column 4 of the corresponding car is "false", it is displayed. If no cars are available to be rented , it displays no cars can be rented as of now.

2) rentCar(const string& carModel, const string& inputDate):
   This function enables the customer to rent a car after cross checking if the name the customer has input is correct or not and if it is available to rent(true/false) or not . The date is taken as
   input because when the customer rents a car he/she is expected it to return at a certain duedate , the 6th column of the car_database. In my rental system that date is 10 days ahead of the inpuDate, or the date of renting the car. If the input name i.e model of car input is wrong ad does not eexist in the database , it displays no such car exists pr input car cannot be rented.

3)seeRentedCars(const Customer customer):
Enables the customer to see which all cars has he rented , checks for the ID of the corresponding customer for every 6th column of every car,checks if it is rented(true,4th column) and if the ID coincides ,the corresponding cars with some other details is then displayed on the screen. If he doesnt have any rented car , the function outputs that he hasnt rented any cars as of now .

4)fineForRentedCars(const string& inputDate): 
By taking an input date , it helps the customer to show the existing fines , if any . The logic behind this is if the input date exceeds the duedate of cars owned by the owner , then the dues for that corresponding car is displayed. Dues are calculated as : daysOverdue*finePerDay
daysOverDue: (input date- due date) should be >0
finePerDay for customers is set as : 10 $ per day.

5)ClearDues(const string& inputDate, int damage):
This function helps the corresponding customer to clear his/her existing dues , one car at a time, Ex: If a customer has more 2 cars whose dues are to paid , he would have to call this function twice to do so. Once due are cleared for a car , their availability is set to false(coulmn 4 ) , duedate is set to "00000000" (column 5)and the owner ID is changed to None(column 6).



3.The Managers:
A Manager class has been created , which is a child class of the User Class, inherited via Public Inheritance.
Thus it is bound to have the three attributes:
1)A Name
2)An ID
3)A Password
 The Manager class has an autonomy of performing 17 functions :
 1.addCustomer(const string& name, const string& id, const string& password): Adds customers
 2.addEmployee(const string& name, const string& id, const string& password):Adds employees
 3.addCar(const string& modelName, const string& condition):Adds cars
 4.deleteCustomerById(const string& customerId): deletes a customer from the database and frees the customers' corresponding rented cars(if there exist any).
 5.deleteEmployeeById(const string& employeeId): deletes a customer from the database and frees the employees' corresponding rented cars(if there exist any).
 6.deleteCar(const string& carModel): deletes cars
 7.displayCarDatabase(const Manager& manager): displays the car_database
 8.displayCustomerDatabase(const Manager& manager): displays the customer_database
 9.updateCarModel(const string& oldModel, const string& newModel): updates the car model
 10:updateAvgCustomerRecord(const string& carModel, const string& avgCustomerRecord): updates average customer record for a given car
 11 updateCondition(const string& carModel, const string& condition): updates condition of a car
 12.updateCustomerPassword(const string& customerId, const string& newPassword): updates a customer password
 13.updateCustomerRecord(const string& customerId, const string& newCustomerRecord): updates a customer record
 14.updateCustomerName(const string& customerId, const string& newName): updates customer name
 15.updateEmployeePassword(const string& employeeId, const string& newPassword): updates employee password
 16.updateEmployeeRecord(const string& employeeId, const string& newRecord): update employee record
 17.updateEmployeeName(const string& employeeId, const string& newName): updates employee name

4. Auxiliary Functions :
There are some auxiliary functions such as :
1. exploreCustomer(Customer& C): Helps the customer to expore all the 5 functions he can perform and also logout when required.
2. exploreManager(Manager& manager): Helps the manager to expore all the 17 functions he can perform and also logout when required.
3. exploreEmployee(Employee& E): Helps the employee to expore all the 5 functions he can perform and also logout when required.
4. Login(char UserType): Helps the managers, customers and employees to login / regsister as and when required . Performed with efficient error handling and unique ID registration for employee-customers.
5. Welcome():This function nests the Login function. The login function thus nests all the explore functions . Hence for the system to operate only the Wlcome(0 function needs to be called once inside the main driver code.
