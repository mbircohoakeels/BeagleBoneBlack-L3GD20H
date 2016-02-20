//
//  BBBL3GD20H.h
//  BBBL3GD20H
//
//  Created by Michael Brookes on 14/11/2015.
//  Copyright © 2015 Michael Brookes. All rights reserved.
//

#ifndef SRC_H_BBBL3GD20H_H_
#define SRC_H_BBBL3GD20H_H_

#include "BBBI2CDevice.h"
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include <iostream>

using namespace std;

//From Table 17. of the L3GD20H Data sheet ( http://www.adafruit.com/datasheets/L3GD20H.pdf )
#define GYRO_ADDRESS 	0x6B
#define CTRL1       	0x20
#define OUT_X_L     	0x28
#define OUT_X_H     	0x29
#define OUT_Y_L     	0x2A
#define OUT_Y_H     	0x2B
#define OUT_Z_L     	0x2C
#define OUT_Z_H     	0x2D

#define MAX_DATASTORE	15

/**
 \brief BBBL3GD20H : A class that provides control of the L3GD20H which contains : gyroscope.
 \author    Michael Brookes
 \version   1.0
 \date      Nov-2015
 \copyright GNU Public License.
 */
class BBBL3GD20H {

public:

    /**
     \brief Please refer to the documention provided for your L3GD20H for further details (CTRL1 pg 36 point 7.2)
     */
    enum _switch {
        ON 	= 0x7F,  		//!< 0x7F 200 Hz Data Rate, Power On and X, Y, Z enabled
        OFF = 0x00   		//!< 0x00 Off
    };

    int GetX( void );
    int GetY( void );
    int GetZ( void );

    void PrintValuesToScreen( void ) { cout << "X = " << this->g.X << " | Y = " << this->g.Y << " | Z = " << this->g.Z << endl;	}
    void PrintAveragesToScreen( void ) { cout << "X = " << this->avgX << " | Y = " << this->avgY << " | Z = " << this->avgZ << endl;	}
    void CalcOffsets( );
    void SetAverages( int iterations );
    void Start( );

    int GyroXDataStoredValues[ MAX_DATASTORE ];
    int GyroYDataStoredValues[ MAX_DATASTORE ];
    int GyroZDataStoredValues[ MAX_DATASTORE ];

    int avgX;
    int avgY;
    int avgZ;

    BBBL3GD20H();

protected:

    int totalX;
    int totalY;
    int totalZ;
    int OffsetX = 0;
    int OffsetY = 0;
    int OffsetZ = 0;
    int GyroRet;

    /**
     \brief Creates a new object containing communication and the setup of the gyroscope.
     */
    BBBI2CDevice Gyroscope = *new BBBI2CDevice( GYRO_ADDRESS, BBBI2CDevice::I2C1 );

    _switch GyroSwitch; 					//!< enum value for the Gyroscope power status.

    int SetPower( _switch GyroSwitch );

    void SetX( void );

    void SetY( void );

    void SetZ( void );

    static void* GetValues( void *gyro_inst );

    void AddToDataStore( );

    /**
     \brief Creates the template for vector, used by the gyroscope.
     */
    template <typename T> struct vector {
        T X, Y, Z;
    };

    pthread_t GyroNotifyer;

    vector<int> g; //!< Gyroscope vector (X, Y, Z)
};

#endif /* SRC_H_BBBL3GD20H_H_ */
