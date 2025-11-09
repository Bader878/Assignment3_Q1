//packages 
#include <iostream>
#include <string>
#include <cstdlib>
#include <libpq-fe.h>

// function for getallstudents()
void getAllStudents(PGconn* conn) {
    // excutes SQL query 
    PGresult* res = PQexec(conn, "SELECT * FROM students ORDER BY student_id;");
    
    // error incase the Select fails
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "SELECT failed: " << PQerrorMessage(conn) << "\n";
        PQclear(res);
        return;
    }

    // number of rows
    int rows = PQntuples(res);
    // number of columns 
    int cols = PQnfields(res);

    // prints the header row 
    std::cout << "\n=== Students Table ===\n";
    for (int c = 0; c < cols; c++)
        std::cout << PQfname(res, c) << "\t";
    std::cout << "\n------------------------------------------\n";

    // prints each row 
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++)
            std::cout << PQgetvalue(res, r, c) << "\t";
        std::cout << "\n";
    }

    PQclear(res);
}

// addStudent function 
void addStudent(PGconn* conn) {
    std::string first, last, email, date;

    // this asks the user for student information and reads the data given from the user
    std::cout << "First name: ";
    std::cin >> first;
    std::cout << "Last name: ";
    std::cin >> last;
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Enrollment date (YYYY-MM-DD): ";
    std::cin >> date;

    // builds SQL INSERT query 
    std::string query =
        "INSERT INTO students (first_name, last_name, email, enrollment_date) "
        "VALUES ('" + first + "', '" + last + "', '" + email + "', '" + date + "');";

    // excute query 
    PGresult* res = PQexec(conn, query.c_str());

    // checks for error 
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        std::cerr << "INSERT failed: " << PQerrorMessage(conn) << "\n";
    else
        std::cout << "Student added successfully.\n";

    PQclear(res);
}

// updates student email 
void updateStudentEmail(PGconn* conn) {
    int id;
    std::string newEmail;

    // asks for student id and new email, and reads the user input
    std::cout << "Enter student ID: ";
    std::cin >> id;
    std::cout << "New email: ";
    std::cin >> newEmail;

    // builds UPDATE query 
    std::string query =
        "UPDATE students SET email='" + newEmail + "' WHERE student_id=" + std::to_string(id) + ";";

    // excutes UPDATE
    PGresult* res = PQexec(conn, query.c_str());

    // checks for error 
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        std::cerr << "UPDATE failed: " << PQerrorMessage(conn) << "\n";
    else
        std::cout << "Email updated.\n";

    PQclear(res);
}

// Deletes student 
void deleteStudent(PGconn* conn) {
    // asks user for student id and reads user input
    int id;
    std::cout << "Enter student ID to delete: ";
    std::cin >> id;

   // builds DELETE query
    std::string query =
        "DELETE FROM students WHERE student_id=" + std::to_string(id) + ";";
    // excutes DELETE query
    PGresult* res = PQexec(conn, query.c_str());

    // checks for error
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        std::cerr << "DELETE failed: " << PQerrorMessage(conn) << "\n";
    else
        std::cout << "Student deleted.\n";

    PQclear(res);
}

int main() {
    // this is used for the connection between Postpres using PGCONN environment varibale  
    // reads the connect string 
    const char* conninfo = std::getenv("PGCONN");
    if (!conninfo) {
        std::cerr << "PGCONN environment variable not set.\n";
        return 1;
    }

    // Connects to the database
    PGconn* conn = PQconnectdb(conninfo);

    // if error during connection, then the system will quit 
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection failed: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
        return 1;
    }

    // prints out menu, allowing the user to use all 4 functions with 1 exit condition as well
    int choice;
    while (true) {
        std::cout << "\n=== Student Database Menu ===\n";
        std::cout << "1. View all students\n";
        std::cout << "2. Add student\n";
        std::cout << "3. Update student email\n";
        std::cout << "4. Delete student\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        // handles all the choices the user makes
        switch (choice) {
            case 1: getAllStudents(conn); break;
            case 2: addStudent(conn); break;
            case 3: updateStudentEmail(conn); break;
            case 4: deleteStudent(conn); break;
            case 5:
                PQfinish(conn);
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
