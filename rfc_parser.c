#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "scheduler.h"

// RFC 9213 Parser Fonksiyonu
// Gelen string'i (header) analiz edip Request struct'ını doldurur.
Request parse_rfc9213(int id, const char* header, int arrival_time) {
    Request req;

    // Temel atamalar
    req.request_id = id;
    req.arrival_time = arrival_time;
    req.urgency = 3;         // RFC 9213 standardına göre varsayılan urgency değeri 3'tür.
    req.incremental = false; // Varsayılan olarak parçalı (incremental) değil.

    // String'i güvenli bir şekilde struct içindeki raw_header dizisine kopyalıyoruz
    strncpy(req.raw_header, header, sizeof(req.raw_header) - 1);
    req.raw_header[sizeof(req.raw_header) - 1] = '\0'; // Sonlandırma karakterini garantiye alıyoruz

    // "u=" ifadesini string içinde arıyoruz (strstr C'de alt string aramak için kullanılır)
    char *u_ptr = strstr(header, "u=");
    if (u_ptr != NULL) {
        // u_ptr "u=" kısmını gösteriyor. u_ptr[2] bize rakamı verir.
        // Karakter olan rakamı integer'a çevirmek için '0' ın ASCII değerini çıkartıyoruz.
        int parsed_u = u_ptr[2] - '0';

        // Urgency 0 ile 7 arasında olmalıdır (RFC standardı)
        if (parsed_u >= 0 && parsed_u <= 7) {
            req.urgency = parsed_u;
        }
    }

    // "i" parametresi var mı kontrolü (incremental flag)
    // Eğer string içinde "i" harfi geçiyorsa true yapıyoruz
    if (strstr(header, "i") != NULL) {
         req.incremental = true;
    }

    return req;
}
