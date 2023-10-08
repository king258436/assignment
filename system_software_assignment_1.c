#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 100001

typedef struct listNode {
    struct listNode *next;
    long long data;
} listNode;

int p_num[MAX];
int check[MAX];
int prime_check[MAX];
int only_one[MAX];
double db_arr[MAX];
int db_p;
long long big_arr[MAX];
int big_p;
long long minus_arr[MAX];
int minus_p;
double minus_db_arr[MAX];
int minus_db_p;

listNode *init_list() {
    listNode *head;
    head = (listNode *)malloc(sizeof(listNode));
    head->next = NULL;
    return head;
}

void add_first_node(listNode *h, long long data) {
    listNode *newNode;
    newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = data;
    newNode->next = h->next;
    h->next = newNode;
}

int node_len(listNode *h) {
    int cnt;
    listNode *cur;
    cur = h;

    cnt = 0;
    while (cur->next != NULL) {
        cnt++;
        cur = cur->next;
    }
    return cnt;
}

int static compare_db(const void *first, const void *second) {
    if (*(double *)first > *(double *)second)
        return 1;
    else if (*(double *)first < *(double *)second)
        return -1;
    else
        return 0;
}

int static compare_big(const void *first, const void *second) {
    if (*(long long *)first > *(long long *)second)
        return 1;
    else if (*(long long *)first < *(long long *)second)
        return -1;
    else
        return 0;
}

void eratos() {
    p_num[0] = 1;
    p_num[1] = 1;
    for (int i = 2; i <= sqrt(MAX); i++) {  // 에라토스 테네스의 체
        if (p_num[i] == 0) {
            for (int j = 2; i * j < MAX; j++) {
                p_num[i * j] = 1;  // 소수가 아니면 1
            }
        }
    }
}

int is_prime(double n) {
    for (int i = 2; i <= sqrt(n); i++) {
        if ((long long)n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void big_arr_cal(long long arr[], int arr_p, listNode *prime_node, listNode *one_node) {  // 10만 이상인 수들 계산
    qsort(arr, arr_p, sizeof(long long), compare_big);                                    // 큰것들만 퀵 소트

    for (int i = 0; i < arr_p; i++) {
        if (arr[i] == arr[i + 1]) {
            only_one[i] = 0;  // 두 수가 같다
        } else {              // 두 수가 다르다.
            only_one[i] = 1;
        }
    }  // 중복되는 수 없애고 하나의 수만 1로 표시.

    for (int i = 0; i < arr_p; i++) {
        if (only_one[i] == 1) {
            add_first_node(one_node, arr[i]);  // 오직 하나의수는 onenode에 들어가야함
            if (arr[i] > 0) {
                for (int j = -1; j < 2; j += 2) {
                    if (is_prime(arr[i] + j) == 1) {
                        add_first_node(prime_node, arr[i]);  // 만약 소수라면 primenode에 들어감
                        break;
                    }
                }
            }
        }
    }
}

void db_arr_cal(double arr[], int arr_p, listNode *one_node) {
    qsort(arr, arr_p, sizeof(double), compare_db);  // 큰것들만 퀵 소트

    for (int i = 0; i < arr_p; i++) {
        if (arr[i] == arr[i + 1]) {
            only_one[i] = 0;  // 두 수가 같다
        } else {              // 두 수가 다르다.
            only_one[i] = 1;
        }
    }  // 중복되는 수 없애고 하나의 수만 1로 표시.

    for (int i = 0; i < arr_p; i++) {
        if (only_one[i] == 1) {
            add_first_node(one_node, arr[i]);  // 오직 하나의수는 onenode에 들어가야함
        }
    }
}

void min10_cal(listNode *prime_node, listNode *one_node) {  // 10만 이하의 수가 소수인지 판별하고 인서트 시키는 함수.
    for (int i = 0; i < MAX; i++) {
        if (prime_check[i] > 0) {
            add_first_node(prime_node, i);
        }
        if (check[i] > 0) {
            add_first_node(one_node, i);
        }
    }
}

void print_all_node(listNode *h) {  // 연결된 모든노드 프린트 하는 함수
    listNode *cur;
    cur = h->next;

    if (cur != NULL) {
        while (1) {
            printf("%lld", cur->data);
            if (cur->next == NULL) {
                printf("\n");
                break;
            } else
                printf("->");
            cur = cur->next;
        }
    } else {
        printf("No data\n");
    }
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    listNode *prime_node;
    listNode *one_node;
    // clock_t start = clock();
    prime_node = init_list();
    one_node = init_list();
    double num;
    eratos();

    while (fscanf(fp, "%lf", &num) != EOF) {  // 입력됨 num은 입력받은 값 자체

        if (num != (long long)num && num >= 0) {  // num이 실수라면,
            db_arr[db_p++] = num;                 // 배열에 넣어줌. 얘는 only one cnt 만 계산
        }                                         // 이상없음
        else if (num < MAX && 0 <= num) {         // n이 10만 안에있는 양수라면, 에라토스테네스 체 이용.
            for (int i = -1; i < 2; i += 2) {
                int n = num + i;
                if (n < 0) {
                    continue;
                }
                if (p_num[n] == 0) {          // 소수 라면,
                    prime_check[(int)num]++;  // 소수 체크
                    check[(int)num]++;        // 방문 표시++
                    break;
                }
                if (i == 1) {
                    check[(int)num]++;
                }
            }
        }                            // 이상없음
        else if (num >= MAX) {       // n 이 실수도 아니고 10만 이상의 숫자라면,
            big_arr[big_p++] = num;  // 일단 다 집어넣고 함수로 중복되는 숫자와, 리어 프라임 넘버 확인
        } else if (num != (long long)num && num < 0) {
            minus_db_arr[minus_db_p++] = num;
        } else if (num < 0) {            // 음수라면
            minus_arr[minus_p++] = num;  // 마이너스 값 다 집어넣기
        }
    }

    min10_cal(prime_node, one_node);
    big_arr_cal(big_arr, big_p, prime_node, one_node);
    big_arr_cal(minus_arr, minus_p, prime_node, one_node);
    db_arr_cal(db_arr, db_p, one_node);
    db_arr_cal(minus_db_arr, minus_db_p, one_node);
    printf("Only one count : %d\n", node_len(one_node));      //
    printf("Near prime count : %d\n", node_len(prime_node));  //
    // print_all_node(prime_node);
    // clock_t end = clock();
    // printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
