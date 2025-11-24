# A*-PathfindingGridGenerator

## Περιγραφή

Αυτό το πρόγραμμα υλοποιεί τον αλγόριθμο A* για εύρεση της μικρότερης διαδρομής μεταξύ δύο σημείων σε ένα τυχαία παραγόμενο grid με εμπόδια.

Το πρόγραμμα:

Δημιουργεί ένα τετράγωνο grid (10x10 έως 20x20)

Τοποθετεί εμπόδια σε τυχαία ποσοστά (10%–90%)

Ορίζει είσοδο (S) και έξοδο (E)

Τρέχει τον αλγόριθμο A* χρησιμοποιώντας Manhattan distance

Εκτυπώνει:

Το grid

Τα εμπόδια (#)

Τη διαδρομή (*)

Αναφέρει αν βρέθηκε μονοπάτι ή όχι

## Τεχνολογία & Αλγόριθμος

Ο αλγόριθμος A* λειτουργεί ως εξής:

f(n) = g(n) + h(n)

g(n) = κόστος από την αρχή μέχρι το n
h(n) = Manhattan distance (ευρετική)


Το πρόγραμμα εφαρμόζει:

Min-Heap για την open list

4 κατευθύνσεις κίνησης (up, down, left, right)

Ανακατασκευή διαδρομής μέσω parent pointers

Αποφυγή εμποδίων και ήδη εξερευνημένων κόμβων

## Δομή Grid
Σύμβολο	Περιγραφή
.	κενό τετράγωνο
#	εμπόδιο
S	Start / είσοδος
E	End / έξοδος
*	μονοπάτι A*
## Εισαγωγή – Δημιουργία Grid

Το πρόγραμμα δημιουργεί:

Μέγεθος grid: 10 έως 20

Ποσοστό εμποδίων: 10% έως 90%

int rows = 10 + rand() % 11;  // 10–20
int obstacle_percent = 10 + rand() % 81; // 10–90%

## Έξοδος – Παράδειγμα
![1](/images/astar1.png)

![2](/images/astar2.png)

🛠️ Μεταγλώττιση & Εκτέλεση
Linux / macOS
gcc a_star.c -o a_star
./a_star

Windows (MinGW)
gcc a_star.c -o a_star.exe
a_star.exe

📌 Συναρτήσεις Κλειδιά
🔹 generate_grid()

Δημιουργεί το grid και τοποθετεί εμπόδια, start & end.

🔹 a_star()

Υλοποίηση του A* με:

Min heap

Manhattan distance

Reconstruction του path

🔹 heap_insert(), heap_extract()

Υλοποίηση binary min heap.

🔹 print_grid()

Εκτυπώνει grid + path.
