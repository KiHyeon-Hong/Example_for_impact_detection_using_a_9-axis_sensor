#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ShockLevel.h"

/*
* 예제 데이터 파일 읽어오기
*/
Input_Data* createData() {
    Input_Data* data = (Input_Data *)malloc(sizeof(Input_Data) * 6000);
    int i = 0;

    char str_tmp[1024];
    FILE* pFile = NULL;

    pFile = fopen("./files/sampleFile.csv", "r");

    if(pFile != NULL) {
        while((!feof(pFile)) && (i <= 6000)) {
            fgets(str_tmp, 1024, pFile);

            char* ptr = strtok(str_tmp, ",");
            float tempData[9];
            int j = 0;
            
            while(ptr != NULL) {
                if(i == 0) {
                    break;
                }

                if((j >= 1)) {
                    tempData[j - 1] = atof(ptr);
                }
                j = j + 1;

                ptr = strtok(NULL, ",");
            }

            if(i != 0) {
                data[i - 1].acc_x = tempData[0];
                data[i - 1].acc_y = tempData[1];
                data[i - 1].acc_z = tempData[2];
                data[i - 1].vel_x = tempData[3];
                data[i - 1].vel_y = tempData[4];
                data[i - 1].vel_z = tempData[5];
                data[i - 1].mag_x = tempData[6];
                data[i - 1].mag_y = tempData[7];
                data[i - 1].mag_z = tempData[8];
            }

            i = i + 1;
        }
    }
    fclose(pFile);

    return data;
}

/*
* 예제 데이터 파일을 읽고 모듈을 호출하는 메소드
*/
int main(void) {
    Input_Data* dataBuffer = createData();

    int i = 0;

    /*
    * 예제 데이터 파일을 1초마다 윈도우 사이즈로 실행
    */
    for(i = 0; i < (int)(6000 / 10) - 1; i++) {
        Input_Data* inputData = (Input_Data *)malloc(sizeof(Input_Data) * bufferSize);

        sleep(1);
        
        /*
        * 1초에 20개의 데이터 입력
        */
        int j = 0;
        for(j = 0; j < bufferSize; j++) {
            inputData[j] = dataBuffer[(i * 10) + j];
        }

        /*
        * 지능형 충격감지 알고리즘 호출
        */
        Output_Data* returnLevel = getShockLevel(i, inputData);
        
        printf("%3d\t", returnLevel->reqID);
        printf("%3d\t", returnLevel->code);
        printf("%15s\t", returnLevel->message);
        printf("%20s\t", (returnLevel->shocklevel == 0)? "0 (No Shock)": (returnLevel->shocklevel == 1)? "1 (Week Shock)": (returnLevel->shocklevel == 2)? "2 (Strong Shock)": "-1 (Error)");
        printf("%3d (%8s)\t", returnLevel->shockDirection, (returnLevel->shockDirection == 0)? "No Shock": (returnLevel->shockDirection == 1)? "Left": (returnLevel->shockDirection == 2)? "Right": (returnLevel->shockDirection == 3)? "Down": (returnLevel->shockDirection == 4)? "Top": "-1 (Error)");
        printf("%5.2f (m/s^2)\t", returnLevel->shockValue);
        printf("%5.1f\n", returnLevel->degree);
    }

    return 0;
}
