#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>

// RFC 9213 standardına uygun İstek (Request) Yapısı
// Urgency (u): 0 en yüksek, 7 en düşük öncelik
typedef struct {
    int request_id;
    int urgency;
    bool incremental;
    int arrival_time;  // Aynı u değerine sahip isteklerde FIFO(ilk giren ilk çıkar) sağlamak için
    char raw_header[50]; // Örn: "u=2, i"
} Request;

// Min-Heap (Priority Queue) Yapısı
typedef struct {
    Request *requests; // Dinamik dizi (malloc/realloc ile yöneteceğiz)
    int capacity;
    int size;
} PriorityQueue;

// --- FONKSİYON PROTOTİPLERİ ---

// 1. RFC Parser Katmanı
Request parse_rfc9213(int id, const char* header, int arrival_time);

// 2. Memory (Bellek) Yönetimi ve Queue Katmanı
PriorityQueue* create_queue(int capacity);
void free_queue(PriorityQueue* pq);

// 3. Min-Heap Algoritması Katmanı
void insert_request(PriorityQueue* pq, Request req);
Request extract_min(PriorityQueue* pq);
void heapify_down(PriorityQueue* pq, int index);
void heapify_up(PriorityQueue* pq, int index);
bool is_empty(PriorityQueue* pq);

#endif
