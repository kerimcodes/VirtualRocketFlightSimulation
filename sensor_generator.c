/*
 * Project: Virtual Rocket Flight Computer Simulation
 * Author: Kerim Erduran
 * Date: Feb 2026
 * Description: 
 * Simulates a 1-DOF rocket flight physics, sensor noise/bias, 
 * and GNC algorithms (Filtering & State Machine).
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enum State
{
    IDLE,
    ASCENT,
    DESCENT,
    LANDED
};

typedef struct datas 
{
    float time;
    float acc_x, acc_y, acc_z;
    float vel_x, vel_y, vel_z; 
    float pos_x, pos_y, pos_z;
    float filtered_acc_x, filtered_acc_y, filtered_acc_z;
    int state;
} data;

void adddataTofile(const char* path,data *sensor, int n)
{
    FILE *fptr = fopen(path, "w");
    if (fptr == NULL)
        printf("File open operation is unsuccessfull\n");
    else {
        fprintf(fptr,"Time,Ax,FilteredAx,Vx,Px,Ay,FilteredAy,Vy,Py,Az,FilteredAz,Vz,Pz,State\n");
        for (int i = 0; i <= n;i++)
        {
            fprintf(fptr,"%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%d\n",
                sensor[i].time, sensor[i].acc_x,sensor[i].filtered_acc_x, sensor[i].vel_x, sensor[i].pos_x,
                sensor[i].acc_y,sensor[i].filtered_acc_y, sensor[i].vel_y, sensor[i].pos_y,
                sensor[i].acc_z,sensor[i].filtered_acc_z, sensor[i].vel_z, sensor[i].pos_z,
                sensor[i].state);
        }
        fclose(fptr);
    }
}

void createData(const char* path,int n)
{
    data *sensor = malloc((n+1) * (sizeof(data)));
    if (sensor == NULL) {
        printf("Error! Memory full");
    }
    else {
    srand(time(NULL));

    float dt = 0.01;
    float vx = 0.0, vy = 0.0, vz = 0.0;
    float px = 0.0, py = 0.0, pz = 0.0;

    float current_time = 0.0;
    float mass = 1.5, thrust = 16.0;
    int motor_burn_time = 5;
    float gravity = 9.81;

    enum State current_state = IDLE;
    for (int i = 0; i <= n;i++)
    {
        int current_thrust = 0.0;
        if (current_time < motor_burn_time)
            current_thrust = thrust;

        float true_accz = (current_thrust / mass) - gravity;

        if (pz <= 0 && current_thrust < (mass * gravity)) 
        {
            true_accz = 0; 
            vx = 0;
            vy = 0;
            vz = 0;
            pz = 0;
        }
        int random_numberx = rand() % 200;
        int random_numbery = rand() % 200;
        int random_numberz = ((rand() % 15) + 975);

        float fnumberx = (float)random_numberx / 100 -1 ;
        float fnumbery = (float)random_numbery / 100 - 1;
        float fnumberz = (float)random_numberz / 100;
        
        // --- SENSOR MODELING ---
        // Real sensor reads (True Acceleration + Gravity) due to internal mass.
        // Added random noise to simulate MEMS sensor characteristics.

        float noise_z = (float)random_numberz / 100 - 9.81;

        sensor[i].time = current_time;
        sensor[i].acc_x = fnumberx;
        sensor[i].acc_y = fnumbery;
        sensor[i].acc_z = (true_accz +9.81) + noise_z;

        float filtered_accX = 0.0;
        float filtered_accY = 0.0;
        float filtered_accZ = 0.0;
        if (i < 9) 
        {   
            for (int j = i; j >= 0;j--)
            {
                filtered_accX += sensor[j].acc_x;
                filtered_accY += sensor[j].acc_y;
                filtered_accZ += sensor[j].acc_z;
            }

            filtered_accX = filtered_accX / (i + 1);
            filtered_accY = filtered_accY / (i + 1);
            filtered_accZ = filtered_accZ / (i + 1);
            
        }
        else 
        {
            for (int j = 0; j < 10;j++)
            {
                filtered_accX += sensor[i - j].acc_x;
                filtered_accY += sensor[i - j].acc_y;
                filtered_accZ += sensor[i - j].acc_z;
            }
            filtered_accX = filtered_accX / 10;
            filtered_accY = filtered_accY / 10;
            filtered_accZ = filtered_accZ / 10;
        }
        vx += filtered_accX * dt;
        vy += filtered_accY * dt;
        vz += (filtered_accZ - 9.81) * dt;
        
        px += vx * dt;
        py += vy * dt;
        pz += vz * dt;

        sensor[i].filtered_acc_x = filtered_accX;
        sensor[i].filtered_acc_y = filtered_accY;
        sensor[i].filtered_acc_z = filtered_accZ;

        sensor[i].vel_x = vx;
        sensor[i].vel_y = vy;
        sensor[i].vel_z = vz;

        sensor[i].pos_x = px;
        sensor[i].pos_y = py;
        sensor[i].pos_z = pz;

        // --- FLIGHT STATE MACHINE (FSM) ---
        // Determines the flight phase based on altitude and velocity.
        // Transition Logic: IDLE -> ASCENT -> DESCENT -> LANDED

        if (current_state == IDLE && pz > 0.1)
            current_state = ASCENT;
        else if (current_state == ASCENT && vz < 0)
            current_state = DESCENT;
        else if (current_state == DESCENT && pz < 0.5)
            current_state = LANDED;

        sensor[i].state = current_state;
        current_time += 0.01; 
    }

    adddataTofile(path, sensor, n);
    free(sensor);

    printf("Datas are added");
    }
}
int main()
{
    const char *path = "C:\\software\\C_Python_Projects\\VirtualFlightSimulation\\data\\sensor.csv";

    createData(path, 1000);
    return 0;
}