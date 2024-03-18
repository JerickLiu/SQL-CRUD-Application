#include <iostream>
#include <limits>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

connection* connect_db(string dbname, string user, string password);
void getAllStudents(connection *C);
void addStudent(connection *C);
void updateStudentEmail(connection *C);
void deleteStudent(connection *C);

// Function to connect to the database
connection* connect_db(string dbname, string user, string password) {
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

    W.exec(sql);
    W.commit();
    cout << "Records created successfully" << endl;
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

    W.exec(sql);
    W.commit();
    cout << "Email updated successfully" << endl;
}

// Function to delete the record of the student with the specified student_id
void deleteStudent(connection *C) {
    int student_id;
    cout << "Enter student ID to delete: ";
    cin >> student_id;

    work W(*C);
    
    /* Format the SQL query */
    string sql = "DELETE from students where student_id = " + to_string(student_id) + ";";

    W.exec(sql);
    W.commit();
    cout << "Record deleted successfully" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <dbname> <username> <password>" << endl;
        return 1;
    }

    string dbname = argv[1];
    string user = argv[2];
    string password = argv[3];

    connection* C = connect_db(dbname, user, password);
    if (C == nullptr) {
        return 1;
    }

    string input;
    cout << "Database Connection Established.\n" << endl;

    while (true) {
        cout <<
            "\nSelect one of the following options:\n"
            "1. Get all students / Prints the entire 'students' table\n"
            "2. Add a new student\n"
            "3. Update the email of a current student\n"
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

    C->disconnect();
    delete C;
    return 0;
}
