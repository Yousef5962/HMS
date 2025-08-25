#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department
{
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType
{
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //hjth
class Patient
{
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c)
    {
        id = pid;
        name = n;
        age = a;
        contact = c;
        // other default values :
        isAdmitted = false;
        roomType = GENERAL_WARD;
    }

    void admitPatient(RoomType type)
    {
        isAdmitted = true;
        roomType = type;
        string log = "Patient: " + name + " --> has been admitted to room type: " + to_string(type);
        medicalHistory.push(log);
        cout << log << endl;
    }

    void dischargePatient()
    {
        isAdmitted = false;
        string log = "Patient: " + name + " --> has been discharged ";
        medicalHistory.push(log);
        cout << log << endl;
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }


    void requestTest(string testName){
        cout<<name<<"has requested test:"<<testName<<endl;
        testQueue.push(testName);

    }

    string performTest()
    {
        string result = "test completed successfully";
        cout << result << endl;
        return result;
    }

    void displayHistory()
    {
        cout << "Medical history for " << name << ":\n";
        stack<string> temp = medicalHistory;
        while (!temp.empty())
        {

            cout << "- " << temp.top() << endl;
            temp.pop();
        }
    }

    int getId()
    {
        return id;
    }


    string getName()
    {
        return name;
    }

    bool getAdmissionStatus()
    {
        return isAdmitted;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d)
    {
        id = did;
        name = n;
        department = d;
    }

    void addAppointment(int patientId)
    {
        appointmentQueue.push(patientId);
    }

    int seePatient()
    {
        if (appointmentQueue.empty())
        {
            return -1;
        }
        int patient = appointmentQueue.front();
        appointmentQueue.pop();
        return patient;
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }

    string getDepartment()
    {
        switch (department)
        {
        case CARDIOLOGY:
            return "CARDIOLOGY";
        case NEUROLOGY:
            return "NEUROLOGY";
        case ORTHOPEDICS:
            return "ORTHOPEDICS";
        case PEDIATRICS:
            return "PEDIATRICS";
        case GENERAL:
            return "GENERAL";
        default:
            return " ";
        }
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital
{
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital();
    int registerPatient(string name, int age, string contact);
    int addDoctor(string name, Department dept);
    void displayCounters() const;

    void admitPatient(int patientId, RoomType type)
    {
        for (auto &patient : patients)
        {
            if (patient.getId() == patientId)
            {
                if (patient.getAdmissionStatus())
                {
                    cout << "Patient " << patient.getName() << " is already admitted." << endl;
                }
                else
                {
                    patient.admitPatient(type);
                }
                return;
            }
        }
        cout << "Admit Failed: Patient with ID " << patientId << " not found." << endl;
    }

    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
        cout << "Emergency case added for patient ID: " << patientId << endl;
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            cout << "No emergency cases to handle." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();

        Patient *patient = nullptr;
        for (int i = 0; i < patients.size(); ++i)
        {
            if (patients[i].getId() == patientId)
            {
                patient = &patients[i];
                break;
            }
        }

        if (patient)
        {
            cout << "Emergency handled for patient: " << patient->getName() << endl;
        }
        else
        {
            cout << "Emergency case for unknown patient ID: " << patientId << endl;
        }

        return patientId;
    }

    void bookAppointment(int doctorId, int patientId)
    {
        Doctor *doctor = nullptr;
        Patient *patient = nullptr;

        for (int i = 0; i < doctors.size(); ++i)
        {
            if (doctors[i].getId() == doctorId)
            {
                doctor = &doctors[i];
                break;
            }
        }

        for (int i = 0; i < patients.size(); ++i)
        {
            if (patients[i].getId() == patientId)
            {
                patient = &patients[i];
                break;
            }
        }

        if (!doctor)
        {
            cout << "Booking Failed: Doctor with ID " << doctorId << " not found." << endl;
            return;
        }
        if (!patient)
        {
            cout << "Booking Failed: Patient with ID " << patientId << " not found." << endl;
            return;
        }

        doctor->addAppointment(patientId);
        cout << "Appointment booked: Patient " << patient->getName()
             << " with Doctor " << doctor->getName() << endl;
    }

    void displayPatientInfo(int patientId)
    {
        Patient *patient = nullptr;
        for (int i = 0; i < patients.size(); ++i)
        {
            if (patients[i].getId() == patientId)
            {
                patient = &patients[i];
                break;
            }
        }

        if (patient)
        {
            cout << "\n===== Patient Information =====" << endl;
            cout << "ID: " << patient->getId() << endl;
            cout << "Name: " << patient->getName() << endl;
            cout << "Admission Status: " << (patient->getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
            patient->displayHistory();
            cout << "=============================\n"
                 << endl;
        }
        else
        {
            cout << "Display Failed: Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayDoctorInfo(int doctorId)
    {
        Doctor *doctor = nullptr;
        for (int i = 0; i < doctors.size(); ++i)
        {
            if (doctors[i].getId() == doctorId)
            {
                doctor = &doctors[i];
                break;
            }
        }
        if (doctor)
        {
            cout << "\n===== Doctor Information =====" << endl;
            cout << "ID: " << doctor->getId() << endl;
            cout << "Name: " << doctor->getName() << endl;
            cout << "Department: " << doctor->getDepartment() << endl;
            cout << "==========================\n"
                 << endl;
        }
        else
        {
            cout << "Display Failed: Doctor with ID " << doctorId << " not found." << endl;
        }
    }
};
Hospital::Hospital()
{
    patientCounter = 1;
    doctorCounter = 1;
}

int Hospital::registerPatient(string name, int age, string contact)
{
    Patient newPatient(patientCounter, name, age, contact);
    patients.push_back(newPatient);
    cout << "Patient registered with ID: " << patientCounter << endl;
    return patientCounter++;
}

int Hospital::addDoctor(string name, Department dept)
{
    Doctor newDoctor(doctorCounter, name, dept);
    doctors.push_back(newDoctor);
    cout << "Doctor added with ID: " << doctorCounter << endl;
    return doctorCounter++;
}

void Hospital::displayCounters() const
{
    cout << "Patient Counter: " << patientCounter << endl;
    cout << "Doctor Counter: " << doctorCounter << endl;
}

// ========== MAIN PROGRAM ========== //
int main()
{
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
