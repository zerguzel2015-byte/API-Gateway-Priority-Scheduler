#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int main() {
    printf("--- API Gateway Priority Scheduler ---\n\n");

    // 10 kapasiteli bir kuyruk oluşturuyoruz.
    // min_heap.c içindeki realloc sayesinde kapasite aşılırsa otomatik büyüyecek.
    PriorityQueue* pq = create_queue(10);
    int time_counter = 0; // FIFO mantığı için geliş zamanı sayacı

    // Test Senaryosu: Gelen HTTP Request Header'ları
    const char* headers[] = {
        "u=5",       // ID 1: Düşük öncelik
        "u=1, i",    // ID 2: Yüksek öncelik
        "",          // ID 3: Boş header (RFC'ye göre varsayılan u=3 olmalı)
        "u=1",       // ID 4: ID 2 ile aynı öncelik ama sonra geldi (FIFO çalışmalı)
        "u=0"        // ID 5: En yüksek öncelik (ilk işlenmesi gereken)
    };

    // 1. Aşama: İstekleri Parse Et ve Kuyruğa Ekle
    for (int i = 0; i < 5; i++) {
        Request req = parse_rfc9213(i + 1, headers[i], time_counter++);
        insert_request(pq, req);
        printf("[GELEN ISTEK] ID: %d | Raw: '%s' \t-> Parse Sonucu: u=%d, i=%d, time=%d\n",
               req.request_id, headers[i], req.urgency, req.incremental, req.arrival_time);
    }

    printf("\n--- Istekler Isleniyor (Priority Queue) ---\n");

    // 2. Aşama: İstekleri Önceliğe Göre Çıkar
    while (!is_empty(pq)) {
        Request req = extract_min(pq);
        printf("[ISLENEN ISTEK] ID: %d | Urgency: %d | Incremental: %d | Arrival: %d\n",
               req.request_id, req.urgency, req.incremental, req.arrival_time);
    }

    // 3. Aşama: Bellek Yönetimi (Memory Leak olmaması için)
    free_queue(pq);
    printf("\n[SISTEM] Bellek basariyla temizlendi. Program sonlandiriliyor.\n");

    return 0;
}
