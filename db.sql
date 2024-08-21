--! here we will save the queries that we used
--the Patients table creation query
CREATE TABLE
      IF NOT EXISTS Patients (
            PatientID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name VARCHAR(255) NOT NULL,
            BirthDate DATE NOT NULL,
            Gender CHAR(1) NOT NULL,
            ContactNumber VARCHAR(15),
            Email VARCHAR(255),
            Address TEXT
      );

--the Doctors table creation query
CREATE TABLE
      IF NOT EXISTS Doctors (
            DoctorID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name VARCHAR(255) NOT NULL,
            BirthDate DATE NOT NULL,
            Gender CHAR(1) NOT NULL,
            Specialization VARCHAR(255),
            ContactNumber VARCHAR(15),
            Email VARCHAR(255),
            Address TEXT
      );

--the Appointments table creation query
CREATE TABLE
      IF NOT EXISTS Appointments (
            AppointmentID INTEGER PRIMARY KEY AUTOINCREMENT,
            PatientID INT NOT NULL,
            DoctorID INT NOT NULL,
            AppointmentDate DATETIME NOT NULL,
            Status VARCHAR(50),
            CancellationReason TEXT,
            RescheduledDate DATETIME,
            Notes TEXT,
            FOREIGN KEY (PatientID) REFERENCES Patients (PatientID),
            FOREIGN KEY (DoctorID) REFERENCES Doctors (DoctorID)
      );

--the MedicalRecords table creation query
CREATE TABLE
      IF NOT EXISTS MedicalRecords (
            RecordID INTEGER PRIMARY KEY AUTOINCREMENT,
            PatientID INT NOT NULL,
            AppointmentID INT NOT NULL,
            Diagnosis TEXT,
            Prescription TEXT,
            Notes TEXT,
            FOREIGN KEY (PatientID) REFERENCES Patients (PatientID),
            FOREIGN KEY (AppointmentID) REFERENCES Appointments (AppointmentID)
      );

--the Prescriptions table creation query
CREATE TABLE
      IF NOT EXISTS Prescriptions (
            PrescriptionID INTEGER PRIMARY KEY AUTOINCREMENT,
            RecordID INT NOT NULL,
            DrugName VARCHAR(255) NOT NULL,
            Dosage VARCHAR(50),
            Frequency VARCHAR(50),
            StartDate DATE,
            EndDate DATE,
            SpecialInstructions TEXT,
            FOREIGN KEY (RecordID) REFERENCES MedicalRecords (RecordID)
      );

--the Payments table creation query
CREATE TABLE
      IF NOT EXISTS Payments (
            PaymentID INTEGER PRIMARY KEY AUTOINCREMENT,
            PatientID INT NOT NULL,
            AppointmentID INT NOT NULL,
            PaymentDate DATETIME NOT NULL,
            Amount DECIMAL(10, 2),
            PaymentMethod VARCHAR(50),
            Notes TEXT,
            FOREIGN KEY (PatientID) REFERENCES Patients (PatientID),
            FOREIGN KEY (AppointmentID) REFERENCES Appointments (AppointmentID)
      );