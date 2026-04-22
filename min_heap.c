#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

// 1. Yeni bir Kuyruk Oluşturma (Dinamik Bellek Ayırma)
PriorityQueue* create_queue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->requests = (Request*)malloc(capacity * sizeof(Request));
    return pq;
}

// 2. Belleği Serbest Bırakma (Memory Leak Önleme)
void free_queue(PriorityQueue* pq) {
    free(pq->requests);
    free(pq);
}

// Elemanları takas etmek için yardımcı fonksiyon
void swap(Request *a, Request *b) {
    Request temp = *a;
    *a = *b;
    *b = temp;
}

// 3. Heapify Up: Yeni eklenen elemanı yukarı taşır
void heapify_up(PriorityQueue* pq, int index) {
    if (index == 0) return;

    int parent = (index - 1) / 2;

    // Eğer mevcut elemanın önceliği (urgency) ebeveyninden küçükse VEYA
    // Öncelikler eşitse ve mevcut eleman daha önce gelmişse (FIFO tie-break)
    if (pq->requests[index].urgency < pq->requests[parent].urgency ||
       (pq->requests[index].urgency == pq->requests[parent].urgency &&
        pq->requests[index].arrival_time < pq->requests[parent].arrival_time)) {

        swap(&pq->requests[index], &pq->requests[parent]);
        heapify_up(pq, parent);
    }
}

// 4. Heapify Down: Kökten çıkarılan eleman yerine gelen en son elemanı aşağı indirir
void heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Sol çocuk ile karşılaştır
    if (left < pq->size) {
        if (pq->requests[left].urgency < pq->requests[smallest].urgency ||
           (pq->requests[left].urgency == pq->requests[smallest].urgency &&
            pq->requests[left].arrival_time < pq->requests[smallest].arrival_time)) {
            smallest = left;
        }
    }

    // Sağ çocuk ile karşılaştır
    if (right < pq->size) {
        if (pq->requests[right].urgency < pq->requests[smallest].urgency ||
           (pq->requests[right].urgency == pq->requests[smallest].urgency &&
            pq->requests[right].arrival_time < pq->requests[smallest].arrival_time)) {
            smallest = right;
        }
    }

    if (smallest != index) {
        swap(&pq->requests[index], &pq->requests[smallest]);
        heapify_down(pq, smallest);
    }
}

// 5. Kuyruğa İstek Ekleme
void insert_request(PriorityQueue* pq, Request req) {
    // Kapasite dolarsa realloc ile genişletilebilir (Ödevde verimlilik için önemli)
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->requests = (Request*)realloc(pq->requests, pq->capacity * sizeof(Request));
    }

    pq->requests[pq->size] = req;
    heapify_up(pq, pq->size);
    pq->size++;
}

// 6. En Yüksek Öncelikli İsteği Çıkarma (Min-Extract)
Request extract_min(PriorityQueue* pq) {
    Request root = pq->requests[0];
    pq->requests[0] = pq->requests[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return root;
}

bool is_empty(PriorityQueue* pq) {
    return pq->size == 0;
}
