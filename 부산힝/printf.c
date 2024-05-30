#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Windows���� Sleep �Լ� ����� ����

#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0  // ������ ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 0 // ��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1 
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

void print_train(int train_length, int mm, int mz, int mc) {
    // ���� ù ��° ��
    for (int i = 0; i < train_length; i++) {
        printf("#");
    }
    printf("\n");

    // ���� �� ��° ��
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

    // ���� �� ��° ��
    for (int i = 0; i < train_length; i++) {
        printf("#");
    }
    printf("\n");
}

int main(void) {
    // ��Ʈ��
    printf("���� ����\n");
    Sleep(500);

    srand((unsigned int)time(NULL));
    printf("���� ���� (%d~%d): ", LEN_MIN, LEN_MAX);
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

    // ���� ���� �߸� �Է��� ���
    if (train_length < LEN_MIN || train_length > LEN_MAX) {
        printf("���� ���̸� �ٽ� �Է����ּ���.\n");
        return 0;
    }

    // ������ ü��
    printf("������ ü�� (%d~%d): ", STM_MIN, STM_MAX);
    scanf_s("%d", &S_M);
    if (S_M < STM_MIN || S_M > STM_MAX) {
        printf("�߸� �Է��߽��ϴ�.\n");
        return 0;
    }

    // Ȯ�� �߸� �Է��� ���
    printf("Ȯ�� (%d~%d): ", PROB_MIN, PROB_MAX);
    scanf_s("%d", &p_p);
    if (p_p < PROB_MIN || p_p > PROB_MAX) {
        printf("Ȯ���� �ٽ� �Է����ּ���.\n");
        return 0;
    }
    printf("\n");

    print_train(train_length, mm, mz, mc);

    // ���� ����
    printf("���� ����\n");
    Sleep(500);

    int turn = 1;
    int zombie_can_move = 1; // ���� �̵� �������� ����

    // while ����
    while (1) {
        int r = rand() % 101;
        b_mm = mm;
        b_mz = mz;
        b_mc = mc;
        bc_a = c_a;
        bm_a = m_a;
        bS_M = S_M;

        // ���� ������
        if (turn % 2 != 0 && zombie_can_move) {
            if (c_a > m_a || (c_a == m_a && mz < mm)) {
                mz--;
            }
            else if (c_a < m_a || (c_a == m_a && mz > mm)) {
                if (mz + 1 != mm) {
                    mz++;
                }
            }
            else {
                mz--;
            }
        }

        // �ù� ������
        if (100 - p_p >= r) {
            mc--;
        }

        print_train(train_length, mm, mz, mc);

        printf("\n");
        // �ù� ����
        if (b_mc == mc) {
            c_a--;
            if (c_a < AGGRO_MIN) {
                c_a = AGGRO_MIN; // �ù��� ��׷ΰ� AGGRO_MIN ���Ϸ� �������� �ʵ��� 
            }
            printf("citizen: stay %d (aggro: %d -> %d)\n", mc, bc_a, c_a);
        }
        else {
            c_a++;
            if (c_a > AGGRO_MAX) {
                c_a = AGGRO_MAX; // �ù��� ��׷ΰ� AGGRO_MAX�� �ʰ����� �ʵ���
            }
            printf("citizen: %d -> %d (aggro: %d -> %d)\n", b_mc, mc, bc_a, c_a);
        }

        // ���� ����
        if (turn % 2 != 0 && zombie_can_move) {
            if (b_mz == mz) {
                printf("zombie: stay %d\n", mz);
            }
            else {
                printf("zombie: %d -> %d\n", b_mz, mz);
            }
        }
        else {
            printf("zombie: stay %d (cannot move)\n", mz);
            zombie_can_move = 1; // ���� �Ͽ��� ���� �ٽ� �̵� ����
        }
        printf("\n");

        // ������ �̵� �Է� �ݺ�
        while (1) {
            printf("madongseok move (%d: stay, %d: left)>>> ", MOVE_STAY, MOVE_LEFT);
            scanf_s("%d", &MM);
            if (MM <= MOVE_LEFT && MM >= MOVE_STAY) {
                if (MM == MOVE_LEFT && mm - 1 == mz) {
                    continue; // ����� ������ ��ġ�� �Ǹ� �ٽ� �Է� �ޱ�
                }
                else {
                    break;
                }
            }
        }

        // �������� �̵� ����� ���� ���� ���¸� �ٽ� ���
        if (MM == MOVE_STAY) {
            int prev_a = m_a; // ���� ��׷� �� ����
            m_a--;
            if (m_a < AGGRO_MIN) {
                m_a = AGGRO_MIN; // �������� ��׷ΰ� AGGRO_MIN ���Ϸ� �������� �ʵ��� ��
            }
            print_train(train_length, mm, mz, mc);
            printf("\n");
            printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", mm, prev_a, m_a, S_M);
        }
        else if (MM == MOVE_LEFT) {
            int prev_a = m_a; // ���� ��׷� �� ����
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
            printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n", b_mm, mm, prev_a, m_a, S_M);
        }

        printf("\n");
        printf("citizen does nothing\n");

        // ���� ������ �ΰ��� �����ϴ� ����
        if ((mz == mm - 1 && mz == mc + 1) || (mz == mm && mz == mc)) {
            // �ùΰ� �������� ��� ������ ���
            if (m_a >= c_a) {
                bS_M = S_M;  // ���� �� ü�� ����
                S_M--;
                printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", c_a, m_a, bS_M, S_M);
                if (S_M <= STM_MIN) {
                    printf("GAME OVER! madongseok dead...\n");
                    return 0;
                }
            }
            else {
                printf("GAME OVER! citizen dead...\n");
                return 0;
            }
        }
        else if (mz == mm - 1) {
            bS_M = S_M;  // ���� �� ü�� ����
            S_M--;
            printf("Zombie attacked madongseok (madongseok stamina: %d -> %d)\n", bS_M, S_M);
            if (S_M <= STM_MIN) {
                printf("GAME OVER! madongseok dead...\n");
                return 0;
            }
        }
        else if (mz == mc + 1) {
            printf("GAME OVER! citizen dead...\n");
            return 0;
        }
        else {
            printf("zombie attacked nobody\n");
        }

        // ������ ����� ���� �ڵ�
        if (mm != mz + 1) {
            printf("madongseok action(%d.rest, %d.provoke)>>> ", m_rest, m_pro);
            scanf_s("%d", &m_action);
        }
        else if (mm == mz + 1) {
            printf("madongseok action(%d.rest, %d.provoke, %d.pull)>>> ", m_rest, m_pro, m_pull);
            scanf_s("%d", &m_action);
        }

        printf("\n");

        // ������ �ൿ
        int prev_a = m_a; // ���� ��׷� �� ����
        int prev_S_M = S_M; // ���� ü�� �� ����
        if (m_action == ACTION_REST) {
            m_a--;
            S_M++;
            if (m_a < AGGRO_MIN) {
                m_a = AGGRO_MIN;
            }
            if (S_M > STM_MAX) {
                S_M = STM_MAX;
            }
            printf("madongseok rests...\n");
            printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mm, prev_a, m_a, prev_S_M, S_M);
        }
        else if (m_action == ACTION_PROVOKE) {
            m_a = AGGRO_MAX;
            printf("madongseok provokes zombie...\n");
            printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", mm, prev_a, m_a, S_M);
        }
        else if (m_action == ACTION_PULL) {
            prev_S_M = S_M; // ���� ü�� �� ����
            m_a += 2;
            S_M--;
            if (m_a > AGGRO_MAX) {
                m_a = AGGRO_MAX;
            }
            if (S_M < STM_MIN) {
                S_M = STM_MIN;
            }

            int pull_success = rand() % 101;
            if (pull_success < (100 - p_p)) {
                printf("madongseok pulled zombie... Next turn, it can't move\n");
                zombie_can_move = 0; // ���� �Ͽ� ���� �̵����� ����
            }
            else {
                printf("madongseok failed to pull zombie\n");
            }

            printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mm, prev_a, m_a, prev_S_M, S_M);
        }

        printf("\n");

        // �ù��� Ż���ߴ��� �Ǻ�
        if (mc == 1) {
            printf("YOU WIN!...\n");
            return 0;
        }

        turn++;
    }

    return 0;
}