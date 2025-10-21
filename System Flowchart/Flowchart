## ⚙️ System Flowchart

```mermaid
flowchart TD
A[Start] --> B[Initialize System & RTC]
B --> C[Check Current Time]
C --> D{Is it Operating Time?}
D -->|Yes| E[Activate Motors & Conveyor]
E --> F[Check for Obstacles via Ultrasonic Sensor]
F -->|Obstacle Detected| G[Adjust Direction]
F -->|No Obstacle| H[Continue Cleaning]
G --> H
H --> I{Operation Time Over?}
I -->|Yes| J[Stop All Motors & Conveyor]
I -->|No| E
J --> K[Activate Buzzer]
K --> L[Display Status on LCD]
L --> M[Go to Idle / Wait for Next Cycle]
D -->|No| M
M --> C
