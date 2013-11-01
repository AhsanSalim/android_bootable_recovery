/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mincharge.h"

typedef struct {
    int isCharging;
    int chargeAmount;
    char ChgText[32];
} ChargeContext;

static ChargeContext gChargeContext = {0};

static void* chargeThread(void* cookie)
{
    char    tmpStr[32];
    int     tmpVal;
    FILE    *fbatt_stat, *fbatt_cap;
    
    cookie = NULL;
    
    while(1)
    {
        bzero(tmpStr, 32);
        
        // Check if battery is charging
        fbatt_stat = fopen("/sys/class/power_supply/battery/status", "r");
        fscanf(fbatt_stat, "%s", tmpStr);
        if(!strcmp("Charging", tmpStr)) {
            gChargeContext.isCharging = 1;
        } else {
            gChargeContext.isCharging = 0;
        }
        fclose(fbatt_stat);
        
        // Read amount of charge
        fbatt_cap = fopen("/sys/class/power_supply/battery/capacity", "r");
        fscanf(fbatt_cap, "%d", &tmpVal);
        gChargeContext.chargeAmount = tmpVal;
        fclose(fbatt_cap);
        
        // Sleep 2 seconds
        usleep((long)(2 * 1000000));
    }
    
    return NULL;
}

void initCharge() 
{
#ifdef RECOVERY_CHARGE_ENABLE
    pthread_t t;
    pthread_create(&t, NULL, chargeThread, NULL);
#endif //RECOVERY_CHARGE_ENABLE
}

char* getChargeString()
{
#ifdef RECOVERY_CHARGE_ENABLE
    bzero(gChargeContext.ChgText, 32);
    snprintf(gChargeContext.ChgText, 31, "Charg%s - %d%c", ((gChargeContext.isCharging)? "ing" : "e"), gChargeContext.chargeAmount, '%');
    return gChargeContext.ChgText;
#else
    return NULL;
#endif //RECOVERY_CHARGE_ENABLE
}

int getIsCharging()
{
#ifdef RECOVERY_CHARGE_ENABLE
    return gChargeContext.isCharging;
#else
    return 0;
#endif //RECOVERY_CHARGE_ENABLE
}

int getChargeAmount()
{
#ifdef RECOVERY_CHARGE_ENABLE
    return gChargeContext.chargeAmount;
#else
    return 0;
#endif //RECOVERY_CHARGE_ENABLE
}

