# Remote Sensing Communication CanSat 🛰️

**2nd Place — 10th International Iran CanSat Competition 2024**  
**Best Project of the Year — SUPARCO Institute of Technical Training**

The Remote Sensing Communication CanSat was developed as the final-year project for the Diploma of Associate Engineering (Electronics) program at SUPARCO Institute of Technical Training. The team built a can-sized satellite simulation model for the 2024 international competition.

The repository contains project documentation, firmware, Altium PCB design files, subsystem diagrams and build photographs.

> **Project attribution:** this was a team project. The role described below is Qaisar Khan's confirmed contribution, not a claim that one person completed every subsystem.

## My role and contributions

I served as **Project Lead**. My confirmed work included:

- Coordinating the project work and supporting the team.
- Electronics hardware work.
- Component stuffing and soldering.
- Hardware assembly and testing.
- Power-subsystem calculations, design and simulations.
- Sensor-subsystem coding, interfacing and testing.
- Preparing project documentation.

I am not using this README to claim sole ownership of all firmware, ground-station, mechanical or communication work. The repository contains contributions and artifacts from the wider project team.

## Competition recognition

- **2nd Place**, Remote Sensing-Communication class, 10th International Iran CanSat Competition 2024, organized by the Aerospace Research Institute, Ministry of Science, Research and Technology.
- **Best Project of the Year**, SUPARCO Institute of Technical Training.

Award and team certificate scans are stored in [docs/certificates](docs/certificates).

## System overview

The project documentation describes a CanSat platform with sensing, telemetry, imaging and recovery subsystems. The documented hardware includes:

| Subsystem | Components / implementation documented in this repository |
|---|---|
| Main controller | ESP32 family boards |
| Sensors | BMP280 pressure / temperature sensor; MPU6050 inertial sensor |
| Position | NEO-6M GPS module |
| Imaging payload | ESP32-CAM |
| Telemetry | LoRa SX1278 radio link |
| Power | Battery and regulated power subsystem |
| Recovery | Parachute-based descent system |

Refer to the design reviews and final report for detailed design assumptions, wiring, calculations and competition constraints. Component configuration and test results should be read from the relevant project documents rather than inferred from this summary.

### Subsystem diagrams

The repository includes diagrams for the full system and individual subsystems:

- [Overall subsystem layout](hardware/subsystem-schematics/SUBSYSTEMS.png)
- [Power subsystem](hardware/subsystem-schematics/power.png)
- [Communication subsystem](hardware/subsystem-schematics/COMM.png)
- [Sensor subsystem](hardware/subsystem-schematics/SENSOR.png)
- [Camera subsystem](hardware/subsystem-schematics/CAMERA.png)
- [Ground-station diagram](hardware/subsystem-schematics/GCS.png)
- [Recovery subsystem](hardware/subsystem-schematics/recovery.png)
- [Structure diagram](hardware/subsystem-schematics/Structure.png)

## Firmware and hardware repository map

~~~text
remote-sensing-communication-cansat/
├── firmware/
│   ├── ESP32_SUPERMINI/
│   │   ├── ESP32_SUPERMINI.ino
│   │   ├── Sensors.cpp
│   │   └── Sensors.h
│   ├── ESP32_CAM/
│   │   ├── ESP32_CAM.ino
│   │   └── camera_pins.h
│   └── GCS/
│       └── GCS.ino
├── hardware/
│   ├── pcb/
│   │   ├── CANSAT_COMMUNICATION/
│   │   ├── CANSAT_CONTROLLER/
│   │   ├── CANSAT_EPS/
│   │   └── CANSAT_SENSOR/
│   └── subsystem-schematics/
├── docs/
│   ├── PDR.docx
│   ├── CDR.docx
│   ├── Final_Project_Report.pdf
│   ├── Competition_Guidelines.pdf
│   └── certificates/
├── Cansat final images/
└── README.md
~~~

### Firmware directories

- **ESP32_SUPERMINI:** sensor and telemetry-oriented firmware, with shared sensor routines in Sensors.cpp / Sensors.h.
- **ESP32_CAM:** camera capture and image-file handling code.
- **GCS:** LoRa receiver / serial-output sketch.

These files document the team implementation. Their presence in the repository does not mean every part was individually authored by the project lead.

### Hardware directories

The hardware/pcb folder stores the Altium project files and related design outputs for the communication, controller, electrical power and sensor boards.

## Documentation

- [Preliminary Design Review (PDR)](docs/PDR.docx)
- [Critical Design Review (CDR)](docs/CDR.docx)
- [Final Project Report](docs/Final_Project_Report.pdf)
- [Competition Guidelines](docs/Competition_Guidelines.pdf)
- [Award certificates](docs/certificates/)

## Build photographs

Selected photographs from the final CanSat build are stored in the [Cansat final images](Cansat%20final%20images/) folder.

## Security note

The current firmware files have been cleaned of unused hard-coded Wi-Fi credential declarations. Earlier repository history may still contain the previous values. If those credentials were ever used on real hardware, rotate them. Removing a secret from the latest commit does not erase it from earlier Git commits; a repository-history rewrite would be required to purge historical copies.

Do not commit passwords, API keys, private network credentials or other secrets in future firmware updates.

## Tools and technologies

ESP32, Arduino-style C/C++, BMP280, MPU6050, GPS interfacing, LoRa, ESP32-CAM, Altium Designer, PCB assembly, soldering, electronics testing and subsystem documentation.

---

This project was completed by the SITTians team as part of the Electronics diploma program at SUPARCO Institute of Technical Training and submitted for the 10th International Iran CanSat Competition 2024.
