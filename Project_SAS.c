#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

typedef struct {
    char username[50];
    char password[50];
    int role; // 1: Admin, 2: Agent, 3: Client
    int failed_attempts;
    time_t lockout_time;
    int all_rec[50];
} User;

typedef struct {
    int id;
    char motif[100];
    char description[255];
    char categorie[50];
    char status[20];
    char date[20];
    char client[50];
    int priority;
} Reclamation;

User users[100];
Reclamation rec[100];
int Indexfree = 1;
int IndexRec = 0;
//fanction pour ajouter une reclamation
void ajouterRec();
//fanctions primaire 
void tri_by_name();
void fordelay();
void displayMenu();
void signUp();
void menu_client();
void menu_ajent();
void menu_admin();
int signIn();
bool isCorrect_MDP();
int main();

//factions lié au menu admin

void afficherReclamations();
void modifierReclamation();
void supprimerReclamation();
void traiterReclamation();
void rechercherReclamation();
void attribuerPriorite();
void trierParPriorite();
void gererRoles();
void afficherStatistiques();


// Fonction pour afficher le menu
void displayMenu() {
    system("cls");
    printf("\t\t\tBienvenue !\n\n\n");
    printf("\t1. Sign In\n");
    printf("\t2. Sign Up\n");
    printf("\t3. Users\n\n");
    printf("\t4. Quitter\n");
    printf("\t\t\t\tChoisissez une option: ");
}

void signUp(User *user) {
    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin@2024");
    users[0].role = 1;

    printf("Inscription:\n");
    printf("Entrez votre nom d'utilisateur: ");
    scanf("%s", user-> username);
    Invalid_signUP_pass:
    printf("Entrez votre mot de passe: ");
    scanf("%s", user-> password);
    if (!isCorrect_MDP(user-> password)) {
            printf("Mot de passe incorrect. Veuillez reessayer.\n");
            printf("%s\n",user->password);
            goto Invalid_signUP_pass;
            
    }
    
    strcpy(users[Indexfree].username, user->username);
    strcpy(users[Indexfree].password, user->password);
    users[Indexfree].role = 3;
    system("cls");
    printf("\n\n\n\n\n\t\t\t\tInscription reussie !");
    Indexfree++; 
    tri_by_name();
    fordelay();
    menu_client();
}


// Fonction pour vérifier la validité du mot de passe
bool isCorrect_MDP(char *mdp) {
    int car = 0, alph_UP = 0, num = 0, alph_LOW = 0;
    for (int i = 0; i < strlen(mdp); i++) {
        if (mdp[i] >= 'A' && mdp[i] <= 'Z') {
            alph_UP = 1;
        } else if (mdp[i] >= 'a' && mdp[i] <= 'z') {
            alph_LOW = 1;
        } else if (isdigit(mdp[i])) {
            num = 1;
        } else if ((mdp[i] >= '!' && mdp[i] <= '/') || (mdp[i] >= ':' && mdp[i] <= '@')) {
            car = 1;
        }
    }
    printf("%d-%d-%d-%d-%d\n", car, alph_LOW, alph_UP, num,strlen(mdp));
    return car && alph_UP && alph_LOW && num && strlen(mdp) == 8;
}

// Fonction pour la connexion
int signIn(User *user) {
    int cote = 0;
    char username[50];
    char password[50];
    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin@2024");
    users[0].role = 1;

    while (1) {
        printf("Connexion:\n");
        printf("Entrez votre nom d'utilisateur: ");
        scanf("%s", username);
        printf("Entrez votre mot de passe: ");
        scanf("%s", password);
        int role;
        //checker le nombre de failde de mdp aprés 3 essey va veilleiz 180
        for (int i = 0; i < Indexfree; i++) {
        if (strcmp(users[i].username, username) == 0) {
            time_t current_time = time(NULL);
            if (users[i].failed_attempts == 3 && difftime(current_time, users[i].lockout_time) < 180) {
                printf("Compte verrouille. Veuillez reessayer apres %d secondes.\n", (int)(180 - difftime(current_time, users[i].lockout_time)));
                return 0;
            }
            if (strcmp(users[i].password, password) == 0) {
                users[i].failed_attempts = 0;
                printf("Connexion reussie. Bienvenue, %s!\n", users[i].username);
                break;
            } else {
                users[i].failed_attempts++;
                if (users[i].failed_attempts >= 3) {
                    users[i].lockout_time = current_time;
                    printf("Compte verrouille apres %d tentatives echouees. Veuillez reessayer apres %d secondes.\n", 3, 180);
                } else {
                    printf("Identifiant ou mot de passe incorrect. Tentatives restantes : %d\n", 3 - users[i].failed_attempts);
                }
                return 0;
            }
        }
        }
        //checker le role du persone qui connecter correctement
        for (int i = 0; i < Indexfree; i++)
        {
            if ((strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0))
            {
                role = users[i].role;
            }
            
        }

        if (role == 1) {
            cote = 1;
            break;
        } else if (role == 2) {
            cote = 2;
            break;
        } else if (role == 3) {
            cote = 3;
            printf("Connexion reussie !\n");
            break;
        } else {
            printf("Nom d'utilisateur ou mot de passe incorrect.\n");
            inv_aaa:
            printf("Entrer 1 pour reantrer les donner ou 0 pour revenir au page d'accuelle : ");
            int aaa;
            scanf("%d",&aaa);
            switch (aaa)
            {
            case 0:
                main();
                break;
            case 1:
                break;
            
            default:
                printf("invalid Input !");
                goto inv_aaa;
                break;
            }
        }
    }
    return cote;
}

void menu_admin() {
    int choix;
    system("cls");
    printf("\t\t\tWelcome dans l'administrattion\n");
    printf("\t\t\t!!!!! WELCOME ADMIN !!!!\n");
    printf("1. Afficher la liste des reclamations.\n");
    printf("2. Modifier une reclamation.\n");
    printf("3. Supprimer une reclamation.\n");
    printf("4. Traiter une reclamation.\n");
    printf("5. Rechercher une reclamation\n");
    printf("6. Trier par priorite\n");
    printf("7. Gerer les roles\n");
    printf("8. Statistiques\n");
    printf("9. Deconxion\n");
    printf("\t\tEntrer votre choix : ");
    scanf("%d", &choix);

    int id;
    switch (choix) {
        case 1:
            afficherReclamations();
            break;
        case 2:
            printf("Entrez l'ID de la réclamation à modifier : ");
            scanf("%d", &id);
            modifierReclamation(id);
            break;
        case 3:
            printf("Entrez l'ID de la réclamation à supprimer : ");
            scanf("%d", &id);
            supprimerReclamation(id);
            break;
        case 4:
            printf("Entrez l'ID de la réclamation à traiter : ");
            scanf("%d", &id);
            traiterReclamation(id);
            break;
        case 5:
            printf("Entrez l'ID de la réclamation à rechercher : ");
            scanf("%d", &id);
            rechercherReclamation(id);
            break;
        case 6:
            trierParPriorite();
            break;
        case 7:
            gererRoles();
            break;
        case 8:
            afficherStatistiques();
            break;
        case 9:
            printf("\nDeconexion en cours ");
            fordelay();
            main();
            break;
        default:
            printf("Option invalide. Veuillez reessayer.\n");
    }
}

void afficherReclamations() {
    for (int i = 0; i < IndexRec; i++) {
        printf("ID: %d\n", rec[i].id);
        printf("Motif: %s\n", rec[i].motif);
        printf("Description: %s\n", rec[i].description);
        printf("Catégorie: %s\n", rec[i].categorie);
        printf("Status: %s\n", rec[i].status);
        printf("Date: %s\n", rec[i].date);
        printf("Client: %s\n\n", rec[i].client);
    }
}

void modifierReclamation(int id) {
    for (int i = 0; i < IndexRec; i++) {
        if (rec[i].id == id) {
            printf("Entrez le nouveau motif : ");
            scanf(" %s", rec[i].motif);
            printf("Entrez la nouvelle description : ");
            scanf(" %s", rec[i].description);
            printf("Entrez la nouvelle categorie : ");
            scanf(" %s", rec[i].categorie);
            printf("Entrez le nouveau status : ");
            scanf(" %s", rec[i].status);
            printf("Reclamation modifiee avec succes.\n");
            return;
        }
    }
    printf("Reclamation non trouvee.\n");
}

void supprimerReclamation(int id) {
    for (int i = 0; i < IndexRec; i++) {
        if (rec[i].id == id) {
            for (int j = i; j < IndexRec - 1; j++) {
                rec[j] = rec[j + 1];
            }
            IndexRec--;
            printf("Réclamation supprimée avec succès.\n");
            return;
        }
    }
    printf("Réclamation non trouvée.\n");
}

void traiterReclamation(int id) {
    for (int i = 0; i < IndexRec; i++) {
        if (rec[i].id == id) {
            printf("Entrez le nouveau status (en cours, résolue, fermée) : ");
            scanf(" %s", rec[i].status);
            printf("Réclamation traitée avec succès.\n");
            return;
        }
    }
    printf("Réclamation non trouvée.\n");
}

void rechercherReclamation(int id) {
    for (int i = 0; i < IndexRec; i++) {
        if (rec[i].id == id) {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Catégorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %s\n", rec[i].date);
            printf("Client: %s\n\n", rec[i].client);
            return;
        }
    }
    printf("Reclamation non trouvee.\n");
}

void attribuerPriorite(Reclamation *rec) {
    if (strstr(rec->description, "urgent") != NULL || strstr(rec->description, "immediat") != NULL) {
        strcpy(rec->status, "haute");
    } else if (strstr(rec->description, "important") != NULL) {
        strcpy(rec->status, "moyenne");
    } else {
        strcpy(rec->status, "basse");
    }
}

void trierParPriorite() {
    for (int i = 0; i < IndexRec; i++) {
        attribuerPriorite(&rec[i]);
    }
    // Tri par priorité (haute, moyenne, basse)
    for (int i = 0; i < IndexRec - 1; i++) {
        for (int j = i + 1; j < IndexRec; j++) {
            if (strcmp(rec[i].status, rec[j].status) > 0) {
                Reclamation temp = rec[i];
                rec[i] = rec[j];
                rec[j] = temp;
            }
        }
    }
    printf("Réclamations triées par priorité.\n");
}

void gererRoles() {
    char *role1[] = {"Admin","Ajent","Client"};
    char username[50];
    int newRole;
    printf("Entrez le nom d'utilisateur : ");
    scanf("%s", username);
    

    for (int i = 0; i < Indexfree; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("%s est de role %s.\nEntrez le nouveau role (1: Admin, 2: Agent, 3: Client) : ",users[i].username,role1[users[i].role-1]);
            scanf("%d", &newRole);
            users[i].role = newRole;
            printf("Role mis a jour avec succes.\n");
            return;
        }
    }
    printf("Utilisateur non trouve.\n");
}

void afficherStatistiques() {
    int totalReclamations = IndexRec;
    int resolues = 0;
    for (int i = 0; i < IndexRec; i++) {
        if (strcmp(rec[i].status, "resolue") == 0) {
            resolues++;
        }
    }
    printf("Nombre total de reclamations : %d\n", totalReclamations);
    printf("Nombre de reclamations resolues : %d\n", resolues);
    printf("Taux de resolution : %.2f%%\n", (resolues / (float)totalReclamations) * 100);
}









void menu_ajent() {
    printf("C'est le cote agent\n");
    fordelay();
}

void menu_client() {
    system("cls");
    int choix;
    printf("\t\tC'est le cote client\n");
    printf("1. Ajouter une reclamation\n");
    printf("2. modifer mon reclamation\n");
    printf("3. Voir votre etat\n");
    printf("4. Deconexion\n");
    printf("\t\tEntrer votre choix : ");
    scanf("%d",&choix);
    switch (choix)
    {
    case 1:
        ajouterRec();
        break;
    case 4:
        printf("\nDeconexion en cours ");
        fordelay();
        main();
        break;
    
    default:
        break;
    }
    
    
    fordelay();
}
void ajouterRec() {
    system("cls");
    int new_id = IndexRec+1;
    
    printf("Nouvelle reclamation de l'ID : %d\n",new_id);
    printf("Entrer le motif (ne depasse pas (100) caracter ) :\n");
    getchar();
    fgets(rec[IndexRec].motif,sizeof(rec[IndexRec].motif),stdin);
    // printf("\nle motif est : %s",rec[IndexRec].motif);
    printf("Entrer la description (ne depasse pas (255) caracter) :\n");
    getchar();
    fgets(rec[IndexRec].description,sizeof(rec[IndexRec].description),stdin);
    printf("Entrer une categorie :\n");
    getchar();
    fgets(rec[IndexRec].categorie,sizeof(rec[IndexRec].categorie),stdin);
    printf("Entrer une status :\n");
    getchar();
    fgets(rec[IndexRec].status,sizeof(rec[IndexRec].status),stdin);
    printf("Entrer la date d'aujorduit (jj/mm/aaaa) :\n");
    getchar();
    fgets(rec[IndexRec].date,sizeof(rec[IndexRec].date),stdin);
    printf("Entrer votre username exact : ");
    char *temp_name;
    scanf("%s",&temp_name);
    for (int i = 0; i < Indexfree; i++)
    {
        if (strcmp(users[i].username,temp_name) == 0)
        {
            if (sizeof(users[i].all_rec)/4 != 0)
            {
                for (int j = 0; j <= sizeof(users[i].all_rec)/4; j++)
                {
                    if (users[i].all_rec[j] == 0)
                    {
                    users[i].all_rec[j] = new_id;
                    printf("Votre reclamation ajouter succesfuly !!");
                    fordelay();
                    break;
                        }
                
                }
            }else {
                users[i].all_rec[0] = new_id;
                printf("Votre reclamation ajouter succesfuly !!");
                    fordelay();
                    break;
            }
            
            
            
            
        }
        
    }
    
    rec[IndexRec].id = new_id;
    IndexRec++;
    menu_client();
}

void aff_rec_admin(int my_rec[]) {
    for (int i = 0; my_rec[i] != 0; i++)
    {
        for (int j = 0; j < IndexRec; j++)
        {
            if (my_rec[i] == rec[j].id)
            {
                int ccc;
                printf("%s",rec[j].motif);
                printf("Entrer 0 to return to menu");
                scanf("%d",&ccc);
                if (ccc == 0)
                {
                    menu_client();
                }
                
                
                break;
            }
            
        }
        
        
    }
    
    
    
}


void fordelay() {
    for (int i = 0; i < 6; i++) {
        printf(".");
        Sleep(1000);
    }
}

int main() {

    User user;
    int choice;
    int isRegistered = 0;
    srand(time(NULL));
    while (true) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                switch (signIn(&user)) {
                    case 1:
                        menu_admin();
                        break;
                    case 2:
                        menu_ajent();
                        break;
                    case 3:
                        menu_client();
                        break;
                    default:
                        printf("Veuillez vous inscrire.\n");
                        fordelay();
                        break;
                }
                break;
            case 2:
                signUp(&user);
                isRegistered = 1;
                break;
            case 3:
                for (int i = 0; i < Indexfree; i++)
                {
                    printf("name : %s\nmdp : %s\nrole : %d\n",users[i].username,users[i].password,users[i].role);
                }
                int re ;
                while (true)
                {

                    printf("\tEntrer 0 to return to menu : ");
                    scanf("%d",&re);
                    if (re != 0)
                    {
                        system("cls");
                        printf("Invalid Input");
                        continue;
                    }
                    break;
                    
                }
                main();
                
                break;
            case 4:
                printf("Au revoir !\n");
                exit(0);
            default:
                printf("Option invalide. Veuillez reessayer.\n");
        }
    }
    
    
    return 0;
}
void tri_by_name() {
    for (int i = 2; i < Indexfree - 1; i++) {
        for (int j = 0; j < Indexfree - i - 1; j++) {
            if (strcmp(users[j].username, users[j+1].username) > 0) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}
