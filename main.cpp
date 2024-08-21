// thes a new2
#include <iostream>
#include "sqlite3.h"
#include <cstdlib>
#include<map>

using namespace std;

void clearScreen() {
    system(
        #ifdef _WIN32
            "cls"
        #else
            "clear"
        #endif
    );
}

void pauseProgram() {
    cout<<"press any key to continue...";
    cin.ignore(1, '\n').get();
}


sqlite3 *DB;
class connection
{
private:
    int response = sqlite3_open("hospital.db", &DB);

public:
    int connect()
    {
        if (response)
        {
            cout << "Error open DB " << sqlite3_errmsg(DB) << endl;
        }
        return response;
    }

    void disoncect()
    {
        sqlite3_close(DB);
    }
};

class createtion
{
private:
    char *errMsg = 0;
    string query;
    connection db;
    int response = db.connect();
    map<string,string> tableQueries = {
        {"Patients", "CREATE TABLE IF NOT EXISTS Patients (PatientID INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR(255) NOT NULL, BirthDate DATE NOT NULL, Gender CHAR(1) NOT NULL, ContactNumber VARCHAR(15), Email VARCHAR(255), Address TEXT);"},
        {"Doctors", "CREATE TABLE IF NOT EXISTS Doctors (DoctorID INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR(255) NOT NULL, BirthDate DATE NOT NULL, Gender CHAR(1) NOT NULL, Specialization VARCHAR(255), ContactNumber VARCHAR(15), Email VARCHAR(255), Address TEXT);"},
        {"Appointments", "CREATE TABLE IF NOT EXISTS Appointments (AppointmentID INTEGER PRIMARY KEY AUTOINCREMENT, PatientID INT NOT NULL, DoctorID INT NOT NULL, AppointmentDate DATETIME NOT NULL, Status VARCHAR(50), CancellationReason TEXT, RescheduledDate DATETIME, Notes TEXT, FOREIGN KEY (PatientID) REFERENCES Patients (PatientID), FOREIGN KEY (DoctorID) REFERENCES Doctors (DoctorID));"},
        {"MedicalRecords", "CREATE TABLE IF NOT EXISTS MedicalRecords (RecordID INTEGER PRIMARY KEY AUTOINCREMENT, PatientID INT NOT NULL, AppointmentID INT NOT NULL, Diagnosis TEXT, Prescription TEXT, Notes TEXT, FOREIGN KEY (PatientID) REFERENCES Patients (PatientID), FOREIGN KEY (AppointmentID) REFERENCES Appointments (AppointmentID));"},
        {"Prescriptions", "CREATE TABLE IF NOT EXISTS Prescriptions (PrescriptionID INTEGER PRIMARY KEY AUTOINCREMENT, RecordID INT NOT NULL, DrugName VARCHAR(255) NOT NULL, Dosage VARCHAR(50), Frequency VARCHAR(50), StartDate DATE, EndDate DATE, SpecialInstructions TEXT, FOREIGN KEY (RecordID) REFERENCES MedicalRecords (RecordID));"},
        {"Payments", "CREATE TABLE IF NOT EXISTS Payments (PaymentID INTEGER PRIMARY KEY AUTOINCREMENT, PatientID INT NOT NULL, AppointmentID INT NOT NULL, PaymentDate DATETIME NOT NULL, Amount DECIMAL(10, 2), PaymentMethod VARCHAR(50), Notes TEXT, FOREIGN KEY (PatientID) REFERENCES Patients (PatientID), FOREIGN KEY (AppointmentID) REFERENCES Appointments (AppointmentID));"}
    };


public:
    void createDb()
    {
           for (auto& entry : tableQueries) {


        response = sqlite3_exec(DB, entry.second.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Create "+ entry.first +" Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
           }

        db.disoncect();
    }
};

class print
{
public:
    static int callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        for (int i = 0; i < argc; i++)
        {
            cout << "|" << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "-----------------------------------------------------------\n"
             << endl;
        return 0;
    }
};

class patient
{
public:
    int insertPatient()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string name, birthDate, gender, contactNumber, email, address;

        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter BirthDate (YYYY-MM-DD): ";
        getline(cin, birthDate);

        cout << "Enter Gender (M/F): ";
        getline(cin, gender);

        cout << "Enter ContactNumber: ";
        getline(cin, contactNumber);

        cout << "Enter Email: ";
        getline(cin, email);

        cout << "Enter Address: ";
        getline(cin, address);

        string query = "INSERT INTO Patients (Name, BirthDate, Gender, ContactNumber, Email, Address) "
                     "VALUES ('" +
                     name + "', '" + birthDate + "', '" + gender + "', '" +
                     contactNumber + "', '" + email + "', '" + address + "');";

        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Insert into Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
          pauseProgram();

            // system("pause");

            return -1;
        }

        cout << "Patient inserted successfully!\n"
             << endl;
        sqlite3_close(DB);
       pauseProgram();

        // system("pause");

        return 0;
    }

    int printPatients()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string query = "SELECT * FROM Patients;";

        response = sqlite3_exec(DB, query.c_str(), print::callback, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Select Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }

    int deletePatientData(int patientID)
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string query = "DELETE FROM Payments WHERE PatientID = " + to_string(patientID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Payments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM MedicalRecords WHERE PatientID = " + to_string(patientID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from MedicalRecords Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM Appointments WHERE PatientID = " + to_string(patientID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Appointments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM Patients WHERE PatientID = " + to_string(patientID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }

    int editPatient()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        int patientID;
        string name, birthDate, gender, contactNumber, email, address;
        int choice;

        cout << "Enter PatientID of the patient to edit: ";
        cin >> patientID;
        cin.ignore();

        cout << "\nSelect the field to edit:\n";
        cout << "-------------------------\n";
        cout << "1. Name\n\n";
        cout << "2. BirthDate (YYYY-MM-DD)\n\n";
        cout << "3. Gender (M/F)\n\n";
        cout << "4. ContactNumber\n\n";
        cout << "5. Email\n\n";
        cout << "6. Address\n\n";
        cout << "7. All Field To Edit\n\n";
        cout << "Enter Your Choice: ";
        cin >> choice;
        cin.ignore();

        string query;

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "Enter new Name: ";
            getline(cin, name);
            query = "UPDATE Patients SET Name = '" + name + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 2:
            clearScreen();
            cout << "Enter new BirthDate (YYYY-MM-DD): ";
            getline(cin, birthDate);
            query = "UPDATE Patients SET BirthDate = '" + birthDate + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 3:
            clearScreen();
            cout << "Enter new Gender (M/F): ";
            getline(cin, gender);
            query = "UPDATE Patients SET Gender = '" + gender + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 4:
            clearScreen();
            cout << "Enter new ContactNumber: ";
            getline(cin, contactNumber);
            query = "UPDATE Patients SET ContactNumber = '" + contactNumber + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 5:
            clearScreen();
            cout << "Enter new Email: ";
            getline(cin, email);
            query = "UPDATE Patients SET Email = '" + email + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 6:
            clearScreen();
            cout << "Enter new Address: ";
            getline(cin, address);
            query = "UPDATE Patients SET Address = '" + address + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        case 7:
            clearScreen();
            cout << "Enter new Name: ";
            getline(cin, name);
            cout << "Enter new BirthDate (YYYY-MM-DD): ";
            getline(cin, birthDate);
            cout << "Enter new Gender (M/F): ";
            getline(cin, gender);
            cout << "Enter new ContactNumber: ";
            getline(cin, contactNumber);
            cout << "Enter new Email: ";
            getline(cin, email);
            cout << "Enter new Address: ";
            getline(cin, address);
            query = "UPDATE Patients SET Name = '" + name + "' WHERE PatientID = " + to_string(patientID) + ";"
                                                                                                          "UPDATE Patients SET BirthDate = '" +
                  birthDate + "' WHERE PatientID = " + to_string(patientID) + ";"
                                                                              "UPDATE Patients SET Gender = '" +
                  gender + "' WHERE PatientID = " + to_string(patientID) + ";"
                                                                           "UPDATE Patients SET ContactNumber = '" +
                  contactNumber + "' WHERE PatientID = " + to_string(patientID) + ";"
                                                                                  "UPDATE Patients SET Email = '" +
                  email + "' WHERE PatientID = " + to_string(patientID) + ";"
                                                                          "UPDATE Patients SET Address = '" +
                  address + "' WHERE PatientID = " + to_string(patientID) + ";";
            break;
        default:
            cout << "Invalid choice." << endl;
            sqlite3_close(DB);
            return -1;
        }

        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error updating patient data: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        cout << "Patient data updated successfully!" << endl;
        sqlite3_close(DB);
        return 0;
    }

    void displayPatientOperations()
    {
        int choice;
        while (true)
        {
            clearScreen();
            cout << "\nPatient Management Menu:\n";
            cout << "-------------------------\n";
            cout << "1. Insert Patient\n\n";
            cout << "2. Print Patients\n\n";
            cout << "3. Delete Patient\n\n";
            cout << "4. Update Patient\n\n";
            cout << "0. Return to Main Menu\n\n";
            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                clearScreen();
                insertPatient();
                break;
            case 2:
                clearScreen();
                printPatients();
                cout << "\n\n";
                pauseProgram();
                break;
            case 3:
                int patientID;
                clearScreen();
                cout << "Enter Patient ID to Delete: ";
                cin >> patientID;
                deletePatientData(patientID);
                cout << "Patient deleted successfully.\n\n";
                pauseProgram();
                break;
            case 4:
                clearScreen();
                editPatient();
                cout << "\n\n";
                pauseProgram();
                break;
            case 0:
                clearScreen();
                return;
            default:
                cout << "Invalid choice. Press Enter and try again.\n\n";
                pauseProgram();
                break;
            }
        }
    }
};

class doctor
{
public:
    int insertDoctors()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string name, birthDate, gender, Specialization, contactNumber, email, address;

        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter BirthDate (YYYY-MM-DD): ";
        getline(cin, birthDate);

        cout << "Enter Gender (M/F): ";
        getline(cin, gender);

        cout << "Enter Specialization: ";
        getline(cin, Specialization);

        cout << "Enter ContactNumber: ";
        getline(cin, contactNumber);

        cout << "Enter Email: ";
        getline(cin, email);

        cout << "Enter Address: ";
        getline(cin, address);

        string query = "INSERT INTO Doctors (Name, BirthDate, Gender, Specialization, ContactNumber, Email, Address) "
                     "VALUES ('" +
                     name + "' , '" +
                     birthDate + "' , '" +
                     gender + "' , '" +
                     Specialization + "' , '" +
                     contactNumber + "' , '" +
                     email + "' , '" +
                     address + "');";

        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Insert into Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            pauseProgram();
            return -1;
        }

        cout << "Doctors inserted successfully!\n"
             << endl;
        sqlite3_close(DB);
        pauseProgram();
        return 0;
    }

    int printDoctors()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string query = "SELECT * FROM Doctors;";

        response = sqlite3_exec(DB, query.c_str(), print::callback, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Select Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }

    int deleteDoctorsData(int DoctorsID)
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string query = "DELETE FROM Doctors WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM MedicalRecords WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from MedicalRecords Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM Appointments WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Appointments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        query = "DELETE FROM Doctors WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error Delete from Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }

    int editdoctor()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int response = sqlite3_open("hospital.db", &DB);

        if (response)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        int DoctorsID;
        string name, birthDate, gender, Specialization, contactNumber, email, address;
        int choice;

        cout << "Enter doctorID of the patient to edit: ";
        cin >> DoctorsID;
        cin.ignore();

        cout << "\nSelect the field to edit:\n";
        cout << "-------------------------\n";
        cout << "1. Name\n\n";
        cout << "2. BirthDate (YYYY-MM-DD)\n\n";
        cout << "3. Gender (M/F)\n\n";
        cout << "4. Specialization\n\n";
        cout << "5. ContactNumber\n\n";
        cout << "6. Email\n\n";
        cout << "7. Address\n\n";
        cout << "8. All Field To Edit\n\n";
        cout << "Enter Your Choice: ";
        cin >> choice;
        cin.ignore();

        string query;

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "Enter new Name: ";
            getline(cin, name);
            query = "UPDATE Doctors SET Name = '" + name + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 2:
            clearScreen();
            cout << "Enter new BirthDate (YYYY-MM-DD): ";
            getline(cin, birthDate);
            query = "UPDATE Doctors SET BirthDate = '" + birthDate + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 3:
            clearScreen();
            cout << "Enter new Gender (M/F): ";
            getline(cin, gender);
            query = "UPDATE Doctors SET Gender = '" + gender + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 4:
            clearScreen();
            cout << "Enter new Specialization: ";
            getline(cin, gender);
            query = "UPDATE Doctors SET Specialization = '" + Specialization + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 5:
            clearScreen();
            cout << "Enter new ContactNumber: ";
            getline(cin, contactNumber);
            query = "UPDATE Doctors SET ContactNumber = '" + contactNumber + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 6:
            clearScreen();
            cout << "Enter new Email: ";
            getline(cin, email);
            query = "UPDATE Doctors SET Email = '" + email + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 7:
            clearScreen();
            cout << "Enter new Address: ";
            getline(cin, address);
            query = "UPDATE Doctors SET Address = '" + address + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        case 8:
            clearScreen();
            cout << "Enter new Name: ";
            getline(cin, name);
            cout << "Enter new BirthDate (YYYY-MM-DD): ";
            getline(cin, birthDate);
            cout << "Enter new Gender (M/F): ";
            getline(cin, gender);
            cout << "Enter new ContactNumber: ";
            getline(cin, contactNumber);
            cout << "Enter new Email: ";
            getline(cin, email);
            cout << "Enter new Address: ";
            getline(cin, address);
            query = "UPDATE Doctors SET Name = '" + name + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                                                        "UPDATE Doctors SET BirthDate = '" +
                  birthDate + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                             "UPDATE Doctors SET Gender = '" +
                  gender + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                          "UPDATE Doctors SET Specialization = '" +
                  Specialization + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                                  "UPDATE Doctors SET ContactNumber = '" +
                  contactNumber + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                                 "UPDATE Doctors SET Email = '" +
                  email + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                                                                         "UPDATE Doctors SET Address = '" +
                  address + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
            break;
        default:
            cout << "Invalid choice." << endl;
            sqlite3_close(DB);
            return -1;
        }

        response = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMsg);
        if (response != SQLITE_OK)
        {
            cerr << "Error updating Doctor data: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        cout << "Doctor data updated successfully!" << endl;
        sqlite3_close(DB);
        return 0;
    }

    void displayDoctorsOperations()
    {
        int choice;
        while (true)
        {
            clearScreen();
            cout << "\nDoctors Management Menu:\n";
            cout << "-------------------------\n";
            cout << "1. Insert Doctor\n\n";
            cout << "2. Print Doctors\n\n";
            cout << "3. Delete Doctor\n\n";
            cout << "4. Update Doctor\n\n";
            cout << "0. Return to Main Menu\n\n";
            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                clearScreen();
                insertDoctors();
                break;
            case 2:
                clearScreen();
                printDoctors();
                cout << "\n\n";
                pauseProgram();
                break;
            case 3:
                int DoctorsID;
                clearScreen();
                cout << "Enter Doctors ID to Delete: ";
                cin >> DoctorsID;
                deleteDoctorsData(DoctorsID);
                cout << "Doctors deleted successfully.\n\n";
                pauseProgram();
                break;
            case 4:
                clearScreen();
                editdoctor();
                cout << "\n\n";
                pauseProgram();
                break;
            case 0:
                clearScreen();
                return;
            default:
                cout << "Invalid choice. Press Enter and try again.\n\n";
                pauseProgram();
                break;
            }
        }
    }
};

void main_menu()
{
    cout << "\nHospital Management System\n";
    cout << "-----------------------------\n";
    cout << "1. Manage Patients\n\n";
    cout << "2. Manage Doctors\n\n";
    cout << "3. Manage Appointments\n\n";
    cout << "4. Manage Medical Records\n\n";
    cout << "5. Manage Prescriptions\n\n";
    cout << "6. Manage Payments\n\n";
    cout << "0. Exit\n\n";
    cout << "Enter Your Choice: ";
}

void handle()
{
    int choice;
    patient p;
    doctor d;
    while (true)
    {
        clearScreen();
        main_menu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            p.displayPatientOperations();
            break;
        case 2:
            d.displayDoctorsOperations();
            break;
        case 0:
            cout << "Exiting the system..." << endl;
            return;
        default:
            cout << "Invalid choice. Press Enter and try again.\n\n";
             pauseProgram();
            break;
        }
    }
}

int main()
{
    createtion DB;
    DB.createDb();
    handle();
    return 0;
}
