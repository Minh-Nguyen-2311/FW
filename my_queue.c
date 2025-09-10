
#include "my_queue.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdbool.h>

static char queue[MAX_SIZE][MSG_MAX_LEN];
static int front = -1, rear = -1;
static osMutexId queueMutex;

void queueMutexInit(void) {
    osMutexDef(queueMutexDef);
    queueMutex = osMutexCreate(osMutex(queueMutexDef));
}

// Kiểm tra đầy
bool isFull(void) {
    bool full;
    osMutexWait(queueMutex, osWaitForever);
    full = ((rear + 1) % MAX_SIZE == front);
    osMutexRelease(queueMutex);
    return full;
}

// Kiểm tra rỗng
bool isEmpty(void) {
    bool empty;
    osMutexWait(queueMutex, osWaitForever);
    empty = (front == -1);
    osMutexRelease(queueMutex);
    return empty;
}

// Thêm phần tử
void enqueue(const char *data) {
    osMutexWait(queueMutex, osWaitForever);
    if (!((rear + 1) % MAX_SIZE == front)) {  // Không đầy
        if (front == -1) front = 0;
        rear = (rear + 1) % MAX_SIZE;
        strncpy(queue[rear], data, MSG_MAX_LEN - 1);
        queue[rear][MSG_MAX_LEN - 1] = '\0';
    }
    osMutexRelease(queueMutex);
}

// Lấy phần tử
//char* dequeue(void) {
//    static char temp[MSG_MAX_LEN];  // buffer tĩnh để trả về
//    osMutexWait(queueMutex, osWaitForever);
//    if (front == -1) {
//        temp[0] = '\0'; // Queue rỗng => trả về chuỗi rỗng
//    } else {
//        strncpy(temp, queue[front], MSG_MAX_LEN);
//        temp[MSG_MAX_LEN - 1] = '\0';
//        if (front == rear) {
//            front = rear = -1;
//        } else {
//            front = (front + 1) % MAX_SIZE;
//        }
//    }
//    osMutexRelease(queueMutex);
//    return temp;
//}

bool dequeue(char *out) {
    bool hasData = false;
    osMutexWait(queueMutex, osWaitForever);
    if (front != -1) { // Không rỗng
        strncpy(out, queue[front], MSG_MAX_LEN - 1);
        out[MSG_MAX_LEN - 1] = '\0';
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % MAX_SIZE;
        }
        hasData = true;
    }
    osMutexRelease(queueMutex);
    return hasData;
}

// Đếm số phần tử
static int getCount(void) {
    int count;
    osMutexWait(queueMutex, osWaitForever);
    if (front == -1) {
        count = 0;
    } else if (rear >= front) {
        count = rear - front + 1;
    } else {
        count = MAX_SIZE - front + rear + 1;
    }
    osMutexRelease(queueMutex);
    return count;
}

// Lấy số ô trống
int getFreeSlots(void) {
    return MAX_SIZE - getCount();
}
