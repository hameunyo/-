#include <stdio.h>


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

int get_input(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s (%d~%d): ", prompt, min, max);
        scanf_s("%d", &value);
        if (value >= min && value <= max) {
            break;
        }
        else {
            printf("�߸� �Է��߽��ϴ�. �ٽ� �Է����ּ���.\n");
        }
    }
    return value;
}

void zombie_move(int* mz, int mm, int mc, int m_a, int c_a, int turn) {
    if (turn % 2 != 0) {
        if (c_a > m_a || (c_a == m_a && *mz < mm)) {
            if (*mz - 1 > mc) {
                (*mz)--;
            }
        }
        else if (c_a < m_a || (c_a == m_a && *mz > mm)) {
            if (*mz + 1 != mm) {
                (*mz)++;
            }
        }
        else {
            if (*mz - 1 > mc) {
                (*mz)--;
            }
        }
    }
}

void citizen_move(int* mc, int p_p) {
    int r = rand() % 101;
    if (100 - p_p >= r) {
        (*mc)--;
    }
}

void madongseok_move(int* mm, int mz) {
    int MM;
    while (1) {
        printf("madongseok move (%d: stay, %d: left)>>> ", MOVE_STAY, MOVE_LEFT);
        scanf_s("%d", &MM);
        if (MM <= MOVE_LEFT && MM >= MOVE_STAY) {
            if (MM == MOVE_LEFT && *mm - 1 == mz) {
                continue;
            }
            else {
                break;
            }
        }
    }
    if (MM == MOVE_LEFT) {
        (*mm)--;
    }
}

void madongseok_action(int mm, int mz, int* m_a, int* S_M, int p_p, int* zombie_can_move) {
    int m_action;
    if (mm != mz + 1) {
        printf("madongseok action(%d.rest, %d.provoke)>>> ", ACTION_REST, ACTION_PROVOKE);
    }
    else {
        printf("madongseok action(%d.rest, %d.provoke, %d.pull)>>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
    }
    scanf_s("%d", &m_action);
    printf("\n");

    int prev_a = *m_a;
    int prev_S_M = *S_M;
    if (m_action == ACTION_REST) {
        (*m_a)--;
        (*S_M)++;
        if (*m_a < AGGRO_MIN) {
            *m_a = AGGRO_MIN;
        }
        if (*S_M > STM_MAX) {
            *S_M = STM_MAX;
        }
        printf("madongseok rests...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mm, prev_a, *m_a, prev_S_M, *S_M);
    }
    else if (m_action == ACTION_PROVOKE) {
        *m_a = AGGRO_MAX;
        printf("madongseok provokes zombie...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", mm, prev_a, *m_a, *S_M);
    }
    else if (m_action == ACTION_PULL) {
        *m_a += 2;
        (*S_M)--;
        if (*m_a > AGGRO_MAX) {
            *m_a = AGGRO_MAX;
        }
        if (*S_M < STM_MIN) {
            *S_M = STM_MIN;
        }

        int pull_success = rand() % 101;
        if (pull_success < (100 - p_p)) {
            printf("madongseok pulled zombie... Next turn, it can't move\n");
            *zombie_can_move = 0; // ���� �Ͽ� ���� �̵����� ����
        }
        else {
            printf("madongseok failed to pull zombie\n");
        }

        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mm, prev_a, *m_a, prev_S_M, *S_M);
    }
}

int main(void) {
    // ��Ʈ��
    printf("���� ����\n");
    Sleep(500);

    srand((unsigned int)time(NULL));

    int train_length = get_input("���� ����", LEN_MIN, LEN_MAX);
    int S_M = get_input("������ ü��", STM_MIN, STM_MAX);
    int p_p = get_input("Ȯ��", PROB_MIN, PROB_MAX);
    printf("\n");

    int mm = train_length - 2;
    int mz = train_length - 3;
    int mc = train_length - 6;
    int c_a = 1, m_a = 1;
    int turn = 1;
    int zombie_can_move = 1; // ���� �̵� �������� ����

    print_train(train_length, mm, mz, mc);

    // ���� ����
    printf("���� ����\n");
    Sleep(500);

    while (1) {
        int b_mm = mm, b_mz = mz, b_mc = mc;
        int bc_a = c_a, bm_a = m_a;
        int bS_M = S_M;

        // ���� ������
        zombie_move(&mz, mm, mc, m_a, c_a, turn);

        // �ù� ������
        citizen_move(&mc, p_p);

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

        // ������ �̵�
        madongseok_move(&mm, mz);

        // �������� �̵� ����� ���� ���� ���¸� �ٽ� ���
        if (mm == b_mm) {
            int prev_a = m_a;

            m_a--;
            if (m_a < AGGRO_MIN) {
                m_a = AGGRO_MIN;
            }
            print_train(train_length, mm, mz, mc);
            printf("\n");
            printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", mm, prev_a, m_a, S_M);
        }
        else {
            int prev_a = m_a;
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

        // ������ �ൿ
        madongseok_action(mm, mz, &m_a, &S_M, p_p, &zombie_can_move);

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
