#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ShockLevel.h"

/*
* 충격량 측정 메소드
*/
float getShock(Input_Data* data) {
    float maxShock = 0;
    float temp = 0;

    int i = 0;

    /*
    * 최대 충격량 측정
    */
    for(i = 0; i < bufferSize; i++) {
        /*
        * 가속도 센서의 x, y, z축의 값을 이용하여 충격량 계산
        */
        temp = sqrt(pow(data[i].acc_x, 2) + pow(data[i].acc_y, 2) + pow(data[i].acc_z, 2));

        if(temp > maxShock) {
            maxShock = temp;
        }
    }

    /*
    * 최대 충격량 반환
    */
    return maxShock;
}

/*
* 각속도 측정 메소드
*/
float getGyro(Input_Data* data) {
    float maxGyro = 0;
    float temp = 0;

    int i = 0;

    /*
    * 최대 각속도 측정
    */
    for(i = 0; i < bufferSize; i++) {
        /*
        * 각속도 센서의 x, y, z 축의 값을 이용하여 센서 이동 계산
        */
        temp = sqrt(pow(data[i].vel_x, 2) + pow(data[i].vel_y, 2) + pow(data[i].vel_z, 2));

        if(temp > maxGyro) {
            maxGyro = temp;
        }
    }

    /*
    * 최대 각속도 반환
    */
    return maxGyro;
}

/*
* x축 충격인지, y축 충격인지 판단 메소드
*/
int getDirectionXY(Input_Data* data) {
    float xMax = 0.0;
    float yMax = 0.0;
    float xTemp = 0.0;
    float yTemp = 0.0;

    int i = 0;

    /*
    * x축으로의 충격 최대값과 y축으로의 충격 최댓값을 비교하여,
    * 큰 충격이 가해진 방향으로 결정
    */ 
    for(i = 0; i < bufferSize; i++) {
        xTemp = fabsf(data[i].acc_x);
        yTemp = fabsf(data[i].acc_y);

        if(xTemp > xMax) {
            xMax = xTemp;
        }

        if(yTemp > yMax) {
            yMax = yTemp;
        }
    }

    /*
    * 0: x축으로의 충격, 1: y축으로의 충격
    */
    return (xMax > yMax)? 0: 1;
}

/*
* 충격 방향 측정 메소드
*/
int getDirection(Input_Data* data) {
    /*
    * 충격 방향 결정
    */
    int xyDirection = getDirectionXY(data);

    /*
    * 0: x축으로의 충격, 1: y축으로의 충격
    */
    if(xyDirection == 0) {
        float min = data[0].acc_x;
        float max = data[0].acc_x;
        int minPoint = 0;
        int maxPoint = 0;

        int i = 0;
        
        /*
        * 충격 값의 최댓값과 최솟값을 계산하고,
        * 해당 값에서의 index 저장
        */
        for(i = 1; i < bufferSize; i++) {
            if(min > data[i].acc_x) {
                minPoint = i;
                min = data[i].acc_x;
            }
            if(max < data[i].acc_x) {
                maxPoint = i;
                max = data[i].acc_x;
            }
        }

        /*
        * 그래프 형태에 따라 충격 방향 결정
        */
        if(maxPoint > minPoint) {
            return 2;
        }
        else {
            return 1;
        }
    }
    else {
        float min = data[0].acc_y;
        float max = data[0].acc_y;
        int minPoint = 0;
        int maxPoint = 0;

        int i = 0;

        /*
        * 충격 값의 최댓값과 최솟값을 계산하고,
        * 해당 값에서의 index 저장
        */
        for(i = 1; i < bufferSize; i++) {
            if(min > data[i].acc_y) {
                minPoint = i;
                min = data[i].acc_y;
            }
            if(max < data[i].acc_y) {
                maxPoint = i;
                max = data[i].acc_y;
            }
        }

        /*
        * 그래프 형태에 따라 충격 방향 결정
        */
        if(maxPoint > minPoint) {
            return 4;
        }
        else {
            return 3;
        }
    }

    /*
    * Error
    */
    return 5;
}

/*
* 센서 기울기 측정 메소드
*/
float getDegree(Input_Data* data) {
    /*
    * z축을 기준으로 x축의 기울기와 y축의 기울기를 계산
    */
    float x_degree = fabsf(atanf((-data[0].acc_x)/sqrt((pow(data[0].acc_y, 2) + pow(data[0].acc_z, 2))))*180/PI);
    float y_degree = fabsf(atanf((-data[0].acc_y)/sqrt((pow(data[0].acc_x, 2) + pow(data[0].acc_z, 2))))*180/PI);
    
    /*
    * x축과 y축 중에서 더 많이 기운 쪽의 기울기로 반환
    */
    return (x_degree > y_degree)? x_degree: y_degree;
}

/*
* 지능형 충격감지 알고리즘 모듈 (포인터로 리턴할 것)
*/
Output_Data* getShockLevel(int reqID, Input_Data* data) {
    Output_Data* returnLevel = (Output_Data *)malloc(sizeof(Output_Data));
    
    /*
    * returnLevel->reqID 
    * returnLevel->shocklevel 충격 레벨
    * returnLevel->shockDirection 충격방향
    * returnLevel->shockValue 충격 값
    * returnLevel->degree 충격 후 현재 기울기
    * returnLevel->code 반환 코드 (정상 처리 1)
    * returnLevel->message 반환 메시지
    */
    returnLevel->reqID = reqID;
    returnLevel->shocklevel = NoShock;
    returnLevel->shockDirection = 0;
    returnLevel->shockValue = 0.0;
    returnLevel->degree = 0.0;
    returnLevel->code = 1;
    returnLevel->message = "Success";

    /*
    * 입력 데이터 버퍼에서 최대 충격량 측정
    */
    returnLevel->shockValue = getShock(data);

    if((returnLevel->shockValue < 0)) {
        returnLevel->code = -1;
        returnLevel->message = "ShockValue Error";
    }

    /*
    * 충격치 기준에 따른 충격 레벨
    */
    if(returnLevel->shockValue > strong) {
        returnLevel->shocklevel = StrongShock;
    }
    else if(returnLevel->shockValue > week) {
        returnLevel->shocklevel = WeekShock;
    }

    if((returnLevel->shocklevel != NoShock) && (returnLevel->shocklevel != WeekShock) && (returnLevel->shocklevel != StrongShock)) {
        returnLevel->code = -1;
        returnLevel->message = "ShockLevel Error";
    }

    /*
    * 충격 후 시설물의 현재 기울기 측정
    * 시설물이 기울고 있을 때에는 현재 기울기를 측정하지 않으며(각속도가 일정 기준 이상일 경우),
    * 시설물이 더 이상 이동하지 않을 때 기울기 측정
    */
    float gyro = getGyro(data);

    if((gyro < 0)) {
        returnLevel->code = -1;
        returnLevel->message = "Gyro Error";
    }

    if((gyro < vibration) && (getDegree(data) > 1.0)) {
        returnLevel->degree = getDegree(data);
    }

    /*
    * 시설물의 충격 방향 측정
    * 충격량이 일정 이상이 아닐 경우에는 충격 방향을 측정하지 않으며,
    * 시설물에 일정 이상의 충격이 가해졌을 경우에만 충격량 측정
    */
    if(returnLevel->shocklevel != NoShock) {
        returnLevel->shockDirection = getDirection(data);
    }

    if((returnLevel->shockDirection < 0) || (returnLevel->shockDirection > 4)) {
        returnLevel->code = -1;
        returnLevel->message = "ShockDirection Error";
    }

    /*
    * 출력 데이터 반환
    */
    return returnLevel;
}