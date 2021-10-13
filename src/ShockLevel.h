/*
* message_len 반환 메시지 길이
* bufferSize 모듈로 입력받는 버퍼 크기
* week 약한 충격 기준
* strong 강한 충격 기준
* vibration 떨림 기준
*/
#define message_len 50
#define bufferSize 20
#define week 11
#define strong 13
#define vibration 10
#define PI 3.14159

/*
* 입력 데이터 구조
* acc_x 가속도 센서 x축 값
* acc_y 가속도 센서 y축 값
* acc_z 가속도 센서 z축 값
* vel_x 각속도 센서 x축 값
* vel_y 각속도 센서 y축 값
* vel_z 각속도 센서 z축 값
* mag_x 지자기 센서 x축 값
* mag_y 지자기 센서 y축 값
* mag_z 지자기 센서 z축 값
*/
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

/*
* 충격 기준
*/
typedef enum Shock_struct {
    NoShock = 0,
    WeekShock = 1,
    StrongShock = 2
} ShockLevel;

/*
* 출력 데이터 구조
* shocklevel 등급
* shockDirection 방향 (0: 충격 없음, 1: 왼쪽 충격, 2: 오른쪽 충격, 3: 위쪽 충격, 4: 아래쪽 충격)
* shockValue 값
* degree 각도
* code코드 (비정상: -1, 정상 1)
* message 리턴 메시지
*/
typedef struct Output_Data_struct {
    int reqID;
    ShockLevel shocklevel;
    int shockDirection;
    float shockValue;
    float degree;
    int code;
    char* message;

} Output_Data;

/*
* 지능형 충격감지 알고리즘 인터페이스
*/
Output_Data* getShockLevel(int reqID, Input_Data* data);