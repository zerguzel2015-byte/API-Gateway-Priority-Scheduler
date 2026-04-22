# RFC 9213 Uyumlu, Modüler API Gateway İstek Zamanlayıcısı (Priority Request Scheduler)

Bu proje, Veri Yapıları ve Algoritmalar dersi kapsamında C dilinde geliştirilmiş gerçeğe yakın bir **API Gateway simülasyonudur**. Gelen HTTP isteklerini **RFC 9213 (Extensible HTTP Priorities)** standardına uygun olarak ayrıştırır (parse) ve **Min-Heap (Öncelikli Kuyruk)** veri yapısını kullanarak yüksek performansla öncelik sırasına göre işler.

## 📌 Proje Teslim Bağlantıları
* **Proje Anlatım Videosu:** [YouTube Video Linkinizi Buraya Yapıştırın]
* **Proje Sunumu:** [Teams veya Drive Linkinizi Buraya Yapıştırın]

## 🚀 Özellikler

* **RFC 9213 Parsing:** Gelen istek başlıklarından `urgency` (u) ve `incremental` (i) değerlerini ayrıştırır. Parametre girilmezse standartlara uygun olarak varsayılan `u=3` değerini atar.
* **Min-Heap Algoritması:** İstekleri sıradan bir dizi yerine Min-Heap ağaç yapısında tutarak sistemin tıkanmasını önler ve kuyruk yönetimini optimize eder.
* **Eşitlik Durumunda FIFO (Tie-Break):** Aynı önceliğe sahip istekler geldiğinde, geliş zamanına (`arrival_time`) bakarak ilk geleni ilk işler (First-In-First-Out).
* **Dinamik Bellek Yönetimi:** `malloc`, `realloc` ve `free` fonksiyonları kullanılarak kapasite dinamik olarak yönetilir. Sistem kapatılırken tüm bellek serbest bırakılarak **sıfır bellek sızıntısı (zero memory leak)** garantisi verilir.
* **Modüler Mimari:** Header (`.h`) ve Source (`.c`) dosya ayrımı yapılarak kodlar izole edilmiş ve 'Single Responsibility' (Tek Sorumluluk) prensibine uyulmuştur.

## 📂 Mimari ve Dosya Yapısı

Proje, C dilinin modülerlik yetenekleri kullanılarak birbirinden izole edilmiş 4 ana dosyaya bölünmüştür:

```text
├── scheduler.h      # İskelet: Struct yapıları, API ve fonksiyon prototipleri
├── rfc_parser.c     # Ayrıştırma: String manipülasyonu ve RFC 9213 istek ayrıştırma
├── min_heap.c       # Algoritma: Min-Heap işlemleri ve dinamik bellek yönetimi
└── main.c           # Orkestrasyon: Test senaryoları ve ana çalışma döngüsü
