#include <iostream>
#include "sqlite3.h"
#include <cstdlib>

using namespace std;


class connection {
    private:
      sqlite3* DB;
      char* errMsg = 0;
      string sql;
      int exit = sqlite3_open("hospital.db", &DB);

    public:
      void connect(){
      if(exit) {
        cout<< "Error open DB " << sqlite3_errmsg(DB) << endl;
        return;
        }
      }

      void disoncect(){
        sqlite3_close(DB);
      }

      void createDb(){

        connect();

        sql = "CREATE TABLE IF NOT EXISTS Patients ("
              "PatientID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "Name VARCHAR(255) NOT NULL, "
              "BirthDate DATE NOT NULL, "
              "Gender CHAR(1) NOT NULL, "
              "ContactNumber VARCHAR(15), "
              "Email VARCHAR(255), "
              "Address TEXT);";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        sql = "CREATE TABLE IF NOT EXISTS Doctors ("
              "DoctorID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "Name VARCHAR(255) NOT NULL, "
              "BirthDate DATE NOT NULL, "
              "Gender CHAR(1) NOT NULL, "
              "Specialization VARCHAR(255), "
              "ContactNumber VARCHAR(15), "
              "Email VARCHAR(255), "
              "Address TEXT);";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        sql = "CREATE TABLE IF NOT EXISTS Appointments ("
              "AppointmentID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "PatientID INT NOT NULL, "
              "DoctorID INT NOT NULL, "
              "AppointmentDate DATETIME NOT NULL, "
              "Status VARCHAR(50), "
              "CancellationReason TEXT, "
              "RescheduledDate DATETIME, "
              "Notes TEXT, "
              "FOREIGN KEY(PatientID) REFERENCES Patients(PatientID), "
              "FOREIGN KEY(DoctorID) REFERENCES Doctors(DoctorID));";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create Appointments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        sql = "CREATE TABLE IF NOT EXISTS MedicalRecords ("
              "RecordID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "PatientID INT NOT NULL, "
              "AppointmentID INT NOT NULL, "
              "Diagnosis TEXT, "
              "Prescription TEXT, "
              "Notes TEXT, "
              "FOREIGN KEY(PatientID) REFERENCES Patients(PatientID), "
              "FOREIGN KEY(AppointmentID) REFERENCES Appointments(AppointmentID));";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create MedicalRecords Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        sql = "CREATE TABLE IF NOT EXISTS Prescriptions ("
              "PrescriptionID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "RecordID INT NOT NULL, "
              "DrugName VARCHAR(255) NOT NULL, "
              "Dosage VARCHAR(50), "
              "Frequency VARCHAR(50), "
              "StartDate DATE, "
              "EndDate DATE, "
              "SpecialInstructions TEXT, "
              "FOREIGN KEY(RecordID) REFERENCES MedicalRecords(RecordID));";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create Prescriptions Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        sql = "CREATE TABLE IF NOT EXISTS Payments ("
              "PaymentID INTEGER PRIMARY KEY AUTOINCREMENT, "
              "PatientID INT NOT NULL, "
              "AppointmentID INT NOT NULL, "
              "PaymentDate DATETIME NOT NULL, "
              "Amount DECIMAL(10,2), "
              "PaymentMethod VARCHAR(50), "
              "Notes TEXT, "
              "FOREIGN KEY(PatientID) REFERENCES Patients(PatientID), "
              "FOREIGN KEY(AppointmentID) REFERENCES Appointments(AppointmentID));";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Create Payments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }

        disoncect();
        }
};

class print {
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
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
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

        string sql = "INSERT INTO Patients (Name, BirthDate, Gender, ContactNumber, Email, Address) "
                     "VALUES ('" + name + "', '" + birthDate + "', '" + gender + "', '" +
                     contactNumber + "', '" + email + "', '" + address + "');";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Insert into Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            system("pause");
            return -1;
        }

        cout << "Patient inserted successfully!\n" << endl;
        sqlite3_close(DB);
        system("pause");
        return 0;
    }


    int printPatients()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string sql = "SELECT * FROM Patients;";

        exit = sqlite3_exec(DB, sql.c_str(), print::callback, 0, &errMsg);
        if (exit != SQLITE_OK)
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
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string sql = "DELETE FROM Payments WHERE PatientID = " + to_string(patientID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Payments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM MedicalRecords WHERE PatientID = " + to_string(patientID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from MedicalRecords Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM Appointments WHERE PatientID = " + to_string(patientID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Appointments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM Patients WHERE PatientID = " + to_string(patientID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Patients Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }


    int editPatient() {
        sqlite3* DB;
        char* errMsg = 0;
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit) {
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

        string sql;

        switch (choice) {
            case 1:
                system("cls");
                cout << "Enter new Name: ";
                getline(cin, name);
                sql = "UPDATE Patients SET Name = '" + name + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 2:
                system("cls");
                cout << "Enter new BirthDate (YYYY-MM-DD): ";
                getline(cin, birthDate);
                sql = "UPDATE Patients SET BirthDate = '" + birthDate + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 3:
                system("cls");
                cout << "Enter new Gender (M/F): ";
                getline(cin, gender);
                sql = "UPDATE Patients SET Gender = '" + gender + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 4:
                system("cls");
                cout << "Enter new ContactNumber: ";
                getline(cin, contactNumber);
                sql = "UPDATE Patients SET ContactNumber = '" + contactNumber + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 5:
                system("cls");
                cout << "Enter new Email: ";
                getline(cin, email);
                sql = "UPDATE Patients SET Email = '" + email + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 6:
                system("cls");
                cout << "Enter new Address: ";
                getline(cin, address);
                sql = "UPDATE Patients SET Address = '" + address + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            case 7:
                system("cls");
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
                sql = "UPDATE Patients SET Name = '" + name + "' WHERE PatientID = " + to_string(patientID) + ";"
                      "UPDATE Patients SET BirthDate = '" + birthDate + "' WHERE PatientID = " + to_string(patientID) + ";"
                      "UPDATE Patients SET Gender = '" + gender + "' WHERE PatientID = " + to_string(patientID) + ";"
                      "UPDATE Patients SET ContactNumber = '" + contactNumber + "' WHERE PatientID = " + to_string(patientID) + ";"
                      "UPDATE Patients SET Email = '" + email + "' WHERE PatientID = " + to_string(patientID) + ";"
                      "UPDATE Patients SET Address = '" + address + "' WHERE PatientID = " + to_string(patientID) + ";";
                break;
            default:
                cout << "Invalid choice." << endl;
                sqlite3_close(DB);
                return -1;
        }

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK) {
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
            system("cls");
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
                system("cls");
                insertPatient();
                break;
            case 2:
                system("cls");
                printPatients();
                cout << "\n\n";
                system("pause");
                break;
            case 3:
                int patientID;
                system("cls");
                cout << "Enter Patient ID to Delete: ";
                cin >> patientID;
                deletePatientData(patientID);
                cout << "Patient deleted successfully.\n\n";
                system("pause");
                break;
            case 4:
                system("cls");
                editPatient();
                cout<<"\n\n";
                system("pause");
                break;
            case 0:
                system("cls");
                return;
            default:
                cout << "Invalid choice. Press Enter and try again.\n\n";
                system("pause");
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
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
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

        string sql = "INSERT INTO Doctors (Name, BirthDate, Gender, Specialization, ContactNumber, Email, Address) "
                     "VALUES ('" + name + "' , '" +
                                   birthDate + "' , '" +
                                   gender + "' , '" +
                                   Specialization + "' , '" +
                                   contactNumber + "' , '" +
                                   email + "' , '" +
                                   address + "');";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Insert into Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            system("pause");
            return -1;
        }

        cout << "Doctors inserted successfully!\n" << endl;
        sqlite3_close(DB);
        system("pause");
        return 0;
    }


    int printDoctors()
    {
        sqlite3 *DB;
        char *errMsg = 0;
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string sql = "SELECT * FROM Doctors;";

        exit = sqlite3_exec(DB, sql.c_str(), print::callback, 0, &errMsg);
        if (exit != SQLITE_OK)
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
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit)
        {
            cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
            return -1;
        }

        string sql = "DELETE FROM Doctors WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM MedicalRecords WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from MedicalRecords Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM Appointments WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Appointments Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sql = "DELETE FROM Doctors WHERE DoctorsID = " + to_string(DoctorsID) + ";";
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK)
        {
            cerr << "Error Delete from Doctors Table: " << errMsg << endl;
            sqlite3_free(errMsg);
            sqlite3_close(DB);
            return -1;
        }

        sqlite3_close(DB);
        return 0;
    }


    int editdoctor() {
        sqlite3* DB;
        char* errMsg = 0;
        int exit = sqlite3_open("hospital.db", &DB);

        if (exit) {
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

        string sql;

        switch (choice) {
            case 1:
                system("cls");
                cout << "Enter new Name: ";
                getline(cin, name);
                sql = "UPDATE Doctors SET Name = '" + name + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 2:
                system("cls");
                cout << "Enter new BirthDate (YYYY-MM-DD): ";
                getline(cin, birthDate);
                sql = "UPDATE Doctors SET BirthDate = '" + birthDate + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 3:
                system("cls");
                cout << "Enter new Gender (M/F): ";
                getline(cin, gender);
                sql = "UPDATE Doctors SET Gender = '" + gender + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 4:
                system("cls");
                cout << "Enter new Specialization: ";
                getline(cin, gender);
                sql = "UPDATE Doctors SET Specialization = '" + Specialization + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 5:
                system("cls");
                cout << "Enter new ContactNumber: ";
                getline(cin, contactNumber);
                sql = "UPDATE Doctors SET ContactNumber = '" + contactNumber + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 6:
                system("cls");
                cout << "Enter new Email: ";
                getline(cin, email);
                sql = "UPDATE Doctors SET Email = '" + email + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 7:
                system("cls");
                cout << "Enter new Address: ";
                getline(cin, address);
                sql = "UPDATE Doctors SET Address = '" + address + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            case 8:
                system("cls");
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
                sql = "UPDATE Doctors SET Name = '" + name + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET BirthDate = '" + birthDate + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET Gender = '" + gender + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET Specialization = '" + Specialization + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET ContactNumber = '" + contactNumber + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET Email = '" + email + "' WHERE DoctorID = " + to_string(DoctorsID) + ";"
                      "UPDATE Doctors SET Address = '" + address + "' WHERE DoctorID = " + to_string(DoctorsID) + ";";
                break;
            default:
                cout << "Invalid choice." << endl;
                sqlite3_close(DB);
                return -1;
        }

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (exit != SQLITE_OK) {
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
            system("cls");
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
                system("cls");
                insertDoctors();
                break;
            case 2:
                system("cls");
                printDoctors();
                cout << "\n\n";
                system("pause");
                break;
            case 3:
                int DoctorsID;
                system("cls");
                cout << "Enter Doctors ID to Delete: ";
                cin >> DoctorsID;
                deleteDoctorsData(DoctorsID);
                cout << "Doctors deleted successfully.\n\n";
                system("pause");
                break;
            case 4:
                system("cls");
                editdoctor();
                cout<<"\n\n";
                system("pause");
                break;
            case 0:
                system("cls");
                return;
            default:
                cout << "Invalid choice. Press Enter and try again.\n\n";
                system("pause");
                break;
            }
        }
    }
};




void menu() {
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

void handle(){
    int choice;
    patient p;
    doctor d;
    while (true)
    {
        system("cls");
        menu();
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
            system("pause");
            break;
        }
    }
}

int main()
{
    connection db ;
    db.connect();
    db.createDb();
    db.disoncect();
    handle();
    return 0;
}
