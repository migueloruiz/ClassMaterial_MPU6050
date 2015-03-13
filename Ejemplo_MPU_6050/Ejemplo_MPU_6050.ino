
//=========================================================================//
//=========================================================================//
//          Universidad Nacional Autonoma de México
//                   Facultad de Ingenieria
//                        Agosto 2014
//                  Ejemplo Basico MPU-6050
//                  Escrito por Miguel Ruiz
//                 --------------------------
//                Open Source / Public Domain
//                ----------------------------
//      Configuracion Default del MPU-6050 default at power-up:
//                  + Giroscopio a 250 º/seg
//                  + Accelerometro a 2g
//                  + Reloj Interno de 8MHz
//                  + Dispositivo en modo "Sleep"
//            -------------------------------------
//      Documentacion:
//      - "MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.0"
//      - "MPU-6000 and MPU-6050 Product Specification Revision 3.4"
//      - "Arduino Wire Library"
//
//=========================================================================//
//=========================================================================//

#include <Wire.h>  // Libreria I²C 

//La direccion del dispositivo depende del estado de AD0
// AD0 a pull-up ------------ 0x69
// AD0 a pull-down ---------- 0x68
#define MPU6050_I2C_ADDRESS 0x68

// Definicion del los registros segun
// "MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.0"
// Solo se definen los registros de interes 

#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define MPU6050_ACCEL_XOUT_L       0x3C   // R  
#define MPU6050_ACCEL_YOUT_H       0x3D   // R  
#define MPU6050_ACCEL_YOUT_L       0x3E   // R  
#define MPU6050_ACCEL_ZOUT_H       0x3F   // R  
#define MPU6050_ACCEL_ZOUT_L       0x40   // R  
#define MPU6050_TEMP_OUT_H         0x41   // R  
#define MPU6050_TEMP_OUT_L         0x42   // R  
#define MPU6050_GYRO_XOUT_H        0x43   // R  
#define MPU6050_GYRO_XOUT_L        0x44   // R  
#define MPU6050_GYRO_YOUT_H        0x45   // R  
#define MPU6050_GYRO_YOUT_L        0x46   // R  
#define MPU6050_GYRO_ZOUT_H        0x47   // R  
#define MPU6050_GYRO_ZOUT_L        0x48   // R  

// Se define el registro Power Management 1
// Necesario para sacar el dispositivo del modo "Sleep"
#define MPU6050_PWR_MGMT_1         0x6B   // R/W


// Definicion de Variables, se usa float ya por la extencion del dato
float AcX,AcY,AcZ,Temp,GyX,GyY,GyZ;

/***************************************************
 *  Nombre:      Setup
 *  Retornos:    Nada
 *  Parametros:  Nada
 *  Descripcion: Función de configuración. 
 ***************************************************/

void setup(){

  Wire.begin(); // Se inicializa el BUS I²C 

  Wire.beginTransmission( MPU6050_I2C_ADDRESS );  // Inicio de la comunicacion con el dispositivo

 
  // De acuerdo con el datasheet, el bit de 'sleep'
  // deberia estar en 1, pero se le 0.
  // Se debe limpiar este bit para sacar pasar el sensor
  // se sleep a wake. Se escribe 0 en el registro

  Wire.write(MPU6050_PWR_MGMT_1);  // Registro Power Management 1
  Wire.write(0);     // se escribe 0


  Wire.endTransmission(true); // se termina la comunicaccion 
  Serial.begin(9600);

}

/***************************************************
 *  Nombre:      Loop
 *  Retorna:     Nada
 *  Parametros:  Nada
 *  Descripcion: Loop general.
 ***************************************************/

void loop(){

    Wire.beginTransmission( MPU6050_I2C_ADDRESS );
    Wire.write(MPU6050_ACCEL_XOUT_H );  // En este registro se inicia la lectura
    Wire.endTransmission(false);        // Se interrumpe la comincación pero se mantene el reloj activo

    // Se piden 14 elementos comenzando desde el registro MPU6050_ACCEL_XOUT_H
    Wire.requestFrom( MPU6050_I2C_ADDRESS,14,true);  

    // Lectura de Acelerometro
    //  9 registros
    AcX = Wire.read()*256+Wire.read();  // 0x3B (MPU6050_ACCEL_XOUT_H) & 0x3C (MPU6050_ACCEL_XOUT_L)    
    AcY = Wire.read()*256+Wire.read();  // 0x3D (MPU6050_ACCEL_YOUT_H) & 0x3E (MPU6050_ACCEL_YOUT_L)
    AcZ = Wire.read()*256+Wire.read();  // 0x3F (MPU6050_ACCEL_ZOUT_H) & 0x40 (MPU6050_ACCEL_ZOUT_L)
  

    // Lectura de Temperatura
    //  2 registros  
    Temp = (Wire.read()*256+Wire.read())/340.00+36.53;  // 0x41 (MPU6050_TEMP_OUT_H) & 0x42 (MPU6050_TEMP_OUT_L)


    // Lectura de Temperatura
    //  9 registros  
    GyX = Wire.read()*256+Wire.read();  // 0x43 (MPU6050_GYRO_XOUT_H) & 0x44 (MPU6050_GYRO_XOUT_L)
    GyY = Wire.read()*256+Wire.read();  // 0x45 (MPU6050_GYRO_YOUT_H) & 0x46 (MPU6050_GYRO_YOUT_L)
    GyZ = Wire.read()*256+Wire.read();  // 0x47 (MPU6050_GYRO_ZOUT_H) & 0x48 (MPU6050_GYRO_ZOUT_L)

    // Imprecion de datos en el puerto R232 (Monitor Serial)
    Serial.print(" | AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.print(AcZ);
    Serial.print(" | Temp = "); Serial.print(Temp);
    Serial.print(" | GyX = "); Serial.print(GyX);
    Serial.print(" | GyY = "); Serial.print(GyY);
    Serial.print(" | GyZ = "); Serial.println(GyZ);
}




