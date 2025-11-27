#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* ============================
   Traffic Light States
   ============================ */
typedef enum {
    RED,
    YELLOW,
    GREEN
} LightState;

/* ============================
   Traffic Lane Structure
   ============================ */
typedef struct {
    int laneID;
    int vehicleCount;
    int vehiclesProcessed;
    int greenTime;
    int yellowTime;
    int redTime;
    LightState state;
    time_t lastUpdate;
} Lane;


/* ============================
   Logging Function
   ============================ */
void logToFile(Lane lanes[], int laneCount, int currentTimeSec) {
    FILE *fp = fopen("traffic_log.txt", "a");

    if (!fp) {
        printf("ERROR: Unable to open log file!\n");
        return;
    }

    fprintf(fp, "===== Cycle %d (%ld) =====\n",
            currentTimeSec, time(NULL));

    for (int i = 0; i < laneCount; i++) {
        const char *stateStr =
            (lanes[i].state == GREEN)  ? "GREEN"  :
            (lanes[i].state == YELLOW) ? "YELLOW" : "RED";

        fprintf(fp,
                "Lane %d | State: %-6s | Waiting: %d | Processed: %d | "
                "G=%ds Y=%ds R=%ds\n",
                lanes[i].laneID,
                stateStr,
                lanes[i].vehicleCount,
                lanes[i].vehiclesProcessed,
                lanes[i].greenTime,
                lanes[i].yellowTime,
                lanes[i].redTime);
    }

    fprintf(fp, "\n");
    fclose(fp);
}


/* ============================
   Initialize Lane
   ============================ */
void initializeLane(Lane *lane, int id, int vehicles) {
    lane->laneID = id;
    lane->vehicleCount = vehicles;
    lane->vehiclesProcessed = 0;

    lane->greenTime = 2 + vehicles;
    if (lane->greenTime > 5)
        lane->greenTime = 5;

    lane->yellowTime = 2;
    lane->redTime   = 2;

    lane->state = RED;
    lane->lastUpdate = time(NULL);
}


/* ============================
   Update Traffic Signals
   ============================ */
void updateSignals(Lane lanes[], int laneCount) {
    time_t current = time(NULL);

    for (int i = 0; i < laneCount; i++) {
        Lane *L = &lanes[i];
        int elapsed = current - L->lastUpdate;

        switch (L->state) {
            case GREEN:
                if (elapsed >= L->greenTime) {
                    L->state = YELLOW;
                    L->lastUpdate = current;
                }
                break;

            case YELLOW:
                if (elapsed >= L->yellowTime) {
                    L->state = RED;
                    L->lastUpdate = current;
                }
                break;

            case RED:
                if (elapsed >= L->redTime) {

                    int allRed = 1;
                    for (int j = 0; j < laneCount; j++) {
                        if (j != i && lanes[j].state == GREEN) {
                            allRed = 0;
                            break;
                        }
                    }

                    if (allRed) {
                        L->state = GREEN;
                        L->vehiclesProcessed += L->vehicleCount;
                        L->vehicleCount = 0;
                        L->lastUpdate = current;
                    }
                }
                break;
        }
    }
}


/* ============================
   Display Lane
   ============================ */
void printLane(const Lane *L) {
    const char *stateStr =
        (L->state == GREEN)  ? "GREEN"  :
        (L->state == YELLOW) ? "YELLOW" : "RED";

    printf("  Lane %d: %-6s | vehicles = %d\n",
           L->laneID, stateStr, L->vehicleCount);
}


/* ============================
   Traffic Stats
   ============================ */
void printStats(Lane lanes[], int size) {
    printf("\n=== Traffic Statistics ===\n");
    for (int i = 0; i < size; i++) {
        printf("Lane %d:\n", lanes[i].laneID);
        printf("  Vehicles waiting     : %d\n", lanes[i].vehicleCount);
        printf("  Vehicles processed   : %d\n", lanes[i].vehiclesProcessed);
        printf("  Current Light        : %s\n",
               (lanes[i].state == GREEN) ? "GREEN" :
               (lanes[i].state == YELLOW) ? "YELLOW" : "RED");
    }
    printf("==========================\n\n");
}


/* ============================
   Emergency Override
   ============================ */
void emergencyOverride(Lane lanes[], int laneCount) {
    int laneID;
    printf("Enter lane ID to force GREEN: ");
    scanf("%d", &laneID);

    if (laneID < 0 || laneID >= laneCount) {
        printf("Invalid lane ID!\n");
        return;
    }

    for (int i = 0; i < laneCount; i++) {
        if (i == laneID) {
            lanes[i].state = GREEN;
            lanes[i].vehicleCount = 0;
        } else {
            lanes[i].state = RED;
        }
        lanes[i].lastUpdate = time(NULL);
    }

    printf("Emergency Override: Lane %d is NOW GREEN.\n", laneID);
}


/* ============================
   Run Simulation (with logging)
   ============================ */
void runSimulation(Lane lanes[]) {
    for (int t = 0; t < 10; t++) {
        printf("Time %2d:\n", t);

        for (int i = 0; i < 2; i++) {
            int arriving = rand() % 2;
            lanes[i].vehicleCount += arriving;

            lanes[i].greenTime = 2 + lanes[i].vehicleCount;
            if (lanes[i].greenTime > 5)
                lanes[i].greenTime = 5;
        }

        for (int i = 0; i < 2; i++)
            printLane(&lanes[i]);

        updateSignals(lanes, 2);

        logToFile(lanes, 2, t); // <-- WRITE TO LOG FILE HERE

        printf("\n");
        sleep(1);
    }

    printf("Simulation finished. Log saved to traffic_log.txt\n");
}


/* ============================
   MAIN MENU
   ============================ */
int main() {
    Lane lanes[2];

    initializeLane(&lanes[0], 0, 2);
    initializeLane(&lanes[1], 1, 1);

    srand(time(NULL));

    int choice;

    while (1) {
        printf("\n=== Smart Traffic Controller Menu ===\n");
        printf("1. Run simulation (10s)\n");
        printf("2. Display current signal states\n");
        printf("3. Show traffic statistics\n");
        printf("4. Emergency override (force green)\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                runSimulation(lanes);
                break;
            case 2:
                for (int i = 0; i < 2; i++)
                    printLane(&lanes[i]);
                break;
            case 3:
                printStats(lanes, 2);
                break;
            case 4:
                emergencyOverride(lanes, 2);
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}
