/**
 ******************************************************************************
 * @file    VL53L4CX_Sat_HelloWorld.ino
 * @author  SRA
 * @version V1.0.0
 * @date    16 March 2022
 * @brief   Arduino test application for the STMicrolectronics VL53L4CX
 *          proximity sensor satellite based on FlightSense.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2022 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *notice, this list of conditions and the following disclaimer in the
 *documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/*
 * To use this sketch you need to connect the VL53L4CD satellite sensor directly
 * to the Nucleo board with wires in this way: pin 1 (GND) of the VL53L4CD
 * satellite connected to GND of the Nucleo board pin 2 (VDD) of the VL53L4CD
 * satellite connected to 3V3 pin of the Nucleo board pin 3 (SCL) of the
 * VL53L4CD satellite connected to pin D15 (SCL) of the Nucleo board pin 4 (SDA)
 * of the VL53L4CD satellite connected to pin D14 (SDA) of the Nucleo board pin
 * 5 (GPIO1) of the VL53L4CD satellite connected to pin A2 of the Nucleo board
 * pin 6 (XSHUT) of the VL53L4CD satellite connected to pin A1 of the Nucleo
 * board
 */
/* Includes ------------------------------------------------------------------*/
#include "vl53l4cx_class.h"
#include <Arduino.h>
#include <Wire.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEV_I2C Wire
#define SerialPort Serial

#define LedPin LED_BLUE

// Components.
VL53L4CX sensor_vl53l4cx_sat(&DEV_I2C, 0);

String VL53L4CX_RangeStatusCode(uint8_t status) {
  switch (status) {
  case VL53L4CX_RANGESTATUS_RANGE_VALID:
    return "VL53L4CX_RANGESTATUS_RANGE_VALID";
  case VL53L4CX_RANGESTATUS_SIGMA_FAIL:
    return "VL53L4CX_RANGESTATUS_SIGMA_FAIL";
  case VL53L4CX_RANGESTATUS_RANGE_VALID_MIN_RANGE_CLIPPED:
    return "VL53L4CX_RANGESTATUS_RANGE_VALID_MIN_RANGE_CLIPPED";
  case VL53L4CX_RANGESTATUS_OUTOFBOUNDS_FAIL:
    return "VL53L4CX_RANGESTATUS_OUTOFBOUNDS_FAIL";
  case VL53L4CX_RANGESTATUS_HARDWARE_FAIL:
    return "VL53L4CX_RANGESTATUS_HARDWARE_FAIL";
  case VL53L4CX_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL:
    return "VL53L4CX_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL";
  case VL53L4CX_RANGESTATUS_WRAP_TARGET_FAIL:
    return "VL53L4CX_RANGESTATUS_WRAP_TARGET_FAIL";
  case VL53L4CX_RANGESTATUS_PROCESSING_FAIL:
    return "VL53L4CX_RANGESTATUS_PROCESSING_FAIL";
  case VL53L4CX_RANGESTATUS_XTALK_SIGNAL_FAIL:
    return "VL53L4CX_RANGESTATUS_XTALK_SIGNAL_FAIL";
  case VL53L4CX_RANGESTATUS_SYNCRONISATION_INT:
    return "VL53L4CX_RANGESTATUS_SYNCRONISATION_INT";
  case VL53L4CX_RANGESTATUS_RANGE_VALID_MERGED_PULSE:
    return "VL53L4CX_RANGESTATUS_RANGE_VALID_MERGED_PULSE";
  case VL53L4CX_RANGESTATUS_TARGET_PRESENT_LACK_OF_SIGNAL:
    return "VL53L4CX_RANGESTATUS_TARGET_PRESENT_LACK_OF_SIGNAL";
  case VL53L4CX_RANGESTATUS_MIN_RANGE_FAIL:
    return "VL53L4CX_RANGESTATUS_MIN_RANGE_FAIL";
  case VL53L4CX_RANGESTATUS_RANGE_INVALID:
    return "VL53L4CX_RANGESTATUS_RANGE_INVALID";
  case VL53L4CX_RANGESTATUS_NONE:
    return "VL53L4CX_RANGESTATUS_NONE";
  default:
    return ("UNKNOWN STATUS: " + String(status));
  }
}

/* Setup ---------------------------------------------------------------------*/

void setup() {
  // Led.
  pinMode(LedPin, OUTPUT);

  // Initialize serial for output.
  SerialPort.begin(115200);
  SerialPort.println("Starting...");

  // Initialize I2C bus.
  DEV_I2C.begin();

  // Configure VL53L4CX satellite component.
  sensor_vl53l4cx_sat.begin();

  // Switch off VL53L4CX satellite component.
  sensor_vl53l4cx_sat.VL53L4CX_Off();

  // Initialize VL53L4CX satellite component.
  sensor_vl53l4cx_sat.InitSensor(0x12);

    // Start Measurements
  sensor_vl53l4cx_sat.VL53L4CX_StartMeasurement();
}

void loop() {
  VL53L4CX_MultiRangingData_t MultiRangingData;
  VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
  uint8_t NewDataReady = 0;
  int no_of_object_found = 0, j;
  char report[64];
  int status;

  delay(100);

  do {
    status =
        sensor_vl53l4cx_sat.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    SerialPort.print("  Status = ");
    SerialPort.println(status);
  } while (!NewDataReady);

  // Led on
  digitalWrite(LedPin, HIGH);

  if ((!status) && (NewDataReady != 0)) {
    status =
        sensor_vl53l4cx_sat.VL53L4CX_GetMultiRangingData(pMultiRangingData);
    no_of_object_found = pMultiRangingData->NumberOfObjectsFound;

    snprintf(report, sizeof(report), "VL53L4CX Satellite: Count=%d, #Objs=%1d ",
             pMultiRangingData->StreamCount, no_of_object_found);
    SerialPort.print(report);
    for (j = 0; j < no_of_object_found; j++) {
      SerialPort.print("\r\n                               ");
      SerialPort.print("status=");
      SerialPort.print(pMultiRangingData->RangeData[j].RangeStatus);
      SerialPort.print(", D=");
      SerialPort.print(pMultiRangingData->RangeData[j].RangeMilliMeter);
      SerialPort.print("mm");
      SerialPort.print(", Signal=");
      SerialPort.print(
          (float)pMultiRangingData->RangeData[j].SignalRateRtnMegaCps /
          65536.0);
      SerialPort.print(" Mcps, Ambient=");
      SerialPort.print(
          (float)pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps /
          65536.0);
      SerialPort.print(" Mcps, ");
      SerialPort.print(VL53L4CX_RangeStatusCode(
          pMultiRangingData->RangeData[j].RangeStatus));
    }
    SerialPort.println();
  }
  if (status == 0) {
    status = sensor_vl53l4cx_sat.VL53L4CX_ClearInterruptAndStartMeasurement();
  }

  //sensor_vl53l4cx_sat.VL53L4CX_StopMeasurement();
  digitalWrite(LedPin, LOW);
  delay(250);
}