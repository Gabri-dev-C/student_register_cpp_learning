/*
 * Programma: Registro studenti
 * Autore: Gabriele Henriet
 * Data: Settembre 2025
 * Scopo: Gestire i dati degli studenti con un registro elettronico
 */

#include <iostream>
#include <cstring>
#include <cstdio>
#include <iomanip>
using namespace std;

const int LUNG_NOME = 20;
const int LUNG_COGNOME = 20;
const int ALUNNI = 100;	        // numero massimo alunni


// struttura dati per ogni studente
struct s_alunno{
    int posizione;		        // posizione dello studente nel registro
    char nome[LUNG_NOME];		// nome dello studente
    char cognome[LUNG_COGNOME];	// cognome dello studente
    int classe;		            // classe dello studente
    char sezione;		        // sezione dello studente
};

// prototipi delle funzioni
// ad ogni funzione che modifica il numero degli elementi nel
// registro, viene passata la lunghezza per riferimento.
void inserisci_alunno(s_alunno registro[], int& num_alunni);
bool cerca_alunno_cognome(s_alunno registro[], int& num_alunni, char stringa[]);
void stampa_dati_alunno(s_alunno registro[], int pos);
void stampa_tabella(s_alunno registro[], int& num_alunni);
void ordina_registro(s_alunno registro[], int& num_alunni);
void elimina_alunno(s_alunno registro[], int& num_alunni, int pos);
void elimina_alunno_wrapper(s_alunno registro[], int& num_alunni);
void modifica_dati_alunno(s_alunno registro[], int& num_alunni);
void carica_registro(s_alunno registro[], int& num_alunni, FILE* file);
void salva_registro(s_alunno registro[], int& num_alunni);
void ordina_registro_pos(s_alunno registro[], int& num_alunni);
void pulisci_schermo();
void premi_invio();

int main(){
    s_alunno tutti_alunni[ALUNNI];	// definizione tabella di dipo s_alunno
    int num_alunni = 0;			    // indice del primo elemnto libero nella tabella
    char scelta;				    // operazione da svolgere
    char alunno[20];				// stringa per ricerca alunno

    // Creazione del file registro.txt
    FILE *punta_file;
    if(fopen_s(&punta_file, "registro.txt", "r") != 0){
        if(fopen_s(&punta_file, "registro.txt", "w") != 0){
            cout << "ERRORE: il file non può essere creato";
            premi_invio();
            return -1;
        }else{
            if(fclose(punta_file) != 0){
                cout << "ERRORE: Il programma verrà chiuso";
                premi_invio();
                return -1;
            }
        }
    }

    // caricamento in memoria dei dati
    carica_registro(tutti_alunni, num_alunni, punta_file);
    fclose(punta_file);

    do{
        pulisci_schermo();

        // menu principale
        cout << "Inserisci alunno...................... 1" << endl;
        cout << "Cerca Alunno.......................... 2" << endl;
        cout << "Visualizza registro................... 3" << endl;
        cout << "Ordina registro per cognome........... 4" << endl;
        cout << "Elimina alunno........................ 5" << endl;
        cout << "Modifica dati alunno.................. 6" << endl;
        cout << "Salva registro........................ 7" << endl;
        cout << "Esci.................................. 9" << endl;
        cout << endl;
        cout << "Inserisci la tua scelta: ";
        cin >> scelta;
        cin.ignore();

        // flusso di elaborazione
        switch (scelta){
        case '1':
            inserisci_alunno(tutti_alunni, num_alunni);
            break;
        case '2':
            cout << endl;
            cout << "Inserisci il cognome: ";
            cin.getline(alunno, 20);
            cerca_alunno_cognome(tutti_alunni, num_alunni, alunno);
            premi_invio();

            break;
        case '3':
            stampa_tabella(tutti_alunni, num_alunni);
            break;
        case '4':
            ordina_registro(tutti_alunni, num_alunni);
            break;
        case '5':
            elimina_alunno_wrapper(tutti_alunni, num_alunni);
            break;
        case '6':
            modifica_dati_alunno(tutti_alunni, num_alunni);
            break;
        case '7':
            salva_registro(tutti_alunni, num_alunni);
            break;
        }

        alunno[0] = '\0';

    }while(scelta != '9');

    return 0;
}

// inserimento del nuovo alunno nella posizione num_alunni
void inserisci_alunno(s_alunno registro[], int& num_alunni){
    registro[num_alunni].posizione = num_alunni + 1;
    cout << "Inserisci il nome: ";
    cin.getline(registro[num_alunni].nome, LUNG_NOME);
    cout << "Inserisci il cognome: ";
    cin.getline(registro[num_alunni].cognome, LUNG_COGNOME);
    cout << "Inserisci la classe: ";
    cin >> registro[num_alunni].classe;
    cin.ignore();
    cout << "Inserisci la sezione: ";
    cin >> registro[num_alunni].sezione;
    cin.ignore();
    num_alunni++;
}

// stampa una tabella con tutti gli alunni del registro
void stampa_tabella(s_alunno registro[], int& num_alunni){
    int larg_posizione = 10;
    int larg_nome = 15;
    int larg_cognome = 15;
    int larg_classe = 10;
    int larg_sezione = 10;
    int larg_totale = larg_posizione + larg_nome + larg_cognome + larg_classe +
        larg_sezione + 14;

    // header della tabella
    cout << "+" << setfill('-') << setw(larg_totale) << "+" << endl;
    cout << "|" << setfill(' ')
        << setw(larg_posizione) << "Posizione" << " | "
        << setw(larg_nome) << "Nome" << " | "
        << setw(larg_cognome) << "Cognome" << " | "
        << setw(larg_classe) << "Classe" << " | "
        << setw(larg_sezione) << "Sezione" << " | " << endl;
    cout << "+" << setfill('-') << setw(larg_totale) << "+" << endl;

    // riga della tabella
    int cont = 0;
    while(cont < num_alunni){
        cout << "|" << setfill(' ')
            << setw(larg_posizione) << registro[cont].posizione << " | "
            << setw(larg_nome) << registro[cont].nome << " | "
            << setw(larg_cognome) << registro[cont].cognome << " | "
            << setw(larg_classe) << registro[cont].classe << " | "
            << setw(larg_sezione) << registro[cont].sezione << " | " << endl;
        cont++;
    }

    // footer tabella
    cout << "+" << setfill('-') << setw(larg_totale) << "+" << endl;
    premi_invio();
}

// cerca il cognome e visualizza i dati degli alunni
bool cerca_alunno_cognome(s_alunno registro[], int& num_alunni, char stringa[]){
    int cont = 0;			        // contatore del vettore registro
    int larghezza_cella = 20;       // per la tabella di stampa
    bool trovato = false;

    // cerco in ogni cella occupata del registro
    while(cont < num_alunni){
        // quando ho una corrispondenza visualizzo i dati
        if(strcmp(registro[cont].cognome, stringa) == 0){
            cout << endl;
            cout << setfill(' ');
            cout << setw(larghezza_cella) << "Posizione: " << setw(larghezza_cella)
                << registro[cont].posizione << endl;
            cout << setw(larghezza_cella) << "Nome: " << setw(larghezza_cella)
                << registro[cont].nome << endl;
            cout << setw(larghezza_cella) << "cognome: " << setw(larghezza_cella)
                << registro[cont].cognome << endl;
            cout << setw(larghezza_cella) << "Classe: " << setw(larghezza_cella)
                << registro[cont].classe << endl;
            cout << setw(larghezza_cella) << "Sezione: " << setw(larghezza_cella)
                << registro[cont].sezione << endl;
            cout << endl;
            trovato = true;
        }
        cont++;
    }
    return trovato;
}

// ordina il registro per cognome con bubble-sort
void ordina_registro(s_alunno registro[], int& num_alunni){
    s_alunno alunno_temp;		// variabile di tipo s_alunno temporanea
    for(int i = 0; i < num_alunni; i++){
        for(int j = 0; j < num_alunni - i - 1; j++){

            // confronto tra le stringhe cognome
            if(strcmp(registro[j].cognome, registro[j + 1].cognome) > 0){
                // swap
                alunno_temp = registro[j];
                registro[j] = registro[j+1];
                registro[j+1] = alunno_temp;
            }
        }
    }
    cout << "Registro ordinato con successo!";
    premi_invio();
}

// ordina il registro per posizione con bubble-sort
void ordina_registro_pos(s_alunno registro[], int& num_alunni){
    s_alunno alunno_temp;		// variabile di tipo s_alunno temporanea

    for(int i = 0; i < num_alunni; i++){
        for(int j = 0; j < num_alunni - i - 1; j++){
            if(registro[j].posizione > registro[j + 1].posizione){
                // swap
                alunno_temp = registro[j];
                registro[j] = registro[j+1];
                registro[j+1] = alunno_temp;
            }
        }
    }
}

// gestione dell'input utente e trova posizione da eliminare
void elimina_alunno_wrapper(s_alunno registro[], int& num_alunni){
    char cognome_alunno[LUNG_COGNOME];
    char risposta, conferma;
    int pos;

    cout << "inserisci il cognome dell'alunno da eliminare: ";
    cin.getline(cognome_alunno, LUNG_COGNOME);

    if(cerca_alunno_cognome(registro, num_alunni, cognome_alunno)){
        cout << endl;
        cout << "L'alunno e' tra questi? s/n: ";
        cin >> risposta;
        cin.ignore();

        if(risposta == 's'){
            cout << "Inserisci la sua posizione: ";
            cin >> pos;
            cin.ignore();
            cout << "Sicuro di volere eliminarlo? s/n: ";
            cin >> conferma;
            cin.ignore();
            if(conferma == 's'){
                elimina_alunno(registro, num_alunni, pos);
                cout << "Alunno eliminato con successo";
                premi_invio();
            }
        }else{
            cout << "ERRORE: Alunno non trovato";
            premi_invio();
        }
    }else{
        cerr << "ERRORE: la ricerca non ha prodotto risultati";
        premi_invio();
    }
}

// elimina alunno nella posizione pos
void elimina_alunno(s_alunno registro[], int& num_alunni, int pos){
    if (pos < 1 || pos > num_alunni)	// controllo sull'input dell'utente
        return;

    // shift a sinistra
    for(int i = pos - 1; i < num_alunni - 1; i++)
        registro[i] = registro[i+1];
    num_alunni--;		                // aggiorno l'indice della prima posizione libera

    // aggiorno le posizioni degli elemnti
    for(int i = pos - 1; i < num_alunni; i++){
        registro[i].posizione = i + 1;
    }
}

// cerca alunno per cognome, visualizza dati, chiede conferma
// e modifica i dati scelti
void modifica_dati_alunno(s_alunno registro[], int& num_alunni){
    char cognome_alunno[LUNG_COGNOME];
    char risposta, conferma, scelta;
    int pos;

    cout << "Inserisci il cognome dell'alunno da modificare: ";
    cin.getline(cognome_alunno, LUNG_COGNOME);

    if(cerca_alunno_cognome(registro, num_alunni, cognome_alunno)){

        cout << endl;
        cout << "L'alunno e' tra questi? s/n ";
        cin >> risposta;
        cin.ignore();

        if(risposta == 's'){
            cout << "Inserisci la sua posizione: ";
            cin >> pos;
            cin.ignore();
            if((pos < 1) || (pos > num_alunni)){
                cout << "ERRORE: inserimento sbagliato";
                premi_invio();
                return;
            }else{
                do{
                pulisci_schermo();
                    cout << "Quali dati vuoi modificare?" << endl;
                    cout << endl;
                    cout << "Nome........................... 1" << endl;
                    cout << "Cognome........................ 2" << endl;
                    cout << "Classe......................... 3" << endl;
                    cout << "Sezione........................ 4" << endl;
                    cout << "Esci........................... 9" << endl;

                    cin >> scelta;
                    cin.ignore();

                    switch (scelta){
                    case '1':
                        cout << "Inserisci il nuovo nome: ";
                        cin.getline(registro[pos-1].nome, LUNG_NOME);
                        break;
                    case '2':
                        cout << "inserisci il nuovo cognome: ";
                        cin.getline(registro[pos-1].cognome, LUNG_COGNOME);
                        break;
                    case '3':
                        cout << "inserisci la nuova classe: ";
                        cin >> registro[pos-1].classe;
                        cin.ignore();
                        break;
                    case '4':
                        cout << "Inserisci la nuova sezione: ";
                        cin >> registro[pos-1].sezione;
                        cin.ignore();
                        break;
                    }
                }while(scelta != '9');
            }
        }else{
            cerr << "ERRORE: l'alunno non e' presente nel registro";
            premi_invio();
        }
    }else{
        cerr << "ERRORE: l'alunno non e' presente nel registro";
        premi_invio();
    }
}

// legge i dati dal file e li carica nel registro
void carica_registro(s_alunno registro[], int& num_alunni, FILE* file){
    char buffer[200];
    char campo[50];
    char c;                     // carattere letto dal file
    int iBuf;                   // indice di buffer[]
    int iCam;                   // indice di campo[]
    int campo_corrente;         // contatore dei campi


    while((c = fgetc(file)) != EOF){
        if(c == '\n'){              // ho letto una riga e la processo
            buffer[iBuf++] = '\n';  //
            buffer[iBuf] = '\0';    // aggiungo fine stringa
            iBuf = 0;
            iCam = 0;
            campo_corrente = 0;

            // leggo i campi e li memorizzo in campo[]
            while(buffer[iBuf] != '\0'){
                if(buffer[iBuf] != ';' && buffer[iBuf] != '\n'){
                    campo[iCam] = buffer[iBuf];
                    iBuf++;
                    iCam++;
                }else{
                    campo[iCam] = '\0';
                    iBuf++;
                    iCam = 0;
                    campo_corrente++;

                    // faccio il parsing
                    switch(campo_corrente){
                    case 1:
                        registro[num_alunni].posizione = atoi(campo);
                        break;
                    case 2:
                        strcpy(registro[num_alunni].nome, campo);
                        break;
                    case 3:
                        strcpy(registro[num_alunni].cognome, campo);
                        break;
                    case 4:
                        registro[num_alunni].classe = atoi(campo);
                        break;
                    case 5:
                        registro[num_alunni].sezione = campo[0];
                        break;
                    }
                }
            }
            num_alunni++;
            iBuf = 0;
        }else{
            buffer[iBuf] = c;
            iBuf++;
        }

    }
}

// riscrive il file ogni volta
void salva_registro(s_alunno registro[], int& num_alunni){
    FILE* file;

    char buffer[200] = "";  // inizializzazione del buffer[], tutte le celle prendono '\0'.
    int i = 0;              // contatore degli alunni
    int j = 0;              // indice di buffer[]
    char classe[10] = "";   // vettore di appoggio per scrivere la classe

    ordina_registro_pos(registro, num_alunni);

    //apertura del file
    if(fopen_s(&file, "registro.txt", "w") != 0){
        cerr << "ERRORE: il file non puo' essere aperto";
        premi_invio();
        return;
    }

    //scansione sequenziale di tutto il registro
    while(i < num_alunni){

        buffer[0] = '\0'; // resetto il buffer

        // converte l'intero in stringa e lo mette in buffer[]
        sprintf(buffer, "%d", registro[i].posizione);
        // concatena punto e virgola
        strcat(buffer, ";");

        strcat(buffer, registro[i].nome);
        strcat(buffer, ";");

        strcat(buffer, registro[i].cognome);
        strcat(buffer, ";");

        // converte l'intero in stringa e lo mette in classe[]
        // serve vettore di appoggio altrimenti sprintf() avrebbe
        // sovrascritto buffer[]
        sprintf(classe, "%d", registro[i].classe);
        strcat(buffer, classe);
        strcat(buffer, ";");

        int lung = strlen(buffer);
        buffer[lung] = registro[i].sezione;         // singolo carattere

        buffer[lung+1] = '\0';
        strcat(buffer, "\n");

        j = 0;
        // scrittura di buffer[] nel file,
        // carattere per carattere
        while(buffer[j] != '\0'){
            if(fputc(buffer[j], file) == EOF){      // se non scrive ritorna EOF
                cerr << "ERRORE in scrittura del file";
                premi_invio();
                return;
            }
            j++;
        }
        i++;
    }
    fclose(file);
    cout << "Registro salvato con successo";
    premi_invio();
}

void pulisci_schermo(){
    if (system("CLS") != 0)     // per windows
    system("clear");            // per linux
}

void premi_invio(){
    cout << endl << endl;
    cout << "Premi INVIO per continuare";
    cin.get();
}

