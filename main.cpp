/*
COMP3005 - Assignment 3
Jerick Liu
101225819
2024-03-18
*/

#include <iostream>
#include <limits>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

// Function to connect to the database
connection* connect_db(string dbname, string user, string password) {
    // Attempt to connect to the database. Any error is caught and safetly exits.
    try {
        connection *C = new connection("dbname = " + dbname + " user = " + user + " password = " + password + " hostaddr = 172.28.240.1 port = 5432");
        if (C->is_open()) {
            cout << "Connected to database successfully: " << C->dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return nullptr;
        }
        return C;
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return nullptr;
    }
}

// Function to retrieve and display all records from the students table
void getAllStudents(connection *C) {
    string sql = "SELECT * from students;";

    /* Create a non-transactional object. */
    nontransaction N(*C);

    /* Execute SQL query */
    result R(N.exec(sql));

    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << "Student ID = " << c[0].as<int>() << ", First Name = " << c[1].as<string>() << ", Last Name = " << c[2].as<string>() << ", Email = " << c[3].as<string>() << ", Enrollment Date = " << c[4].as<string>() << endl;
    }
}

// Function to insert a new student record into the students table
void addStudent(connection *C) {
    string first_name, last_name, email, enrollment_date;
    cout << "Enter first name: ";
    cin >> first_name;
    cout << "Enter last name: ";
    cin >> last_name;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter enrollment date (YYYY-MM-DD): ";
    cin >> enrollment_date;

    work W(*C);
    
    /* Format the SQL query */
    string sql = "INSERT INTO students (first_name, last_name, email, enrollment_date) VALUES ('" + first_name + "', '" + last_name + "', '" + email + "', '" + enrollment_date + "');";

    result R = W.exec(sql);

    // Check if the query affected any rows
    if (R.affected_rows() == 0) {
        cout << "Failed to add student." << endl;
    } else {
        cout << "Record created successfully." << endl;
    }

    W.commit();
}

// Function to update the email address for a student with the specified student_id
void updateStudentEmail(connection *C) {
    int student_id;
    string new_email;
    cout << "Enter student ID: ";
    cin >> student_id;
    cout << "Enter new email: ";
    cin >> new_email;

    work W(*C);
    
    /* Format the SQL query */
    string sql = "UPDATE students set email = '" + new_email + "' where student_id = " + to_string(student_id) + ";";

    result R = W.exec(sql);

    // Check if the query affected any rows
    if (R.affected_rows() == 0) {
        cout << "Failed to find student with ID " << student_id << "." << endl;
    } else {
        cout << "Email updated successfully." << endl;
    }

    W.commit();
}

// Function to delete the record of the student with the specified student_id
void deleteStudent(connection *C) {
    int student_id;
    cout << "Enter student ID to delete: ";
    cin >> student_id;

    work W(*C);
    
    /* Format the SQL query */
    string sql = "DELETE from students where student_id = " + to_string(student_id) + ";";

    result R = W.exec(sql);

    // Check if the DELETE query affected any rows
    if (R.affected_rows() == 0) {
        cout << "Failed to find and delete student with ID " << student_id << "." << endl;
    } else {
        cout << "Record deleted successfully." << endl;
    }

    W.commit();
}

int main(int argc, char* argv[]) {
    // Ensure the program is run with the correct number of arguments (arg[0] is the file name)
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <dbname> <username> <password>" << endl;
        return 1;
    }

    // Parse the args from the arg list
    string dbname = argv[1];
    string user = argv[2];
    string password = argv[3];

    // Establish the connection
    connection* C = connect_db(dbname, user, password);
    if (C == nullptr) {
        return 1;
    }

    // Input var used for menu
    string input;

    while (true) {
        cout <<
            "\n\nSelect from the list of options:\n"
            "1. Get all students\n"
            "2. Add a new student\n"
            "3. Update email of a student\n"
            "4. Delete a student\n\n"
            "Enter 'exit' to exit the program.\n> ";
        getline(cin, input);

        if (input == "1") {
            getAllStudents(C);
        } else if (input == "2") {
            addStudent(C);
	    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer after user inputs whatever so the cin doesn't get garbage input
        } else if (input == "3") {
            updateStudentEmail(C);
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (input == "4") {
            deleteStudent(C);
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (input == "exit") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid option, please try again." << endl;
        }
    }

    // Exiting program, cleanup
    C->disconnect();
    delete C;
    return 0;
}
