#include "include/BBBL3GD20H.h"

/**
 \brief BBBLSM303DLHC : A class that provides control of the accelerometer LSM303DLHC and extends I2C devices on the Beaglebone Black.
  BBBI2CDevice : A low level control of I2C devices on the Beaglebone Black.
 */
BBBL3GD20H::BBBL3GD20H( ) {
    if( this->SetPower( _switch::ON ) < 0 ) {
        cerr << "Critical Error 1 : LSM303DLHC - Attempted to set the Accelerometers Power State to : " << this->GyroSwitch << " | This action failed." << endl;
        exit( 1 );
    }
}

int BBBL3GD20H::SetPower( _switch GyroSwitch ) {
    this->GyroSwitch = GyroSwitch;
    return this->Gyroscope.I2C_WriteToReg( CTRL1, GyroSwitch );
}

void* BBBL3GD20H::GetValues(  void *gyro_inst  ) {
    BBBL3GD20H* gyro = (BBBL3GD20H*)gyro_inst;
    while( 1 ) {
        gyro->SetX( );
        gyro->SetY( );
        gyro->SetZ( );
        gyro->AddToDataStore( );
        usleep( 5000 );
    }
}

void BBBL3GD20H::AddToDataStore( ) {
    for( int i = 0; i < ( MAX_DATASTORE -1 ); i++ ) {
        int nextIndex = ( i + 1 );
        this->GyroXDataStoredValues[ i ] = this->GyroXDataStoredValues[ nextIndex ];
        this->GyroYDataStoredValues[ i ] = this->GyroYDataStoredValues[ nextIndex ];
        this->GyroZDataStoredValues[ i ] = this->GyroZDataStoredValues[ nextIndex ];
    }
    this->GyroXDataStoredValues[ ( MAX_DATASTORE - 1 ) ] = this->GetX( );
    this->GyroYDataStoredValues[ ( MAX_DATASTORE - 1 ) ] = this->GetY( );
    this->GyroZDataStoredValues[ ( MAX_DATASTORE - 1 ) ] = this->GetZ( );
}

void BBBL3GD20H::Start( ) {
    pthread_create( &this->GyroNotifyer, NULL, BBBL3GD20H::GetValues, this );
}

void BBBL3GD20H::CalcOffsets( ) {
    int i = 1, total_x = 0, total_y = 0, total_z = 0;
    while ( i <= 1500 ) {
        total_x += this->g.X;
        total_y += this->g.Y;
        total_z += this->g.Z;
        usleep( 5000 );
        i++;
    }
    this->avgX = total_x / 1500;
    this->avgY = total_y / 1500;
    this->avgZ = total_z / 1500;
}

/**
 \fn Public function SetAverages( int iterations = 50 )
 \brief Reads the current X, Y, Z on this device 50 times and stores the average.
 \param <int>
 \return <void>
 */
void BBBL3GD20H::SetAverages( int iterations = 50 ) {
    int i = 0, total_X = 0, total_Y = 0, total_Z = 0;
    while (i < iterations) {
        total_X += this->g.X;
        total_Y += this->g.Y;
        total_Z += this->g.Z;
        usleep(5000);
        i++;
    }
    this->avgX = total_X / iterations;
    this->avgY = total_Y / iterations;
    this->avgZ = total_Z / iterations;
}

/*!
 @function SetX
 - Set this->g.X to the current X reading from the sensor
 @returns <void>
 */
void BBBL3GD20H::SetX( void ) {
    this->g.X = ( int16_t ) ( this->Gyroscope.I2C_GetValue( OUT_X_L ) | ( this->Gyroscope.I2C_GetValue( OUT_X_H ) << 8 ) );
    this->g.X *= 0.00875F;
    this->g.X = (this->g.X);
}

/*!
 @function SetY
 - Set this->g.Y to the current Y reading from the sensor
 @returns <void>
 */
void BBBL3GD20H::SetY( void ) {
    this->g.Y = ( int16_t ) ( this->Gyroscope.I2C_GetValue( OUT_Y_L ) | ( this->Gyroscope.I2C_GetValue( OUT_Y_H ) << 8 ) );
    this->g.Y *= 0.00875F;
    this->g.Y = (this->g.Y);
}

/*!
 @function SetZ
 - Set this->g.Z to the current Z reading from the sensor
 @returns <void>
 */
void BBBL3GD20H::SetZ( void ) {
    this->g.Z = -( int16_t ) ( this->Gyroscope.I2C_GetValue( OUT_Z_L ) | ( this->Gyroscope.I2C_GetValue( OUT_Z_H ) << 8 ) );
    this->g.Z *= 0.00875F;
    this->g.Z = (this->g.Z);
}

/*!
 @function GetX
 - get the current X reading from the sensor
 @returns <short> g.X
 */
int BBBL3GD20H::GetX( void ) {
    return this->g.X;
}

/*!
 @function GetY
 - get the current Y reading from the sensor
 @returns <short> this->g.Y
 */
int BBBL3GD20H::GetY( void ) {
    return this->g.Y;
}

/*!
 @function GetZ
 - get the current Z reading from the sensor
 @returns <short> this->_Z
 */
int BBBL3GD20H::GetZ( void ) {
    return this->g.Z;
}
