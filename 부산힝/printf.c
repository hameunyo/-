#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Windows에서 Sleep 함수 사용을 위해

#define LEN_MIN 15 // 열차 길이
#define LEN_MAX 50
#define STM_MIN 0  // 마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

void print_train(int train_length, int mm, int mz, int mc) {
    // 열차 첫 번째 줄
    for (int i = 0; i < train_length; i++) {
        printf("#");
    }
    printf("\n");

    // 열차 두 번째 줄
    for (int i = 0; i < train_length; i++) {
        if (i == mm) {
            printf("M");
        }
        else if (i == mz) {
            printf("Z");
        }
        else if (i == mc) {
            printf("C");
        }
        else if (i == 0 || i == train_length - 1) {
            printf("#");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");

    // 열차 세 번째 줄
    for (int i = 0; i < train_length; i++) {
        printf("#");
    }
    printf("\n");
}

int main(void) {
    // 인트로
    printf("게임 시작11111111111111\n");
    Sleep(500);

    srand((unsigned int)time(NULL));
    printf("train length (%d~%d): ", LEN_MIN, LEN_MAX);
    int train_length, p_p;
    int S_M, bS_M;
    scanf_s("%d", &train_length);

    int mm, mz, mc, c_a = 1, bc_a, bm_a, m_a = 1;
    mm = train_length - 2;
    mz = train_length - 3;
    mc = train_length - 6;
    int b_mz, b_mc, b_mm;
    int MM;
    int m_rest = ACTION_REST, m_pro = ACTION_PROVOKE, m_pull = ACTION_PULL;
    int m_action;

    // 열차 길이 잘못 입력한 경우
    if (train_length < LEN_MIN || train_length > LEN_MAX) {
        printf("열차 길이를 다시 입력해주세요.\n");
        return 0;
    }

    // 마동석 체력
    printf("마동석 체력 (%d~%d): ", STM_MIN, STM_MAX);
    scanf_s("%d", &S_M);
    if (S_M < STM_MIN || S_M > STM_MAX) {
        printf("잘못 입력했습니다.\n");
        return 0;
    }

    // 확률 잘못 입력한 경우
    printf("확률 (%d~%d): ", PROB_MIN, PROB_MAX);
    scanf_s("%d", &p_p);
    if (p_p < PROB_MIN || p_p > PROB_MAX) {
        printf("확률을 다시 입력해주세요.\n");
        return 0;
    }

    print_train(train_length, mm, mz, mc);

    // 게임 시작
    printf("게임 시작\n");
    Sleep(500);

    int turn = 1;

    // while 시작
    while (1) {
        int r = rand() % 101;
        int y = rand() % 101;
        b_mm = mm;
        b_mz = mz;
        b_mc = mc;
        bc_a = c_a;
        bm_a = m_a;
        bS_M = S_M;

        
        // 좀비 움직임
        if (turn % 2 != 0) { 
            if (c_a > m_a) { 
                mz--; 
            }
            else if (c_a < m_a) { 
                if (mz + 1 != mm) { 
                    mz++; 
                }
            }
            else { 
                mz--; 
            }
        }


        // 시민 움직임
        if (100 - p_p >= r) {
            mc--;
        }

        print_train(train_length, mm, mz, mc);

        // 시민 상태
        if(b_mc ==mc) {
            c_a--;
            if (c_a < AGGRO_MIN) {
                c_a = AGGRO_MIN; // 시민의 어그로가 AGGRO_MIN 이하로 내려가지 않도록 
            }
            
            printf("citizen: stay %d (aggro: %d -> %d)\n", mc, bc_a, c_a);
        }

         else {
            c_a++;
            
            printf("citizen: %d -> %d (aggro: %d -> %d)\n", b_mc, mc, bc_a, c_a);
            if (c_a >= AGGRO_MAX) {
                c_a = AGGRO_MAX;
            }
        }
        // 좀비 상태
        if (turn % 2 != 0) {
            if (b_mz == mz) {
                printf("zombie: stay %d\n", mz);
            }
            else {
                printf("zombie: %d -> %d\n", b_mz, mz);
            }
        }
        else {
            printf("zombie: stay %d (cannot move)\n", mz);
        }
        printf("\n");

        // 마동석 이동 입력 반복
        while (1) {
            printf("madongseok move (%d: stay, %d: left)>>> ", MOVE_STAY, MOVE_LEFT);
            scanf_s("%d", &MM);
            if (MM <= MOVE_LEFT && MM >= MOVE_STAY) {
                if (MM == MOVE_LEFT && mm - 1 == mz) {
                    continue; // 좀비와 동일한 위치가 되면 다시 입력 받기
                }
                else {
                    break;
                }
            }
        }
        // 마동석의 이동 결과에 따라 열차 상태를 다시 출력
        if (MM == MOVE_STAY) {
            m_a--;
            if (m_a < AGGRO_MIN) {
                m_a = AGGRO_MIN; // 마동석의 어그로가 AGGRO_MIN 이하로 내려가지 않도록 함
            }
            print_train(train_length, mm, mz, mc);
            printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", mm, bm_a, m_a, S_M);
        }
        else if (MM == MOVE_LEFT) {
            mm--;
            print_train(train_length, mm, mz, mc);
            if (mm == mz) {
                mm++;
                m_a++;
                if (m_a > AGGRO_MAX) {
                    m_a = AGGRO_MAX;
                }
            }
            else {
                m_a++;
                if (m_a > AGGRO_MAX) {
                    m_a = AGGRO_MAX;
                }
            }
            printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n", b_mm, mm, bm_a, m_a, S_M);
        }

        printf("\n");
        printf("citizen does nothing\n");
        // 좀비가 시민을 잡았는지 판별
        if (mz == mc + 1) {
            printf("GAME OVER! citizen dead...\n");
            return 0;
        }
        //좀비가 마동석을 공격하면
        if (mz == mm - 1) {
            S_M--;
            printf("Zombie attacked madongseok (arrgo:%d vs %d, madongseok stamina: %d -> %d)",c_a,m_a,bS_M,S_M );
        }
        else {
        printf("zombie attacked nobody\n");

        }
        printf("madongseok action(%d.rest, %d.provoke)>>> ", m_rest, m_pro);
        scanf_s("%d", &m_action);

        printf("\n");

        if (m_action = ACTION_REST) {
            m_a--;
            S_M++;
            printf("madongseok rests...\n");
        printf("madongseok: %d (aggro: %d ->%d, stamina: %d -> %d)\n", mm,bm_a, m_a,bS_M, S_M);

        }
        if (m_action == ACTION_PROVOKE) {
            m_a = AGGRO_MAX;
            printf("madongseok provokes...\n");
            printf("madongseok: %d (aggro: %d ->%d, stamina: %d -> %d)\n", mm,bm_a, m_a,bS_M, S_M);

        }

        

        printf("\n");
        // 시민이 탈출했는지 판별
        if (mc == 1) {
            printf("성공\n");
            return 0;
        }


        turn++;
    }
    return 0;
}
