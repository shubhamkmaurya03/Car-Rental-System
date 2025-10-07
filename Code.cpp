
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm> 
#include <iostream>
#include <vector>
#include <iomanip> 
#include<regex>
using namespace std;


bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to get the number of days in a month
int daysInMonth(int month, int year) {
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

string add10Days(const string& date) {
    // Extract day, month, and year from the input date string
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(2, 2));
    int year = stoi(date.substr(4, 4));

    // Increment the day by 7 days
    day += 10;

    // Check if the day exceeds the number of days in the month
    while (day > daysInMonth(month, year)) {
        // Subtract the number of days in the current month
        day -= daysInMonth(month, year);

        // Move to the next month
        ++month;

        // If the month exceeds 12, move to the next year
        if (month > 12) {
            month = 1;
            ++year;
        }
    }

    // Format the resulting date string
    stringstream result;
    result << setfill('0') << setw(2) << day;
    result << setfill('0') << setw(2) << month;
    result << setfill('0') << setw(4) << year;

    return result.str();
}

int daysBetweenDates(const string& date1, const string& date2) {
    struct tm timeinfo1 = {0};  // Initialize struct to zero
    struct tm timeinfo2 = {0};  // Initialize struct to zero
    time_t rawtime1, rawtime2;

    // Convert date strings to tm structures manually
    if (date1.size() != 8 || date2.size() != 8) {
        cerr << "Invalid date format." << endl;
        return -1;  // Indicate error
    }

    // Convert date1
    timeinfo1.tm_year = stoi(date1.substr(4, 4)) - 1900;
    timeinfo1.tm_mon = stoi(date1.substr(2, 2)) - 1;
    timeinfo1.tm_mday = stoi(date1.substr(0, 2));
    // Set tm_isdst to -1 to let mktime determine daylight saving time automatically
    timeinfo1.tm_isdst = -1;

    // Convert date2
    timeinfo2.tm_year = stoi(date2.substr(4, 4)) - 1900;
    timeinfo2.tm_mon = stoi(date2.substr(2, 2)) - 1;
    timeinfo2.tm_mday = stoi(date2.substr(0, 2));
    // Set tm_isdst to -1 to let mktime determine daylight saving time automatically
    timeinfo2.tm_isdst = -1;

    // Convert tm structures to time_t
    rawtime1 = mktime(&timeinfo1);
    rawtime2 = mktime(&timeinfo2);

    // Calculate the difference in seconds
    double difference = difftime(rawtime2, rawtime1);

    // Convert seconds to days
    int days = static_cast<int>(difference / (24 * 60 * 60));

    return days;
}
  
bool isValidDateFormat(const std::string& date) {
    // Regular expression to match DDMMYYYY format
    std::regex pattern(R"(\b(0[1-9]|[12][0-9]|3[01])(0[1-9]|1[0-2])(19|20)\d\d\b)");

    // Check if the input matches the pattern
    if (!std::regex_match(date, pattern)) {
        return false; // If format doesn't match, return false
    }

    // Extract day, month, and year from the input string
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(2, 2));
    int year = std::stoi(date.substr(4, 4));

    // Check if the month is valid (1-12)
    if (month < 1 || month > 12) {
        return false;
    }

    // Check if the day is valid for the given month
    if ((day < 1) || (day > 31)) {
        return false;
    }

    // Adjust February days for leap years
    bool isLeapYear = (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
    if (month == 2) {
        if (isLeapYear) {
            return day <= 29;
        } else {
            return day <= 28;
        }
    }

    // Check if the day is valid for the given month (considering months with 30 days)
    if ((month == 4 || month == 6 || month == 9 || month == 11) && (day > 30)) {
        return false;
    }

    return true; // If all checks passed, return true
}
void insertIntoCSV(const string& filename, const string& name, const string& id, const string& password) {
    // Open the CSV file in append mode
    ofstream file;
    file.open(filename, ios::app);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Write data to the CSV file
    printf("\n");
    file << name << "," << id << "," << password << "\n";

    // Close the file
    file.close();
}

void insertIntoCSVforEmployeesAndCustomers(const string& filename, const string& name, const string& id, const string& password, int customer_record) {
    // Open the CSV file in append mode
    ofstream file;
    file.open(filename, ios::app);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Write data to the CSV file
    file << name << "," << id << "," << password << "," << customer_record << "\n";

    // Close the file
    file.close();
} 


bool userExistsInCSV(const string& filename, const string& name, const string& id, const string& password) {
    // Open the CSV file
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return false;
    }

    // Read each line from the file
    string line;
    while (getline(file, line)) {
        // Use stringstream to split the line into tokens
        stringstream ss(line);
        string token;

        // Read tokens separated by commas
        getline(ss, token, ','); // Name
        string storedName = token;
        getline(ss, token, ','); // ID
        string storedID = token;
        getline(ss, token, ','); // Password
        string storedPassword = token;

        // Check if both ID and Name match
        if (storedName == name && storedID == id && storedPassword == password) {
            file.close();
            return true; // User already exists
        }
    }

    file.close();
    return false; // User does not exist
}

bool isIDTaken(const string& newID) {
    ifstream customerFile("customer_database.csv");
    ifstream employeeFile("employee_database.csv");

    if (!customerFile.is_open() || !employeeFile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return false;  // Assume ID is not taken in case of an error
    }

    // Check in customer database
    string line;
    while (getline(customerFile, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        if (row.size() > 0 && row[1] == newID) {  // Assuming ID is in the second column
            customerFile.close();
            return true;  // ID is already taken
        }
    }

    // Check in employee database
    while (getline(employeeFile, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        if (row.size() > 0 && row[1] == newID) {  // Assuming ID is in the second column
            employeeFile.close();
            return true;  // ID is already taken
        }
    }

    customerFile.close();
    employeeFile.close();
    return false;  // ID is not taken
}

class Car {
public:
    string Model;
    string Condition;
    bool IsAvailable;
    int DateofAllocation;
    int min_customer_record;
    int Rate;
    //name of owner
    //duedate
    //health
};
class User {
public :
    string Name;
    string ID;
    string Password;

};
class Customer : public User {
public:
int customer_record;
     void rentCar(const string& carModel, const string& inputDate);
     void fineForRentedCars(const string& inputDate);
     void ClearDues(const string& inputDate, int damage);
};


void displayAvailableCarsForCustomers(const Customer customer) {
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    bool carsAvailable = false;  // Flag to check if any cars are available

    for (int i = 0; i < data.size(); i++) {
        if (data[i][3] == "false") {
            carsAvailable = true;  // Set the flag to true if at least one car is available
            cout << "Car Model: " << data[i][0] << endl;
            cout << "Avg Customer Record Required: " << data[i][1] << endl;
            cout << "health: " << data[i][2] << endl;
            cout << "----------------------------------------" << endl;
            // Add other relevant information to display
        }
    }

    if (!carsAvailable) {
        cout << "No cars are available to be rented as of now." << endl;
        cout<<endl;
    }
}

void Customer::rentCar(const string& carModel, const string& inputDate) {
    ifstream carFile("car_database.csv");
    ifstream customerFile("customer_database.csv");

    if (!carFile.is_open() || !customerFile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    // Read car data from the file into a vector of vectors
    vector<vector<string>> carData;
    string carLine;

    while (getline(carFile, carLine)) {
        stringstream ss(carLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    carFile.close();

    // Read customer data from the file into a vector of vectors
    vector<vector<string>> customerData;
    string customerLine;

    while (getline(customerFile, customerLine)) {
        stringstream ss(customerLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }

    customerFile.close();

    // Find the car with the specified model
    for (int i = 0; i < carData.size(); i++) {
        if (carData[i][0] == carModel) {  // Assuming car model is the first column
            // Check if the car is available for rent
            if (carData[i][3] == "false") {  // Assuming availability status is the fourth column
                // Find the customer with the specified ID
                string customerId =this->ID;  // Assuming customer ID is the second column
                int customerRecord = 0;

                // Find the customer record in the customer database
                for (int j = 0; j < customerData.size(); j++) {
                    if (customerData[j][1] == customerId) {  // Assuming customer ID is the second column
                        customerRecord = stoi(customerData[j][3]);  // Assuming customer record is the fourth column
                        break;
                    }
                }

                // Find the average customer record for the car
                int avgCustomerRecord = stoi(carData[i][1]);  // Assuming average customer record is the second column

                // Check if the customer record is greater than or equal to the average
                if (customerRecord >= avgCustomerRecord) {
                    // Increment the input date by 10 days
                    string rentedDate = add10Days(inputDate);

                    // Update the CSV file with the new customer Name and date
                    carData[i][5] = this->ID;  // Using the "this" keyword to access class member variable
                    carData[i][3] = "true";
                    carData[i][4] = rentedDate;

                    // Notify successful rental
                    cout << "Car rented successfully by " << this->Name << " on " << inputDate << "." << endl;

                    // Write modified car data back to the file
                    ofstream carOutfile("car_database.csv");
                    if (!carOutfile.is_open()) {
                        cerr << "Error: Unable to open file." << endl;
                        return;
                    }
                    for (int k = 0; k < carData.size(); k++) {
                        for (int l = 0; l < carData[k].size(); l++) {
                            carOutfile << carData[k][l];
                            if (l != carData[k].size() - 1) carOutfile << ",";
                        }
                        if (k != carData.size() - 1) carOutfile << endl;
                    }
                    carOutfile.close();
                    return;  // Return after updating the first matching car (assuming unique models)
                } else {
                    cout << "Customer record is not sufficient to rent this car." << endl;
                    return;  // Return if the customer record is not sufficient
                }
            } else {
                cout << "Car is not available for rent." << endl;
                return;  // Return if the car is not available
            }
        }
    }

    // If the loop completes without finding the specified car model
    cout << "This Car Model cannot be rented or does not exist." << endl;
}



void seeRentedCars(const Customer customer) {
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    cout << "Cars Rented by " << customer.Name << ":" << endl;

    bool hasRentedCars = false;  // Flag to check if the customer has rented cars

    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == customer.ID && data[i][3] == "true") {
            hasRentedCars = true;  // Set the flag to true if the customer has rented cars
            cout << "Car Model: " << data[i][0] << endl;
            cout << "Avg Customer Record Required: " << data[i][1] << endl;
            cout << "health: " << data[i][2] << endl;
            cout << "Date of Return: " << data[i][4] << endl;
            cout << "----------------------------------------" << endl;
            // Add other relevant information to display
        }
    }

    if (!hasRentedCars) {
        cout << "You have no rented cars as of now." << endl;
    }
}

void Customer::fineForRentedCars(const string& inputDate) {
    double finePerDay=10;
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    // Read data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    // Check if there are any cars rented by the customer
    bool customerHasRentedCars = false;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == this->ID && data[i][3] == "true") {  // Assuming customer name is the fifth column
            customerHasRentedCars = true;
            break;
        }
    }

    if (!customerHasRentedCars) {
        cout << "Customer has no rented cars." << endl;
        return;
    }

    // Check for rented cars by the customer and impose fines
    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == this->ID && data[i][3] == "true") {  // Assuming customer name is the fifth column
            string databaseDate = data[i][4];  // Assuming date is the fourth column

            // Calculate the difference in days using the daysBetweenDates function
            int daysOverdue = daysBetweenDates(databaseDate,inputDate);

            // Check if the input date is greater than the database date
            if (daysOverdue > 0) {
                // Calculate fine
               double fine = daysOverdue * finePerDay;

                // Print fine along with the car model
                cout << "Fine for " << data[i][0] << ": $" << fine << endl;

                // Update data in memory (not in the file yet)
            // Set availability status to false
                
//no else statements for future delivery date;

            }
            else{
                cout<<"No existing Fines as of Now for " <<data[i][0] <<endl;
            }
        }
    }

    // Write modified data back to the file
    ofstream outfile("car_database.csv");
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }
    for (int j = 0; j < data.size(); j++) {
        for (int k = 0; k < data[j].size(); k++) {
            outfile << data[j][k];
            if (k != data[j].size() - 1) outfile << ",";
        }
        if (j != data.size() - 1) outfile << endl;
    }
    outfile.close();
}


void Customer::ClearDues(const string& inputDate, int damage) {
    ifstream carFile("car_database.csv");
    ifstream customerFile("customer_database.csv");

    if (!carFile.is_open() || !customerFile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string carLine;
    // Read car data from the file into a vector of vectors
    while (getline(carFile, carLine)) {
        stringstream ss(carLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }
    carFile.close();

    vector<vector<string>> customerData;
    string customerLine;
    // Read customer data from the file into a vector of vectors
    while (getline(customerFile, customerLine)) {
        stringstream ss(customerLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }
    customerFile.close();

    // Check if the customer has rented cars
    bool customerHasRentedCars = false;
    int rentedCarIndex = -1; 
    int k=0; // Index to store the position of the rented car
    for (int i = 0; i < carData.size(); i++) {
        string databaseDate = carData[i][4];  // Assuming date is the fifth column
    // Calculate the difference in days
    int daysOverdue = daysBetweenDates(databaseDate, inputDate);

        if (carData[i][5] == this->ID && carData[i][3] == "true"&&daysOverdue>0) {  // Assuming customer ID is the sixth column
            customerHasRentedCars = true;
            rentedCarIndex = i; 
             k=daysOverdue; // Save the index of the rented car
            break;  // Break out of the loop after finding the first rented car
        }
    }

    if (!customerHasRentedCars || rentedCarIndex == -1) {
        cout << "Customer has no rented cars." << endl;
        return;
    }

    // Calculate dues for the rented car
    // Check if there are existing dues
    if (k> 0) {
        // Calculate fine
        double finePerDay = 10;  // Assumed fine per day
        double fine = k * finePerDay;

        // Update car data
        carData[rentedCarIndex][4] = "00000000";  // Reset date to "00000000"
        carData[rentedCarIndex][2] = to_string(damage);  // Change the 3rd column to damage value
        carData[rentedCarIndex][5] = "None";
        carData[rentedCarIndex][3] = "false";

        // Print dues cleared information
        cout << "Dues Cleared for Car Model " << carData[rentedCarIndex][0] << " on " << inputDate << " by Customer " << this->Name << "." << endl;
        cout << "Amount of Dues: $" << fine << endl;

        // Write modified car data back to the file
        ofstream carOutfile("car_database.csv");
        if (!carOutfile.is_open()) {
            cerr << "Error: Unable to open car file." << endl;
            return;
        }
        for (int j = 0; j < carData.size(); j++) {
            for (int k = 0; k < carData[j].size(); k++) {
                carOutfile << carData[j][k];
                if (k != carData[j].size() - 1) carOutfile << ",";
            }
            if (j != carData.size() - 1) carOutfile << endl;
        }
        carOutfile.close();

        // Update customer health in the customer database
        for (int l = 0; l < customerData.size(); l++) {
            if (customerData[l][1] == this->ID) {  // Assuming customer ID is the first column
                double currentHealth = stod(customerData[l][3]);  // Assuming health is the fourth column
                double newHealth = currentHealth * (1.0 - ((1.0-((static_cast<double>(damage) / 100)))*(1.0-((static_cast<double>(damage) / 100)))));

                // Ensure the new health value is within [0, 1]
                newHealth = max(0.0, newHealth);

                // Update the 4th column with the new health value
                customerData[l][3] = to_string(newHealth);

                // Write modified customer data back to the file
                ofstream customerOutfile("customer_database.csv");
                if (!customerOutfile.is_open()) {
                    cerr << "Error: Unable to open customer file." << endl;
                    return;
                }
                for (int m = 0; m < customerData.size(); m++) {
                    for (int n = 0; n < customerData[m].size(); n++) {
                        customerOutfile << customerData[m][n];
                        if (n != customerData[m].size() - 1) customerOutfile << ",";
                    }
                    if (m != customerData.size() - 1) customerOutfile << endl;
                }
                customerOutfile.close();
                return;
            }
        }

        // If the loop completes without finding the specified customer ID in the customer database
        cout << "Customer not found in the customer database." << endl;
        return;
    } else {
        cout << "No existing dues as of now." << endl;
    }
}


class  Manager : public User {
public:
 void addCustomer(const string& name, const string& id, const string& password);
 void addEmployee(const string& name, const string& id, const string& password);
 void addCar(const string& modelName, const string& condition);
 void deleteCustomerById(const string& customerId);
 void deleteCar(const string& carModel);
 void deleteEmployeeById(const string& employeeId);
 void updateCarModel(const string& oldModel, const string& newModel);
 void updateAvgCustomerRecord(const string& carModel, const string& avgCustomerRecord);
 void updateCondition(const string& carModel, const string& condition);
 void updateCustomerPassword(const string& customerId, const string& newPassword);
 void updateCustomerRecord(const string& customerId, const string& newCustomerRecord);
 void updateCustomerName(const string& customerId, const string& newName);
 void updateEmployeePassword(const string& employeeId, const string& newPassword);   void updateEmployeeRecord(const string& employeeId, const string& newRecord);   void updateEmployeeName(const string& employeeId, const string& newName);
};

void Manager::addCustomer(const string& name, const string& id, const string& password) {
    // Open the customer database file in append mode
    ofstream file;
    file.open("customer_database.csv", ios::app);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Write data to the CSV file
    file << name << "," << id << "," << password << "," << 15 << "\n"; // Assuming 10 as the initial customer record

    // Close the file
    file.close();

    cout << "Customer added successfully." << endl;
}

void Manager::addEmployee(const string& name, const string& id, const string& password) {
    // Open the employee database file in append mode
    ofstream file;
    file.open("employee_database.csv", ios::app);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Write data to the CSV file
    file << name << "," << id << "," << password << "," << 15 << "\n"; // Assuming 10 as the initial employee record

    // Close the file
    file.close();

    cout << "Employee added successfully." << endl;
}

void Manager::addCar(const string& modelName, const string& condition) {
    // Open the car database file in append mode
    ofstream file;
    file.open("car_database.csv", ios::app);

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Write data to the CSV file
    file << modelName << "," << 10 << "," << condition << "," << false << "," << "00000000" << "," << "None" << "\n";

    // Close the file
    file.close();

    cout << "Car added successfully." << endl;
}
  
void Manager::deleteCustomerById(const string& customerId) {
    // Open the customer database file
    ifstream customerFile("customer_database.csv");

    if (!customerFile.is_open()) {
        cerr << "Error: Unable to open customer database file." << endl;
        return;
    }

    vector<vector<string>> customerData;
    string customerLine;

    // Read customer data from the file into a vector of vectors
    while (getline(customerFile, customerLine)) {
        stringstream ss(customerLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }

    customerFile.close();

    // Find the customer with the specified ID and remove it from the vector
    auto customerIt = remove_if(customerData.begin(), customerData.end(), [&customerId](const vector<string>& customer) {
        return customer.size() > 1 && customer[1] == customerId;  // Assuming customer ID is the second column
    });

    // Check if the customer was found and removed
    if (customerIt != customerData.end()) {
        // Get the index of the customer in the vector
        size_t index = distance(customerData.begin(), customerIt);

        // Check if the customer has rented a car
        if (index < customerData.size() && customerData[index].size() > 2 && customerData[index][2] != "None") {
            // Open the car database file
            ifstream carFile("car_database.csv");

            if (carFile.is_open()) {
                vector<vector<string>> carData;
                string carLine;

                // Read car data from the file into a vector of vectors
                while (getline(carFile, carLine)) {
                    stringstream carSS(carLine);
                    string carCell;
                    vector<string> carTemp;
                    while (getline(carSS, carCell, ',')) {
                        carTemp.push_back(carCell);
                    }
                    carData.push_back(carTemp);
                }

                carFile.close();

                // Find and update cars rented by the customer
                for (auto& car : carData) {
                    if (car.size() > 5 && car[5] == customerId) {  // Assuming customer ID is stored in column 5 of the car database
                        // Update car information
                        car[4] = "00000000";
                        car[3] = "false";
                        car[5] = "None";
                    }
                }

                // Write modified car data back to the file
                ofstream carOutfile("car_database.csv");
                if (carOutfile.is_open()) {
                    for (const auto& car : carData) {
                        for (size_t i = 0; i < car.size(); ++i) {
                            carOutfile << car[i];
                            if (i != car.size() - 1) carOutfile << ",";
                        }
                        carOutfile << endl;
                    }
                    carOutfile.close();
                } else {
                    cerr << "Error: Unable to open car database file." << endl;
                }
            } else {
                cerr << "Error: Unable to open car database file." << endl;
            }
        }

        // Remove the customer from the vector
        customerData.erase(customerIt, customerData.end());

        // Write modified customer data back to the file
        ofstream customerOutfile("customer_database.csv");
        if (customerOutfile.is_open()) {
            for (const auto& customer : customerData) {
                for (size_t i = 0; i < customer.size(); ++i) {
                    customerOutfile << customer[i];
                    if (i != customer.size() - 1) customerOutfile << ",";
                }
                customerOutfile << endl;
            }
            customerOutfile.close();
        } else {
            cerr << "Error: Unable to open customer database file." << endl;
        }

        cout << "Customer with ID '" << customerId << "' deleted successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }
}

void Manager::deleteEmployeeById(const string& employeeId) {
    // Open the employee database file
    ifstream employeeFile("employee_database.csv");

    if (!employeeFile.is_open()) {
        cout << "Error opening the employee file." << endl;
        return;
    }

    vector<vector<string>> employeeData;
    string employeeLine;

    // Read employee data from the file into a vector of vectors
    while (getline(employeeFile, employeeLine)) {
        stringstream ss(employeeLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }

    employeeFile.close();

    // Find the employee with the specified ID and remove it from the vector
    auto employeeIt = remove_if(employeeData.begin(), employeeData.end(), [&employeeId](const vector<string>& employee) {
        return employee.size() > 1 && employee[1] == employeeId;  // Assuming employee ID is the second column
    });

    // Check if the employee was found and removed
    if (employeeIt != employeeData.end()) {
        // Remove the employee from the vector
        employeeData.erase(employeeIt, employeeData.end());

        // Open the car database file
        ifstream carFile("car_database.csv");

        if (carFile.is_open()) {
            vector<vector<string>> carData;
            string carLine;

            // Read car data from the file into a vector of vectors
            while (getline(carFile, carLine)) {
                stringstream carSS(carLine);
                string carCell;
                vector<string> carTemp;
                while (getline(carSS, carCell, ',')) {
                    carTemp.push_back(carCell);
                }
                carData.push_back(carTemp);
            }

            carFile.close();

            // Find and update cars rented by the employee
            for (auto& car : carData) {
                if (car.size() > 5 && car[5] == employeeId) {  // Assuming employee ID is stored in column 5 of the car database
                    // Update car information
                    car[4] = "00000000";
                    car[3] = "false";
                    car[5] = "None";
                }
            }

            // Write modified car data back to the file
            ofstream carOutfile("car_database.csv");
            if (carOutfile.is_open()) {
                for (const auto& car : carData) {
                    for (size_t i = 0; i < car.size(); ++i) {
                        carOutfile << car[i];
                        if (i != car.size() - 1) carOutfile << ",";
                    }
                    carOutfile << endl;
                }
                carOutfile.close();
            } else {
                cerr << "Error: Unable to open car database file." << endl;
            }
        } else {
            cerr << "Error: Unable to open car database file." << endl;
        }

        // Write modified employee data back to the file
        ofstream employeeOutfile("employee_database.csv");
        if (employeeOutfile.is_open()) {
            for (const auto& employee : employeeData) {
                for (size_t i = 0; i < employee.size(); ++i) {
                    employeeOutfile << employee[i];
                    if (i != employee.size() - 1) employeeOutfile << ",";
                }
                employeeOutfile << endl;
            }
            employeeOutfile.close();
        } else {
            cerr << "Error: Unable to open employee database file." << endl;
        }

        cout << "Employee with ID '" << employeeId << "' deleted successfully." << endl;
    } else {
        cout << "Employee not found." << endl;
    }
}

void Manager::deleteCar(const string& carModel) {
    // Open the car database file
    ifstream file("car_database.csv");

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string line;

    // Read car data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    file.close();

    // Find the car with the specified model and remove it from the vector
    auto it = remove_if(carData.begin(), carData.end(), [&carModel](const vector<string>& car) {
        return car[0] == carModel;  // Assuming car model is the first column
    });

    // Check if the car was found and removed
    if (it != carData.end()) {
        carData.erase(it, carData.end());

        // Write modified car data back to the file
        ofstream outfile("car_database.csv");
        if (!outfile.is_open()) {
            cerr << "Error: Unable to open file." << endl;
            return;
        }

        for (const auto& car : carData) {
            for (size_t i = 0; i < car.size(); ++i) {
                outfile << car[i];
                if (i != car.size() - 1) outfile << ",";
            }
            outfile << endl;
        }

        cout << "Car '" << carModel << "' deleted successfully." << endl;
    } else {
        cout << "Car not found." << endl;
    }
}

void displayCarDatabase(const Manager& manager) {
    // Open the car database file
    ifstream file("car_database.csv");

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Print headers with setw for consistent spacing
    cout << setw(15) << left << "Car Model" << setw(25) << left << "Avg Customer Record" 
         << setw(15) << left << "Condition" << setw(10) << left << "Is Rented" 
         << setw(15) << left << "Due Date" << setw(15) << left << "Owner" << endl;
         cout<<endl;

    // Read each line from the file and display car information
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> carInfo;

        // Read tokens separated by commas
        while (getline(ss, cell, ',')) {
            carInfo.push_back(cell);
        }

        // Print all six attributes with setw for consistent spacing
        cout << setw(15) << left << carInfo[0] << setw(25) << left << carInfo[1] 
             << setw(15) << left << carInfo[2] << setw(10) << left << carInfo[3] 
             << setw(15) << left << carInfo[4] << setw(15) << left << carInfo[5] << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------------" << endl;
    }

    // Close the file
    file.close();
}

void displayCustomerDatabase(const Manager& manager) {
    // Open the customer database file
    ifstream file("customer_database.csv");

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Print headers with setw for consistent spacing
    cout << setw(20) << left << "Customer Name" << setw(15) << left << "Customer ID" 
         << setw(20) << left << "Customer Password" << setw(20) << left << "Customer Record" << endl;
    cout << endl;

    // Read each line from the file and display customer information
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> customerInfo;

        // Read tokens separated by commas
        while (getline(ss, cell, ',')) {
            customerInfo.push_back(cell);
        }

        // Print all four attributes with setw for consistent spacing
        cout << setw(20) << left << customerInfo[0] << setw(15) << left << customerInfo[1] 
             << setw(20) << left << customerInfo[2] << setw(20) << left << customerInfo[3] << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
    }

    // Close the file
    file.close();
}

  void displayEmployeeDatabase(const Manager& manager) {
    // Open the employee database file
    ifstream file("employee_database.csv");

    if (!file.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    // Print headers with setw for consistent spacing
    cout << setw(20) << left << "Employee Name" << setw(15) << left << "Employee ID" 
         << setw(20) << left << "Employee Password" << setw(20) << left << "Employee Record" << endl;
    cout << endl;

    // Read each line from the file and display employee information
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> employeeInfo;

        // Read tokens separated by commas
        while (getline(ss, cell, ',')) {
            employeeInfo.push_back(cell);
        }

        // Print all four attributes with setw for consistent spacing
        cout << setw(20) << left << employeeInfo[0] << setw(15) << left << employeeInfo[1] 
             << setw(20) << left << employeeInfo[2] << setw(20) << left << employeeInfo[3] << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
    }

    // Close the file
    file.close();
}

void Manager::updateCarModel(const string& oldModel, const string& newModel) {
    // Open the car database file
    ifstream file("car_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string line;

    // Read car data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    file.close();

    // Find the car with the specified model and update its model
    auto it = find_if(carData.begin(), carData.end(), [&oldModel](const vector<string>& car) {
        return car[0] == oldModel;  // Assuming car model is the first column
    });

    if (it != carData.end()) {
        (*it)[0] = newModel;

        // Write modified car data back to the file
        ofstream outfile("car_database.csv");
        if (outfile.is_open()) {
            for (const auto& car : carData) {
                for (size_t i = 0; i < car.size(); ++i) {
                    outfile << car[i];
                    if (i != car.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Car model updated successfully." << endl;
    } else {
        cout << "Car not found." << endl;
    }
}

void Manager::updateAvgCustomerRecord(const string& carModel, const string& avgCustomerRecord) {
    // Open the car database file
    ifstream file("car_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string line;

    // Read car data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    file.close();

    // Find the car with the specified model and update its avg customer record
    auto it = find_if(carData.begin(), carData.end(), [&carModel](const vector<string>& car) {
        return car[0] == carModel;  // Assuming car model is the first column
    });

    if (it != carData.end()) {
        (*it)[1] = avgCustomerRecord;

        // Write modified car data back to the file
        ofstream outfile("car_database.csv");
        if (outfile.is_open()) {
            for (const auto& car : carData) {
                for (size_t i = 0; i < car.size(); ++i) {
                    outfile << car[i];
                    if (i != car.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Avg customer record updated successfully." << endl;
    } else {
        cout << "Car not found." << endl;
    }
}

void Manager::updateCondition(const string& carModel, const string& condition) {
    // Open the car database file
    ifstream file("car_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string line;

    // Read car data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    file.close();

    // Find the car with the specified model and update its condition
    auto it = find_if(carData.begin(), carData.end(), [&carModel](const vector<string>& car) {
        return car[0] == carModel;  // Assuming car model is the first column
    });

    if (it != carData.end()) {
        (*it)[2] = condition;

        // Write modified car data back to the file
        ofstream outfile("car_database.csv");
        if (outfile.is_open()) {
            for (const auto& car : carData) {
                for (size_t i = 0; i < car.size(); ++i) {
                    outfile << car[i];
                    if (i != car.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Condition updated successfully." << endl;
    } else {
        cout << "Car not found." << endl;
    }
}
 
 void Manager::updateCustomerPassword(const string& customerId, const string& newPassword) {
    // Open the customer database file
    ifstream file("customer_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> customerData;
    string line;

    // Read customer data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }

    file.close();

    // Find the customer with the specified ID and update its password
    auto it = find_if(customerData.begin(), customerData.end(), [&customerId](const vector<string>& customer) {
        return customer[1] == customerId;  // Assuming customer ID is the second column
    });

    if (it != customerData.end()) {
        (*it)[2] = newPassword;  // Assuming password is the fourth column

        // Write modified customer data back to the file
        ofstream outfile("customer_database.csv");
        if (outfile.is_open()) {
            for (const auto& customer : customerData) {
                for (size_t i = 0; i < customer.size(); ++i) {
                    outfile << customer[i];
                    if (i != customer.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Password updated successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }
}

void Manager::updateCustomerRecord(const string& customerId, const string& newCustomerRecord) {
    // Open the customer database file
    ifstream file("customer_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> customerData;
    string line;

    // Read customer data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }

    file.close();

    // Find the customer with the specified ID and update its customer record
    auto it = find_if(customerData.begin(), customerData.end(), [&customerId](const vector<string>& customer) {
        return customer[1] == customerId;  // Assuming customer ID is the second column
    });

    if (it != customerData.end()) {
        (*it)[3] = newCustomerRecord;  // Assuming customer record is the fourth column

        // Write modified customer data back to the file
        ofstream outfile("customer_database.csv");
        if (outfile.is_open()) {
            for (const auto& customer : customerData) {
                for (size_t i = 0; i < customer.size(); ++i) {
                    outfile << customer[i];
                    if (i != customer.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Customer record updated successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }
}
 
void Manager::updateCustomerName(const string& customerId, const string& newName) {
    // Open the customer database file
    ifstream file("customer_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> customerData;
    string line;

    // Read customer data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        customerData.push_back(temp);
    }

    file.close();

    // Find the customer with the specified ID and update its name
    auto it = find_if(customerData.begin(), customerData.end(), [&customerId](const vector<string>& customer) {
        return customer[1] == customerId;  // Assuming customer ID is the second column
    });

    if (it != customerData.end()) {
        (*it)[0] = newName;  // Assuming name is the first column

        // Write modified customer data back to the file
        ofstream outfile("customer_database.csv");
        if (outfile.is_open()) {
            for (const auto& customer : customerData) {
                for (size_t i = 0; i < customer.size(); ++i) {
                    outfile << customer[i];
                    if (i != customer.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Customer name updated successfully." << endl;
    } else {
        cout << "Customer not found." << endl;
    }
}

void Manager::updateEmployeePassword(const string& employeeId, const string& newPassword) {
    // Open the employee database file
    ifstream file("employee_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> employeeData;
    string line;

    // Read employee data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }

    file.close();

    // Find the employee with the specified ID and update its password
    auto it = find_if(employeeData.begin(), employeeData.end(), [&employeeId](const vector<string>& employee) {
        return employee[1] == employeeId;  // Assuming employee ID is the first column
    });

    if (it != employeeData.end()) {
        (*it)[2] = newPassword;  // Assuming password is the third column

        // Write modified employee data back to the file
        ofstream outfile("employee_database.csv");
        if (outfile.is_open()) {
            for (const auto& employee : employeeData) {
                for (size_t i = 0; i < employee.size(); ++i) {
                    outfile << employee[i];
                    if (i != employee.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Employee password updated successfully." << endl;
    } else {
        cout << "Employee not found." << endl;
    }
}

void Manager::updateEmployeeRecord(const string& employeeId, const string& newRecord) {
    // Open the employee database file
    ifstream file("employee_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> employeeData;
    string line;

    // Read employee data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }

    file.close();

    // Find the employee with the specified ID and update its record
    auto it = find_if(employeeData.begin(), employeeData.end(), [&employeeId](const vector<string>& employee) {
        return employee[1] == employeeId;  // Assuming employee ID is the second column
    });

    if (it != employeeData.end()) {
        (*it)[3] = newRecord;  // Assuming record is the fourth column

        // Write modified employee data back to the file
        ofstream outfile("employee_database.csv");
        if (outfile.is_open()) {
            for (const auto& employee : employeeData) {
                for (size_t i = 0; i < employee.size(); ++i) {
                    outfile << employee[i];
                    if (i != employee.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Employee record updated successfully." << endl;
    } else {
        cout << "Employee not found." << endl;
    }
}

void Manager::updateEmployeeName(const string& employeeId, const string& newName) {
    // Open the employee database file
    ifstream file("employee_database.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    vector<vector<string>> employeeData;
    string line;

    // Read employee data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }

    file.close();

    // Find the employee with the specified ID and update its name
    auto it = find_if(employeeData.begin(), employeeData.end(), [&employeeId](const vector<string>& employee) {
        return employee[1] == employeeId;  // Assuming employee ID is the first column
    });

    if (it != employeeData.end()) {
        (*it)[0] = newName;  // Assuming name is the second column

        // Write modified employee data back to the file
        ofstream outfile("employee_database.csv");
        if (outfile.is_open()) {
            for (const auto& employee : employeeData) {
                for (size_t i = 0; i < employee.size(); ++i) {
                    outfile << employee[i];
                    if (i != employee.size() - 1) outfile << ",";
                }
                outfile << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open file for writing." << endl;
        }

        cout << "Employee name updated successfully." << endl;
    } else {
        cout << "Employee not found." << endl;
    }
}

class Employee : public User {
    public:
    int employee_record;
    void rentCarForEmployee(const string& carModel, const string& inputDate);
    void fineForRentedCarsforEmployee(const string& inputDate);
    void ClearDuesForEmployee(const string& inputDate, int damage) ;
};

void displayAvailableCarsForEmployees(const Employee employee) {
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    bool carsAvailable = false;  // Flag to check if any cars are available

    for (int i = 0; i < data.size(); i++) {
        if (data[i][3] == "false") {
            carsAvailable = true;  // Set the flag to true if at least one car is available
            cout << "Car Model: " << data[i][0] << endl;
            cout << "Avg Customer Record Required: " << data[i][1] << endl;
            cout << "health: " << data[i][2] << endl;
            cout << "----------------------------------------" << endl;
            // Add other relevant information to display
        }
    }

    if (!carsAvailable) {
        cout << "No cars are available to be rented as of now." << endl;
        cout << endl;
    }
}

void Employee::rentCarForEmployee(const string& carModel, const string& inputDate) {
    ifstream carFile("car_database.csv");
    ifstream employeeFile("employee_database.csv");

    if (!carFile.is_open() || !employeeFile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    // Read car data from the file into a vector of vectors
    vector<vector<string>> carData;
    string carLine;

    while (getline(carFile, carLine)) {
        stringstream ss(carLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }

    carFile.close();

    // Read employee data from the file into a vector of vectors
    vector<vector<string>> employeeData;
    string employeeLine;

    while (getline(employeeFile, employeeLine)) {
        stringstream ss(employeeLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }

    employeeFile.close();

    // Find the car with the specified model
    for (int i = 0; i < carData.size(); i++) {
        if (carData[i][0] == carModel) {  // Assuming car model is the first column
            // Check if the car is available for rent
            if (carData[i][3] == "false") {  // Assuming availability status is the fourth column
                // Find the employee with the specified ID
                string employeeId = this->ID;  // Assuming employee ID is the second column
                int employeeRecord = 0;

                // Find the employee record in the employee database
                for (int j = 0; j < employeeData.size(); j++) {
                    if (employeeData[j][1] == employeeId) {  // Assuming employee ID is the second column
                        employeeRecord = stoi(employeeData[j][3]);  // Assuming employee record is the fourth column
                        break;
                    }
                }

                // Find the average employee record for the car
                int avgEmployeeRecord = stoi(carData[i][1]);  // Assuming average employee record is the second column

                // Check if the employee record is greater than or equal to the average
                if (employeeRecord >= avgEmployeeRecord) {
                    // Increment the input date by 10 days
                    string rentedDate = add10Days(inputDate);

                    // Update the CSV file with the new employee Name and date
                    carData[i][5] = this->ID;  // Using the "this" keyword to access class member variable
                    carData[i][3] = "true";
                    carData[i][4] = rentedDate;

                    // Notify successful rental
                    cout << "Car rented successfully by employee " << this->Name << " on " << inputDate << "." << endl;

                    // Write modified car data back to the file
                    ofstream carOutfile("car_database.csv");
                    if (!carOutfile.is_open()) {
                        cerr << "Error: Unable to open file." << endl;
                        return;
                    }
                    for (int k = 0; k < carData.size(); k++) {
                        for (int l = 0; l < carData[k].size(); l++) {
                            carOutfile << carData[k][l];
                            if (l != carData[k].size() - 1) carOutfile << ",";
                        }
                        if (k != carData.size() - 1) carOutfile << endl;
                    }
                    carOutfile.close();
                    return;  // Return after updating the first matching car (assuming unique models)
                } else {
                    cout << "Employee record is not sufficient to rent this car." << endl;
                    return;  // Return if the employee record is not sufficient
                }
            } else {
                cout << "Car is not available for rent." << endl;
                return;  // Return if the car is not available
            }
        }
    }

    // If the loop completes without finding the specified car model
    cout << "This Car Model cannot be rented." << endl;
}

void seeRentedCarsForEmployees(const Employee& employee) {
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    cout << "Cars Rented by " << employee.Name << ":" << endl;

    bool hasRentedCars = false;  // Flag to check if the employee has rented cars

    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == employee.ID && data[i][3] == "true") {
            hasRentedCars = true;  // Set the flag to true if the employee has rented cars
            cout << "Car Model: " << data[i][0] << endl;
            cout << "Avg Customer Record Required: " << data[i][1] << endl;
            cout << "health: " << data[i][2] << endl;
            cout << "Date of Return: " << data[i][4] << endl;
            cout << "----------------------------------------" << endl;
            // Add other relevant information to display
        }
    }

    if (!hasRentedCars) {
        cout << "You have no rented cars." << endl;
    }
}


void Employee::fineForRentedCarsforEmployee(const string& inputDate) {
    double finePerDay=8.5;
    ifstream file("car_database.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> data;
    string line;

    // Read data from the file into a vector of vectors
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        data.push_back(temp);
    }
    file.close();

    // Check if there are any cars rented by the employee
    bool employeeHasRentedCars = false;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == this->ID && data[i][3] == "true") {  // Assuming employee ID is the fifth column
            employeeHasRentedCars = true;
            break;
        }
    }

    if (!employeeHasRentedCars) {
        cout << "Employee has no rented cars." << endl;
        return;
    }

    // Check for rented cars by the employee and impose fines
    for (int i = 0; i < data.size(); i++) {
        if (data[i][5] == this->ID && data[i][3] == "true") {  // Assuming employee ID is the fifth column
            string databaseDate = data[i][4];  // Assuming date is the fourth column

            // Calculate the difference in days using the daysBetweenDates function
            int daysOverdue = daysBetweenDates(databaseDate, inputDate);

            // Check if the input date is greater than the database date
            if (daysOverdue > 0) {
                // Calculate fine
               double fine = daysOverdue * finePerDay;

                // Print fine along with the car model
                cout << "Fine for " << data[i][0] << ": $" << fine << endl;

                // Update data in memory (not in the file yet)
                // Set availability status to false

                // no else statements for future delivery date;
            } else {
                cout << "No existing fines as of now for "<<data[i][0] << endl;
            }
        }
    }

    // Write modified data back to the file
    ofstream outfile("car_database.csv");
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }
    for (int j = 0; j < data.size(); j++) {
        for (int k = 0; k < data[j].size(); k++) {
            outfile << data[j][k];
            if (k != data[j].size() - 1) outfile << ",";
        }
        if (j != data.size() - 1) outfile << endl;
    }
    outfile.close();
}

void Employee::ClearDuesForEmployee(const string& inputDate, int damage) {
    ifstream carFile("car_database.csv");
    ifstream employeeFile("employee_database.csv");

    if (!carFile.is_open() || !employeeFile.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    vector<vector<string>> carData;
    string carLine;
    // Read car data from the file into a vector of vectors
    while (getline(carFile, carLine)) {
        stringstream ss(carLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        carData.push_back(temp);
    }
    carFile.close();

    vector<vector<string>> employeeData;
    string employeeLine;
    // Read employee data from the file into a vector of vectors
    while (getline(employeeFile, employeeLine)) {
        stringstream ss(employeeLine);
        string cell;
        vector<string> temp;
        while (getline(ss, cell, ',')) {
            temp.push_back(cell);
        }
        employeeData.push_back(temp);
    }
    employeeFile.close();

    // Check if the employee has rented cars
    bool employeeHasRentedCars = false;
    int rentedCarIndex = -1; 
    int k=0; // Index to store the position of the rented car
    for (int i = 0; i < carData.size(); i++) {
        string databaseDate = carData[i][4];  // Assuming date is the fifth column
        // Calculate the difference in days
        int daysOverdue = daysBetweenDates(databaseDate, inputDate);

        if (carData[i][5] == this->ID && carData[i][3] == "true" && daysOverdue > 0) {  // Assuming employee ID is the sixth column
            employeeHasRentedCars = true;
            rentedCarIndex = i; 
            k = daysOverdue; // Save the index of the rented car
            break;  // Break out of the loop after finding the first rented car
        }
    }

    if (!employeeHasRentedCars || rentedCarIndex == -1) {
        cout << "Employee has no rented cars." << endl;
        return;
    }

    // Calculate dues for the rented car
    // Check if there are existing dues
    if (k > 0) {
        // Calculate fine
        double finePerDay = 10;  // Assumed fine per day
        double fine = k * finePerDay;

        // Update car data
        carData[rentedCarIndex][4] = "00000000";  // Reset date to "00000000"
        carData[rentedCarIndex][2] = to_string(damage);  // Change the 3rd column to damage value
        carData[rentedCarIndex][5] = "None";
        carData[rentedCarIndex][3] = "false";

        // Print dues cleared information
        cout << "Dues Cleared for Car Model " << carData[rentedCarIndex][0] << " on " << inputDate << " by Employee " << this->Name << "." << endl;
        cout << "Amount of Dues: $" << fine << endl;

        // Write modified car data back to the file
        ofstream carOutfile("car_database.csv");
        if (!carOutfile.is_open()) {
            cerr << "Error: Unable to open car file." << endl;
            return;
        }
        for (int j = 0; j < carData.size(); j++) {
            for (int k = 0; k < carData[j].size(); k++) {
                carOutfile << carData[j][k];
                if (k != carData[j].size() - 1) carOutfile << ",";
            }
            if (j != carData.size() - 1) carOutfile << endl;
        }
        carOutfile.close();

        // Update employee health in the employee database
        for (int l = 0; l < employeeData.size(); l++) {
            if (employeeData[l][1] == this->ID) {  // Assuming employee ID is the first column
                double currentHealth = stod(employeeData[l][3]);  // Assuming health is the fourth column
                double newHealth = currentHealth * (1.0 - ((1.0 - ((static_cast<double>(damage) / 100))) * (1.0 - ((static_cast<double>(damage) / 100)))));

                // Ensure the new health value is within [0, 1]
                newHealth = max(0.0, newHealth);

                // Update the 4th column with the new health value
                employeeData[l][3] = to_string(newHealth);

                // Write modified employee data back to the file
                ofstream employeeOutfile("employee_database.csv");
                if (!employeeOutfile.is_open()) {
                    cerr << "Error: Unable to open employee file." << endl;
                    return;
                }
                for (int m = 0; m < employeeData.size(); m++) {
                    for (int n = 0; n < employeeData[m].size(); n++) {
                        employeeOutfile << employeeData[m][n];
                        if (n != employeeData[m].size() - 1) employeeOutfile << ",";
                    }
                    if (m != employeeData.size() - 1) employeeOutfile << endl;
                }
                employeeOutfile.close();
                return;
            }
        }

        // If the loop completes without finding the specified employee ID in the employee database
        cout << "Employee not found in the employee database." << endl;
        return;
    } else {
        cout << "No existing dues as of now." << endl;
    }
}

void exploreCustomer(Customer& C) {
    bool exit = true;
    int choice;
    while (exit) {
        cout<<endl;
        cout << "Welcome" << C.Name << endl;
        cout << "Press 1 for Displaying Available cars" << endl;
        cout << "Press 2 for Renting cars" << endl;
        cout << "Press 3 for Seeing the cars rented by the customer" << endl;
        cout << "Press 4 for Checking the dues" << endl;
        cout << "Press 5 for Clearing the dues" << endl;
        cout << "Press 6 for Logout" << endl;

        cout << "Enter your choice: ";
        cin >> choice;
      
        switch (choice) {
            case 1:
              cout<<endl;
                displayAvailableCarsForCustomers(C);
                break;
            case 2: {
                string carModel, inputDate;
                cout << "Enter Car Model: ";
                cin >> carModel;
                cout << "Enter Date (DDMMYYYY): ";
                cin >> inputDate;
                if(isValidDateFormat(inputDate)){
              C.rentCar(carModel, inputDate);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
                
                break;
            }
            case 3:
                seeRentedCars(C);
                break;
            case 4: {
                string currentDate;
                cout << "Enter Current Date (DDMMYYYY): ";
                cin >> currentDate;

                if(isValidDateFormat(currentDate)){
               C.fineForRentedCars(currentDate);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
                
            }
            case 5: {
                string currentDate;
                int damage;
                cout << "Enter Final Health: ";
                cin >> damage;
                cout<<"Enter Current Date (DDMMYYYY):";
                cin>> currentDate;

                if(isValidDateFormat(currentDate)){
            C.ClearDues(currentDate,damage);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
            }
            case 6:
                cout << "Logging out." << endl;
                exit = false;
                return;  // Exit the function and the loop

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
}
 
void exploreManager(Manager& manager) {
    bool exit = true;
    int choice;
    while (exit) {
        cout << "Welcome, Manager!" << endl;
        cout << "Press 1 to Add a Customer" << endl;
        cout << "Press 2 to Add an Employee" << endl;
        cout << "Press 3 to Add a Car" << endl;
        cout << "Press 4 to Delete a Customer by ID" << endl;
        cout << "Press 5 to Delete a Car by Model" << endl;
        cout << "Press 6 to Delete an Employee by ID" << endl;
        cout << "Press 7 to Update Car Model" << endl;
        cout << "Press 8 to Update Average Customer Record" << endl;
        cout << "Press 9 to Update Condition" << endl;
        cout << "Press 10 to Update Customer Password" << endl;
        cout << "Press 11 to Update Customer Record" << endl;
        cout << "Press 12 to Update Customer Name" << endl;
        cout << "Press 13 to Update Employee Password" << endl;
        cout << "Press 14 to Update Employee Record" << endl;
        cout << "Press 15 to Update Employee Name" << endl;
        cout << "Press 16 to View Car Database" << endl;
      cout << "Press 17 to View Customer Database" << endl;
      cout<<"Press 18 to View Employee Database"<<endl;
        cout << "Press 19 to Logout" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, id, password;
                cout << "Enter Customer Name: ";
                cin >> name;
                cout << "Enter Customer ID: ";
                cin >> id;
                cout << "Enter Customer Password: ";
                cin >> password;
                manager.addCustomer(name, id, password);
                break;
            }
            case 2: {
                string name, id, password;
                cout << "Enter Employee Name: ";
                cin >> name;
                cout << "Enter Employee ID: ";
                cin >> id;
                cout << "Enter Employee Password: ";
                cin >> password;
                manager.addEmployee(name, id, password);
                break;
            }
            case 3: {
                string modelName, condition;
                cout << "Enter Car Model Name: ";
                cin >> modelName;
                cout << "Enter Car Condition: ";
                cin >> condition;
                manager.addCar(modelName, condition);
                break;
            }
            case 4: {
                string customerId;
                cout << "Enter Customer ID to Delete: ";
                cin >> customerId;
                manager.deleteCustomerById(customerId);
                break;
            }
            case 5: {
                string carModel;
                cout << "Enter Car Model to Delete: ";
                cin >> carModel;
                manager.deleteCar(carModel);
                break;
            }
            case 6: {
                string employeeId;
                cout << "Enter Employee ID to Delete: ";
                cin >> employeeId;
                manager.deleteEmployeeById(employeeId);
                break;
            }
            case 7: {
                string oldModel, newModel;
                cout << "Enter Old Car Model: ";
                cin >> oldModel;
                cout << "Enter New Car Model: ";
                cin >> newModel;
                manager.updateCarModel(oldModel, newModel);
                break;
            }
            case 8: {
                string carModel, avgCustomerRecord;
                cout << "Enter Car Model: ";
                cin >> carModel;
                cout << "Enter Average Customer Record: ";
                cin >> avgCustomerRecord;
                manager.updateAvgCustomerRecord(carModel, avgCustomerRecord);
                break;
            }
            case 9: {
                string carModel, condition;
                cout << "Enter Car Model: ";
                cin >> carModel;
                cout << "Enter Car Condition: ";
                cin >> condition;
                manager.updateCondition(carModel, condition);
                break;
            }
            case 10: {
                string customerId, newPassword;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cout << "Enter New Password: ";
                cin >> newPassword;
                manager.updateCustomerPassword(customerId, newPassword);
                break;
            }
            case 11: {
                string customerId, newRecord;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cout << "Enter New Record: ";
                cin >> newRecord;
                manager.updateCustomerRecord(customerId, newRecord);
                break;
            }
            case 12: {
                string customerId, newName;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cout << "Enter New Name: ";
                cin >> newName;
                manager.updateCustomerName(customerId, newName);
                break;
            }
            case 13: {
                string employeeId, newPassword;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                cout << "Enter New Password: ";
                cin >> newPassword;
                manager.updateEmployeePassword(employeeId, newPassword);
                break;
            }
            case 14: {
                string employeeId, newRecord;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                cout << "Enter New Record: ";
                cin >> newRecord;
                manager.updateEmployeeRecord(employeeId, newRecord);
                break;
            }
            case 15: {
                string employeeId, newName;
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                cout << "Enter New Name: ";
                cin >> newName;
                manager.updateEmployeeName(employeeId, newName);
                break;
            }
            case 16: {
                displayCarDatabase(manager);
                break;
            }
            case 17:{
                displayCustomerDatabase(manager);
                break;
            }
            case 18:{
                displayEmployeeDatabase(manager);
                break;
            }
            case 19:
                cout << "Logging out." << endl;
                exit = false;
                return;  // Exit the function and the loop

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
}
  
void exploreEmployee(Employee& E) {
    bool exit = true;
    int choice;
    while (exit) {
        cout << endl;
        cout << "Welcome " << E.Name<< endl;
        cout << "Press 1 for Displaying Available cars" << endl;
        cout << "Press 2 for Renting cars" << endl;
        cout << "Press 3 for Seeing the cars rented by the employee" << endl;
        cout << "Press 4 for Checking the fines for rented cars" << endl;
        cout << "Press 5 for Clearing the dues for rented cars" << endl;
        cout << "Press 6 for Logout" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << endl;
            displayAvailableCarsForEmployees(E);
                break;
            case 2: {
                string carModel, inputDate;
                cout << "Enter Car Model: ";
                cin >> carModel;
                cout << "Enter Date (DDMMYYYY): ";
                cin >> inputDate;
                if(isValidDateFormat(inputDate)){
                E.rentCarForEmployee(carModel, inputDate);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
            }
            case 3:
                seeRentedCarsForEmployees(E);
                break;
            case 4: {
                string currentDate;
                int finePerDay;
                cout << "Enter Current Date (DDMMYYYY): ";
                cin >> currentDate;
                if(isValidDateFormat(currentDate)){
                E.fineForRentedCarsforEmployee(currentDate);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
                
            }
            case 5: {
                string currentDate;
                int damage;
                cout << "Enter Final Health: ";
                cin >> damage;
                cout << "Enter Current Date (DDMMYYYY): ";
                cin >> currentDate;
                 if(isValidDateFormat(currentDate)){
                E.ClearDuesForEmployee(currentDate,damage);
                }
                else{
                    cout<<"Date not Entered in the Required Format "<<endl;
                }
                break;
            }
            case 6:
                cout << "Logging out." << endl;
                exit = false;
                return;  // Exit the function and the loop

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
}

void Login(char UserType) {
    
    if (UserType == 'M') {
        cout << "Press R to Register " << endl;
        cout << "Press L to Login" << endl;
        char ManagerType;
        cin >> ManagerType;
        Manager Mr;
        cout << "Enter Name :" << endl;
        cin >> Mr.Name;
        cout << "Enter ID :" << endl;
        cin >> Mr.ID;
        cout << "Enter Password :" << endl;
        cin >> Mr.Password;
       
         if (ManagerType == 'R') {
  insertIntoCSV("manager_database.csv",Mr.Name,Mr.ID,Mr.Password);
   exploreManager(Mr);
   
         }
        else  if (ManagerType == 'L') {
if(userExistsInCSV("manager_database.csv",Mr.Name,Mr.ID,Mr.Password)){
     cout<< "\n";
    cout<<"Welcome"<<endl;
exploreManager(Mr);
}
else{
    cout<<"Wrong Credentials Entered, Try Again"<<endl;
}
         }
    }
    else if (UserType == 'C') {
        cout << "Press R to Register " << endl;
        cout << "Press L to Login" << endl;
        char CustomerType;
        cin >> CustomerType;
        Customer Cr;
        cout << "Enter Name :" << endl;
        cin >> Cr.Name;
        cout << "Enter ID :" << endl;
        cin >> Cr.ID;
        cout << "Enter Password :" << endl;
        cin >> Cr.Password;
        if (CustomerType == 'R') {
           Cr. customer_record=15;
       if(isIDTaken(Cr.ID)){
                cout << "Error: ID already taken. Please choose a different ID." << endl;
            }
            else{
  insertIntoCSVforEmployeesAndCustomers("customer_database.csv",Cr.Name,Cr.ID,Cr.Password,Cr.customer_record);
  cout<<"You have been Registered!"<<endl;
   exploreCustomer(Cr);
            }
   

        }
         else if (CustomerType == 'L') {

if(userExistsInCSV("customer_database.csv",Cr.Name,Cr.ID,Cr.Password)){
    cout<<"Welcome"<<endl;
exploreCustomer(Cr);

}
else{
    cout<<"Wrong Credentials Entered, Try Again"<<endl;
}
        }

    }
    else  if (UserType=='E'){

        cout << "Press R to Register " << endl;
        cout << "Press L to Login" << endl;
        char EmployeeType;
        cin >> EmployeeType;
        Employee Ey;
        cout << "Enter Name :" << endl;
        cin >> Ey.Name;
        cout << "Enter ID :" << endl;
        cin >> Ey.ID;
        cout << "Enter Password :" << endl;
        cin >> Ey.Password;
        if (EmployeeType == 'R') {
            Ey.employee_record=15;
 if(isIDTaken(Ey.ID)){
                cout << "Error: ID already taken. Please choose a different ID." << endl;
            }
            else{
  insertIntoCSVforEmployeesAndCustomers("employee_database.csv",Ey.Name,Ey.ID,Ey.Password,Ey.employee_record);
  cout<<"You have been Registered!"<<endl;
   exploreEmployee(Ey);
            }
        }
        else if (EmployeeType == 'L') {
if(userExistsInCSV("employee_database.csv",Ey.Name,Ey.ID,Ey.Password)){
    cout<<"Welcome"<<endl;
    exploreEmployee(Ey);
}
else{
    cout<<"Wrong Credentials Entered, Try Again"<<endl;
}
        }
    }
}
void Welcome() {
    char userType;
    while (true) {
        cout << "Welcome To the Car Rental System" << endl;
        cout << endl;
        cout << "To identify yourself as a Manager, Login with M" << endl;
        cout << "To identify yourself as a Customer, Login with C" << endl;
        cout << "To identify yourself as an Employee, Login with E" << endl;
        cout << "To exit, press A" << endl;

        cout << "Enter your choice: ";
        cin >> userType;

        switch (userType) {
            case 'M':
                Login(userType);
                break;
            case 'C':
                Login(userType);
                break;
            case 'E':
                Login(userType);
                break;
            case 'A':
                cout << "Logging out. Thank you for visiting the car rental system. Do visit again." << endl;
                return;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
}

int main()
{
Welcome();
    return 0;
}

