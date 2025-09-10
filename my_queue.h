/*
 * queue.h
 *
 *  Created on: Aug 11, 2025
 *      Author: Nguyen Trieu
 */

#ifndef INC_MY_QUEUE_H_
#define INC_MY_QUEUE_H_

#include <stdbool.h>

#define MAX_SIZE       100
#define MSG_MAX_LEN    30

// Khai báo hàm
bool isFull(void);
bool isEmpty(void);
void enqueue(const char *data);
//char* dequeue(void);
bool dequeue(char *out);
int getFreeSlots(void);
void queueMutexInit(void);

#endif /* INC_MY_QUEUE_H_ */
