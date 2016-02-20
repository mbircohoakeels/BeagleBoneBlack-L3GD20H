//
// Created by Michael Brookes on 20/02/2016.
//

#include "include/BBBL3GD20H.h"

int main(void) {
    BBBL3GD20H L3GD20H;
    L3GD20H.Start();
    usleep(5000);

    while (1) {
        for (int i = 0; i <= (MAX_DATASTORE - 1); i++) {
            cout << "Gyro X Reading " << i << " : " << L3GD20H.GyroXDataStoredValues[i] << endl;
            //cout << "Gyro Y Reading " << i << " : " << L3GD20H.GyroYDataStoredValues[i] << endl;
            //cout << "Gyro Z Reading " << i << " : " << L3GD20H.GyroZDataStoredValues[i] << endl;
        }
        L3GD20H.PrintValuesToScreen();
        usleep(5000);
    }
}
