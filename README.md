# Compile

```bash
make -f MakeFile
./main
```

# Data strcuture

## Input_Data

-   충격량 측정을 위한 입력 데이터 구조
-   충격량, 충격 방향, 시설물의 기울기를 측정하기 위해 9축 센서의 각 x, y, z 값을 입력으로 받는다.
-   이 때, z축은 중력 가속도로부터 수직인 것을 기준으로 한다. 따라서, z축의 가속도 데이터는 시설물에 아무 충격이 가해지지 않을 때 9.8의 값이 측정된다.

```c
typedef struct Input_Data_struct {
    float acc_x;
    float acc_y;
    float acc_z;
    float vel_x;
    float vel_y;
    float vel_z;
    float mag_x;
    float mag_y;
    float mag_z;
} Input_Data;
```

## Output_Data

-   충격량 측정 결과로 출력되는 데이터 구조
-   shockLevel (0: 충격 없음, 1: 약한 충격, 2: 강한 충격)
-   shockDirection (0: 충격 없음, 1: 좌측 충격, 2: 우측 충격, 3: 하측 충격, 4: 상측 충격)
-   code (1: 정상 출력, -1: 비정상 출력)
-   message (함수 API 실행 메시지)

```c
typedef enum Shock_struct {
    NoShock = 0,
    WeekShock = 1,
    StrongShock = 2
} ShockLevel;

typedef struct Output_Data_struct {
    int reqID;
    ShockLevel shocklevel;
    int shockDirection;
    float shockValue;
    float degree;
    int code;
    char* message;

} Output_Data;
```

## 함수 API

-   실제 충격감지 알고리즘을 이용하는 예제
-   ./src/main.c에 구현되어 있으며, 이를 통하여 1초마다 함수 API를 호출하고 충격 측정 결과를 반환한다.

```c
Output_Data* getShockLevel(int reqID, Input_Data* data);
```

## 예제 코드 실행 결과

-   예제 코드 실행 결과 로그 출력
-   순서대로 함수 API 호출 횟수, 정상 출력 여부, 메시지, 충격량 레벨, 충격 방향, 충격량, 그리고 시설물의 기울기를 의미한다.

![Feature 7](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature7.jpg)

# 알고리즘 개요

-   지능형 충격감지 알고리즘은 9축 센서로부터 받은 raw 데이터를 입력받아 시설물에 가해진 충격 강도, 충격 값, 그리고 충격 방향 등을 산출하여 리턴하는 알고리즘
-   1개의 센서가 아닌 다중 센서로부터 받은 데이터를 융합하여 통합적으로 처리하는 알고리즘을 개발

![Feature 1](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature1.jpg)

## 3축 가속도 센서 데이터 예제

-   3축 (x, y, z)로부터 측정되는 가속도에 대한 데이터 예제
-   z축은 중력 가속도를 받으며, x축과 y축으로부터 충격량 및 충격 방향을 산출할 수 있음

![Feature 2](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature2.jpg)

### 가속도 센서 데이터 분석

-   가속도 센서를 왼쪽으로 기울었을 때, x = 9.8
-   가속도 센서를 오른쪽으로 기울었을 때, x = -9.8
-   가속도 센서를 위로 기울었을 때, y = 9.8
-   가속도 센서를 아래로 기울었을 때, y = -9.8

-   왼쪽에서 충격을 줬을 때, x축이 양으로 튀었다가 음으로 튐
-   오른쪽에서 충격을 줬을 때, x축이 음으로 튀었다가 양으로 튐
-   위에서 충격을 줬을 때, y축이 양으로 튀었다가 음으로 튐
-   아래에서 충격을 줬을 때, y축이 음으로 튀었다가 양으로 튐

-   충격을 주는 방향이 먼저 튀며, 그에 따라 충격 방향 산출 가능

### 시설물 기울기 측정

-   시설물이 땅과 수직을 이룰 경우 z축은 중력 가속도를 그대로 받음
-   그러나 시설물이 기울어져 수직을 이루지 못할 경우 z축은 중력 가속도를 그대로 받지 못하며, x축과 y축이 받는 중력 가속도를 이용하여 시설물의 기울어진 정도를 측정할 수 있음

![Feature 3](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature3.jpg)

### 시설물에 가해진 충격량 측정

-   시설물에 아무 충격이 가해지지 않을 경우 다음 그림의 식을 통해 얻은 값은 9.8임
-   그러나 시설물에 일정 충격이 가해질 경우 해당 그림의 식을 통해 얻은 값은 9.8 이상인 것을 볼 수 있음

![Feature 4](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature4.jpg)

-   이는 아래 그림의 빨간색 그래프와 같음

![Feature 5](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature5.jpg)

## 시설물 충격 시나리오

-   시설물에 충격이 발생하기 전의 중력 가속도를 받는 축은 1개이며, 이를 제외한 2개의 축은 0의 값을 보임
-   시설물에 충격이 발생하기 전의 자이로 센서의 변화는 없음
-   시설물에 충격이 가해지고 기울어질 경우 가속도의 센서의 변화가 생기며, 자이로 센서의 값이 변화함
-   시설물이 기울어지는 것이 멈출 경우 자이로 센서의 값은 변화가 없으며, 3축에서 측정되는 중력 가속도 값이 달라짐

![Feature 6](https://kihyeon-hong.github.io/Collection_of_repository_images/ShockLevel_test_code/feature6.jpg)
