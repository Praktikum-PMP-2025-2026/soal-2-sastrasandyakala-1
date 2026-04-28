#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definisi struct untuk Artefak
typedef struct {
    char nama[100];
    int bobot;
    int nilai;
} Artefak;

// Variabel global untuk menyimpan state kombinasi terbaik
int best_val = -1;
int best_weight = 99999999;
int *best_subset;
int best_count = 0;

// Fungsi komparator untuk mengurutkan daftar awal secara alfabetis
int compare_nama(const void *a, const void *b) {
    Artefak *artA = (Artefak *)a;
    Artefak *artB = (Artefak *)b;
    return strcmp(artA->nama, artB->nama);
}

// Fungsi rekursif untuk mencari semua kemungkinan subset
void cari_kombinasi(int idx, int current_val, int current_weight, int *current_subset, int current_count, Artefak *arr, int N, int W) {
    // Basis rekursi: Jika semua artefak sudah dievaluasi (diambil/tidak)
    if (idx == N) {
        bool update = false;

        // Aturan 3: Maksimalkan total nilai
        if (current_val > best_val) {
            update = true; 
        } 
        // Aturan 4: Jika ada beberapa subset dengan nilai maksimum sama
        else if (current_val == best_val) {
            // Aturan 4a: Pilih yang total bobot lebih kecil
            if (current_weight < best_weight) {
                update = true; 
            } 
            // Aturan 4b: Jika bobot masih sama, cek urutan nama leksikografis terkecil
            else if (current_weight == best_weight) {
                // Karena array 'arr' sudah diurutkan alfabetis, kita cukup 
                // membandingkan indeks. Indeks yang lebih kecil berarti namanya lebih awal di alfabet.
                int limit = (current_count < best_count) ? current_count : best_count;
                bool diff = false;
                bool smaller = false;
                
                for (int i = 0; i < limit; i++) {
                    if (current_subset[i] < best_subset[i]) {
                        smaller = true;
                        diff = true;
                        break;
                    } else if (current_subset[i] > best_subset[i]) {
                        smaller = false;
                        diff = true;
                        break;
                    }
                }
                
                if (!diff) {
                    // Jika satu subset adalah awalan (prefix) dari subset lain, yang lebih pendek lebih kecil leksikografisnya
                    if (current_count < best_count) {
                        update = true; 
                    }
                } else if (smaller) {
                    update = true;
                }
            }
        }

        // Simpan kombinasi ini jika terbukti lebih baik dari kombinasi sebelumnya
        if (update) {
            best_val = current_val;
            best_weight = current_weight;
            best_count = current_count;
            for (int i = 0; i < current_count; i++) {
                best_subset[i] = current_subset[i];
            }
        }
        return;
    }

    // Cabang 1: Coba ambil artefak ke-idx (pastikan bobot total <= W)
    if (current_weight + arr[idx].bobot <= W) {
        current_subset[current_count] = idx;
        cari_kombinasi(idx + 1, current_val + arr[idx].nilai, current_weight + arr[idx].bobot, current_subset, current_count + 1, arr, N, W);
    }

    // Cabang 2: Lewati (jangan ambil) artefak ke-idx
    cari_kombinasi(idx + 1, current_val, current_weight, current_subset, current_count, arr, N, W);
}

int main() {
    int N, W;
    
    // Membaca input jumlah artefak dan batas kapasitas beban
    if (scanf("%d %d", &N, &W) != 2) {
        return 0;
    }

    // Alokasi Dynamic Array untuk daftar artefak
    Artefak *daftar = (Artefak *)malloc(N * sizeof(Artefak));
    for (int i = 0; i < N; i++) {
        scanf("%s %d %d", daftar[i].nama, &daftar[i].bobot, &daftar[i].nilai);
    }

    // Mengurutkan array awal secara alfabetis untuk mempermudah pengecekan tie-break nama
    qsort(daftar, N, sizeof(Artefak), compare_nama);

    // Alokasi memori dinamis untuk menyimpan array index subset terbaik dan subset saat ini
    best_subset = (int *)malloc(N * sizeof(int));
    int *current_subset = (int *)malloc(N * sizeof(int));

    // Eksekusi pencarian dengan kondisi awal semua 0
    cari_kombinasi(0, 0, 0, current_subset, 0, daftar, N, W);

    // Mencetak Output (sudah pasti tercetak berurutan alfabetis karena array awal diurutkan)
    printf("MAX VALUE %d\n", best_val);
    for (int i = 0; i < best_count; i++) {
        printf("ITEM %s\n", daftar[best_subset[i]].nama);
    }

    // Membebaskan memori
    free(daftar);
    free(best_subset);
    free(current_subset);

    return 0;
}
