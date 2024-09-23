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
    int sec,min,heur,jour,moi,anne;
}Date;
typedef struct {
    int id;
    char motif[100];
    char description[255];
    char categorie[50];
    char status[20];// 1: "en cours",2: "résolue", ou 3: "fermée".
    Date date;
    Date traitment_date;
    char client[50];
    char priority[50];
    char notes[50]; //note sur le traitement 
} Reclamation;

User users[100];
Reclamation rec[100];
int Indexfree = 1;
int IndexRec = 0;
int anti_spam = 0;
//fanction pour ajouter une reclamation
void ajouterRec();
void aff_rec_client();
void modifier_rec_client();
void supprimer_Rec_client();
void Rapport_journalier();

//fanctions primaire 
void tri_by_name();
void fordelay();
void server_down();
void displayMenu();
void signUp();
void menu_client();
void menu_ajent();
void menu_admin();
int* signIn();
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
    menu_client(user);
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
int* signIn(User *user) {
    static int result[2];
    int cote = 0;
    int user_found = 0;
    char username[50];
    char password[50];
    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin@2024");
    users[0].role = 1;
    int index_user;
    
        while (anti_spam < 3) {
        printf("Connexion:\n");
        printf("Entrez votre nom d'utilisateur: ");
        scanf("%s", username);
        for (int i = 0; i <= Indexfree; i++)
        {
            if (strcmp(users[i].username,username)==0)
            {
                user_found = 1;
                break;
            }
            
        }
        if (user_found == 0)
        {
            anti_spam++;
            continue;
        }
        Invalid_paaaaass:
        printf("Entrez votre mot de passe: ");
        scanf("%s", password);
        int role;
        //checker le nombre de failde de mdp aprés 3 essey va veilleiz 180
        for (int i = 0; i <= Indexfree; i++) {
        if (strcmp(users[i].username, username) == 0) {
            time_t current_time = time(NULL);
            if (users[i].failed_attempts == 3 && difftime(current_time, users[i].lockout_time) < 180) {
                printf("Compte verrouille. Veuillez reessayer apres %d secondes.\n", (int)(180 - difftime(current_time, users[i].lockout_time)));
                fordelay();
                main();
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
                    fordelay();
                    main();
                    return 0;
                } else {
                    printf("Identifiant ou mot de passe incorrect. Tentatives restantes : %d\n", 3 - users[i].failed_attempts);
                    anti_spam++;
                    goto Invalid_paaaaass;
                }
                
            }
        }
        }
        //checker le role du persone qui connecter correctement
        for (int i = 0; i <= Indexfree; i++)
        {
            if ((strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0))
            {
                role = users[i].role;
                index_user = i;
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
    if (anti_spam == 3)
    {
        server_down();
        
    }
    
    
    
    
    result[0] = cote;
    result[1] = index_user;
    printf("SingIn succesfule");
    fordelay();

    return result;
}

void menu_admin() {
    int choix;
    system("cls");
    printf("\t\t\tWelcome dans l'administrattion\n");
    printf("\t\t\t!!!!! WELCOME ADMIN !!!!\n");
    printf("1. Ajouter une reclamation\n");
    printf("2. Afficher la liste des reclamations.\n");
    printf("3. Modifier une reclamation.\n");
    printf("4. Supprimer une reclamation.\n");
    printf("5. Traiter une reclamation.\n");
    printf("6. Rechercher une reclamation\n");
    printf("7. Afficher par priorite\n");
    printf("8. Gerer les roles\n");
    printf("9. Statistiques\n");
    printf("10. Generer Un rapport journalier\n");
    printf("11. Deconxion\n");
    printf("\t\tEntrer votre choix : ");
    scanf("%d", &choix);

    int id;
    switch (choix) {
        case 1:
            ajouterRec(&users[0]);
            return;
        case 2:
            afficherReclamations(&users[0]);
            break;
        case 3:
            printf("Entrez l'ID de la reclamation a modifier : ");
            scanf("%d", &id);
            modifierReclamation(id,&users[0]);
            break;
        case 4:
            printf("Entrez l'ID de la reclamation a supprimer : ");
            scanf("%d", &id);
            supprimerReclamation(id,&users[0]);
            break;
        case 5:
            printf("Entrez l'ID de la reclamation a traiter : ");
            scanf("%d", &id);
            traiterReclamation(id,&users[0]);
            break;
        case 6:
            printf("Entrez l'ID de la reclamation a rechercher : ");
            scanf("%d", &id);
            rechercherReclamation(id,&users[0]);
            break;
        case 7:
            trierParPriorite();
            break;
        case 8:
            gererRoles();
            break;
        case 9:
            afficherStatistiques();
            break;
        case 10:
            Rapport_journalier();
            break;
        case 11:
            printf("\nDeconexion en cours ");
            fordelay();
            main();
            break;
        default:
            printf("Option invalide. Veuillez reessayer.\n");
    }
}

void afficherReclamations(User *user_menu) {
    int choix;

    for (int i = 0; i < IndexRec; i++) {
        printf("ID: %d\n", rec[i].id);
        printf("Motif: %s\n", rec[i].motif);
        printf("Description: %s\n", rec[i].description);
        printf("Categorie: %s\n", rec[i].categorie);
        printf("Status: %s\n", rec[i].status);
        printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
        printf("Client: %s\n\n", rec[i].client);
    }
    if (IndexRec == 0)
    {
        printf("Il n'y a aucune reclamation\n");
    }
    
    Invalid_choix_affichreclamation:
    printf("Entrer 0 pour retourner au menu : ");
    scanf("%d",&choix);
    if (choix == 0)
    {
        if (user_menu->role == 2)
        {
            menu_ajent(user_menu);
        }
        
        menu_admin();
    }else {
        printf("Invalid choix");
        goto Invalid_choix_affichreclamation;
    }
    
    
    
}

void modifierReclamation(int id,User *user_menu) {
    int choix,choix1;
    for (int i = 0; i <= IndexRec; i++) {
        if (rec[i].id == id) {
            printf("1. modifier le motif.\n2. modifier la description. \n3. modifier la categorie \n4. modifier all choix.\n\tEntrer votre choix : ");
            scanf("%d",&choix);
            switch (choix)
            {
            case 1:
                printf("Entrez le nouveau motif : ");
                scanf(" %s", rec[i].motif);
                break;
            case 2:
                printf("Entrez la nouvelle description : ");
                scanf(" %s", rec[i].description);
                break;
            case 3:
                printf("Entrez la nouvelle categorie : ");
                scanf(" %s", rec[i].categorie);
                break;
            case 4:
                printf("Entrez le nouveau motif : ");
                scanf(" %s", rec[i].motif);
                printf("Entrez la nouvelle description : ");
                scanf(" %s", rec[i].description);
                printf("Entrez la nouvelle categorie : ");
                scanf(" %s", rec[i].categorie);
                break;
            
            default:
                printf("Invalid choix ");
                fordelay();
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                
                menu_admin();
                break;
            }
            printf("Reclamation modifiee avec succes.\n");
            Invalid_choix_modif_admin_reclamation:
            printf("Entrer 0 pour retourner au menu : ");
            scanf("%d",&choix1);
            if (choix1 == 0)
            {
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                menu_admin();
            }else {
                printf("Invalid choix");
                goto Invalid_choix_modif_admin_reclamation;
            }
            
        }
    }
    printf("Reclamation non trouvee.\n");
}

void supprimerReclamation(int id, User *user_menu) {
    int choix2;
    for (int i = 0; i <= IndexRec; i++) {
        if (rec[i].id == id) {
            for (int j = i; j <= IndexRec - 1; j++) {
                rec[j] = rec[j + 1];
            }
            IndexRec--;
            printf("Reclamation supprimee avec succes.\n");
            Invalid_suppression_reclamation:
            printf("Entrer 0 pour retourner au menu : ");
            scanf("%d",&choix2);
            if (choix2 == 0)
            {
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                menu_admin();
            }else {
                printf("Invalid choix");
                goto Invalid_suppression_reclamation;
            }
            return;
        }
    }
    printf("Reclamation non trouvee.\n");
}

void traiterReclamation(int id,User *user_menu) {
    int choix,choix2;
    time_t ti = time(NULL);
    struct tm tm = *localtime(&ti);
    for (int i = 0; i <= IndexRec; i++) {
        if (rec[i].id == id) {
            Invalid_traiter_reclamation:
            printf("Entrez le nouveau status (1. en cours,2. resolue,3. fermee) : ");
            scanf("%d",&choix);
            switch (choix)
            {
            case 1:
                strcpy(rec[i].status,"en cours");
                break;
            case 2:
                strcpy(rec[i].status,"resolue");
                rec[i].traitment_date.sec = tm.tm_sec;
                rec[i].traitment_date.min = tm.tm_min;
                rec[i].traitment_date.heur = tm.tm_hour;
                rec[i].traitment_date.jour = tm.tm_mday;
                rec[i].traitment_date.moi = tm.tm_mon + 1;
                rec[i].traitment_date.jour = tm.tm_year + 1900;
                break;
            case 3:
                strcpy(rec[i].status,"rejeter");// en cours  resolue   rejeter
                rec[i].traitment_date.sec = tm.tm_sec;
                rec[i].traitment_date.min = tm.tm_min;
                rec[i].traitment_date.heur = tm.tm_hour;
                rec[i].traitment_date.jour = tm.tm_mday;
                rec[i].traitment_date.moi = tm.tm_mon + 1;
                rec[i].traitment_date.jour = tm.tm_year + 1900;
                break;
            default:
                printf("Invalide choix");
                goto Invalid_traiter_reclamation;
                break;
            }
            printf("Reclamation traitee avec succes.\n");
            Invalid_choix_retourn_trait_rec:
            printf("Entrer 0 pour retourner au menu : ");
            scanf("%d",&choix2);
            if (choix2 == 0)
            {
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                menu_admin();
            }else {
                printf("Invalid choix");
                goto Invalid_choix_retourn_trait_rec;
            }
            return;
        }
    }
    printf("Reclamation non trouvee.\n");
}

void rechercherReclamation(int id,User *user_menu) {
    int choix,choix2;
    for (int i = 0; i <= IndexRec; i++) {
        if (rec[i].id == id) {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Catégorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
            printf("Client: %s\n\n", rec[i].client);
            Invalid_choux_recher_rrec_admin:
            printf("Entrer 0 pour retourner au menu : ");
            scanf("%d",&choix2);
            if (choix2 == 0)
            {
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                menu_admin();
            }else {
                printf("Invalid choix");
                goto Invalid_choux_recher_rrec_admin;
            }
            return;
        }
    }
    printf("Reclamation non trouvee.\nEntrer 0 pour retourner au menu : ");
    scanf("%d",&choix2);
            if (choix2 == 0)
            {
                if (user_menu->role == 2)
                {
                    menu_ajent(user_menu);
                }
                menu_admin();
            }else {
                printf("Invalid choix\n");
                goto Invalid_choux_recher_rrec_admin;
            }
}

void attribuerPriorite(Reclamation *rec) {
    if (strstr(rec->description, "urgent") != NULL || strstr(rec->description, "immediat") != NULL) {
        strcpy(rec->priority, "haute");
    } else if (strstr(rec->description, "important") != NULL) {
        strcpy(rec->priority, "moyenne");
    } else {
        strcpy(rec->priority, "basse");
    }
}

void trierParPriorite() {
    int choix;
    for (int i = 0; i <= IndexRec; i++) {
        attribuerPriorite(&rec[i]);
    }
    // Tri par priorité (haute, moyenne, basse)
    for (int i = 0; i <= IndexRec - 1; i++) {
        if (strcmp(rec[i].priority,"haute") == 0)
        {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Categorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
            printf("Client: %s\n", rec[i].client);
            printf("Prioriter: %s\n\n", rec[i].priority);
        }
        
    }
    for (int i = 0; i <= IndexRec - 1; i++) {
        if (strcmp(rec[i].priority,"moyenne") == 0)
        {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Categorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
            printf("Client: %s\n", rec[i].client);
            printf("Prioriter: %s\n\n", rec[i].priority);
        }
        
    }
    for (int i = 0; i <= IndexRec - 1; i++) {
        if (strcmp(rec[i].priority,"basse") == 0)
        {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Categorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
            printf("Client: %s\n", rec[i].client);
            printf("Prioriter: %s\n\n", rec[i].priority);
        }
        
    }
    printf("Reclamations triees par priorite.\n");
    Invalid_tri_prio:
    printf("Entrer 0 pour retourner au menu : ");
    scanf("%d",&choix);
    if (choix == 0)
    {
        menu_admin();
    }else {
        printf("Invalid choix");
        goto Invalid_tri_prio;
    }
    
}

void gererRoles() {
    int choix;
    char *role1[] = {"Admin","Ajent","Client"};
    char username[50];
    int newRole;
    printf("Entrez le nom d'utilisateur : ");
    scanf("%s", username);
    

    for (int i = 0; i <= Indexfree; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("%s est de role %s.\nEntrez le nouveau role (1: Admin, 2: Agent, 3: Client) : ",users[i].username,role1[users[i].role-1]);
            scanf("%d", &newRole);
            users[i].role = newRole;
            printf("Role mis a jour avec succes.\n");
            fordelay();
            menu_admin();
            return;
        }
    }
    printf("Utilisateur non trouve.\n");
    Invalid_gere_role:
    printf("Entrer 0 pour retourner au menu : ");
    scanf("%d",&choix);
    if (choix == 0)
    {
        menu_admin();
    }else {
        printf("Invalid choix");
        goto Invalid_gere_role;
    }
}

void afficherStatistiques() {
    int choix;
    double total_delai = 0;
    int count = 0;
    int totalReclamations = IndexRec;
    int resolues = 0, en_cours = 0, rejeter = 0;// en cours  resolue   rejeter
    for (int i = 0; i <= IndexRec; i++) {
        if (strcmp(rec[i].status, "resolue") == 0) {
            resolues++;
        }
        if (strcmp(rec[i].status, "en cours") == 0) {
            en_cours++;
        }
        if (strcmp(rec[i].status, "rejeter") == 0) {
            rejeter++;
        }
        
    }
    printf("Nombre total de reclamations : %d\n", totalReclamations);
    printf("Nombre de reclamations en cours : %d\n", en_cours);
    printf("Nombre de reclamations resolues : %d\n", resolues);
    printf("Nombre de reclamations rejeter : %d\n", rejeter);
    printf("Taux de resolution : %.2f%%\n", (resolues / (float)totalReclamations) * 100);
    for (int i = 0; i <= IndexRec; i++)
    {
        if (strcmp(rec[i].status,"resolue")==0 || strcmp(rec[i].status,"rejeter")==0 )
        {
           double delai = difftime(rec[i].date.heur,rec[i].traitment_date.heur);
           total_delai += delai;
           count++;
        }
        
    }
    double moy = total_delai/count; 
    printf("Le delai moyen est : %.2lf ", moy);
    
    Invalid_Statistique_aff:
    printf("Entrer 0 pour retourner au menu : ");
    scanf("%d",&choix);
    if (choix == 0)
    {
        menu_admin();
    }else {
        printf("Invalid choix");
        goto Invalid_Statistique_aff;
    }
}
void Rapport_journalier() {
    FILE *ptr;
    ptr = fopen("rapport","w");//"r"   "w"   "a+"
    time_t t2 = time(NULL);
    struct tm tm = *localtime(&t2);
    fprintf(ptr,"Rapport de : %d/%d/%d\n",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    for (int i = 0; i <= IndexRec; i++)
    {
        if (rec[i].date.jour == tm.tm_mday)
        {
            fprintf(ptr,"Hello");
        }
        
    }
    
    fclose(ptr);
    exit(0);
}








void menu_ajent(User *ajent_menu) {
   system("cls");
    int choix,id;
    printf("\t\tC'est le cote Ajent\n");
    printf("\tWelcome %s\n",ajent_menu->username);
    printf("1. Ajouter une reclamation\n");
    printf("2. afficher les reclamation\n");
    printf("3. modifer une reclamation\n");
    printf("4. supprimer une reclamation\n");
    printf("5. traiter une reclamation\n");
    printf("6. recharcher une reclamation\n");
    printf("7. Voir votre etat\n");
    printf("8. Deconexion\n");
    printf("\t\tEntrer votre choix : ");
    scanf("%d",&choix);
    switch (choix) {
        case 1:
            ajouterRec(ajent_menu);
            return;
        case 2:
            afficherReclamations(ajent_menu);
            break;
        case 3:
            printf("Entrez l'ID de la reclamation a modifier : ");
            scanf("%d", &id);
            modifierReclamation(id,ajent_menu);
            break;
        case 4:
            printf("Entrez l'ID de la reclamation a supprimer : ");
            scanf("%d", &id);
            supprimerReclamation(id,ajent_menu);
            break;
        case 5:
            printf("Entrez l'ID de la reclamation a traiter : ");
            scanf("%d", &id);
            traiterReclamation(id,ajent_menu);
            break;
        case 6:
            printf("Entrez l'ID de la reclamation a rechercher : ");
            scanf("%d", &id);
            rechercherReclamation(id,ajent_menu);
            break;
        case 7:
            aff_rec_client(ajent_menu);
            return;
        case 8:
            printf("\nDeconexion en cours ");
            fordelay();
            main();
            break;
        default:
            printf("Option invalide. Veuillez reessayer.\n");
    }
    
    
    fordelay();
}

void menu_client(User *user_menu) {
    system("cls");
    int choix;
    printf("\t\tC'est le cote client\n");
    printf("\tWelcome %s\n",user_menu->username);
    printf("1. Ajouter une reclamation\n");
    printf("2. modifer ma reclamation\n");
    printf("3. Voir votre etat\n");
    printf("4. suprimer ma reclamation \n");
    printf("5. Deconexion\n");
    printf("\t\tEntrer votre choix : ");
    scanf("%d",&choix);
    switch (choix)
    {
    case 1:
        ajouterRec(user_menu);
        break;
    case 2:
        modifier_rec_client(user_menu);
        break;
    case 3:
        aff_rec_client(user_menu);
        break;
    case 4:
        supprimer_Rec_client(user_menu);
        break;
    case 5:
        printf("\nDeconexion en cours ");
        fordelay();
        main();
        break;
    
    default:
        break;
    }
    
    
    fordelay();
}
void ajouterRec(User *user_menu) {
    system("cls");
    int new_id = IndexRec+1;
    time_t t = time(NULL);//struct tm est une structure qui contient des informations détaillées sur la date et l’heure.
    struct tm tm = *localtime(&t);//Convertit un time_t en une structure tm représentant l’heure locale.
    
    printf("Nouvelle reclamation de l'ID : %d\n",new_id);
    printf("Entrer le motif (ne depasse pas (100) caracter ) :\n");
    scanf(" %[^\n]", rec[IndexRec].motif);
    printf("Entrer la description (ne depasse pas (255) caracter) :\n");
    scanf(" %[^\n]", rec[IndexRec].description);
    printf("Entrer une categorie :\n");
    scanf(" %[^\n]", rec[IndexRec].categorie);
    strcpy(rec[IndexRec].status,"en cours");
    rec[IndexRec].date.sec = tm.tm_sec;
    rec[IndexRec].date.min = tm.tm_min;
    rec[IndexRec].date.heur = tm.tm_hour;
    rec[IndexRec].date.jour = tm.tm_mday;
    rec[IndexRec].date.moi = tm.tm_mon + 1; // tm_mon est de 0 à 11
    rec[IndexRec].date.anne = tm.tm_year + 1900;// tm_year est le nombre d'années depuis 1900
    
    strcpy(rec[IndexRec].client,user_menu->username);
    printf("%s\n",rec[IndexRec].client);
    char temp_name[50];
    printf("Entrer votre nom Exact :");
    scanf("%s",temp_name);
    printf("%s ",temp_name);
    for (int i = 0; i <= Indexfree; i++)
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
    printf("%d",rec[IndexRec].id);
    IndexRec++;
    for (int i = 0; i < 2; i++)
    {
        printf("%d",user_menu->all_rec[i]);
    }
    
    fordelay();
    if (strcmp(user_menu->username,"admin")==0)
    {
        menu_admin();
        
    }
    if (user_menu->role == 2)
    {
        menu_ajent(user_menu);
    }
    
    

    menu_client(user_menu);
}
void modifier_rec_client(User *user_menu) {
    int id,choix;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t); 
    printf("Entrer l'ID de reclamation : ");
    scanf("%d",&id);
    
    
    for (int i = 0; i <= IndexRec; i++)
    {
        if (rec[i].id == id)
        {
            if (strcmp(rec[i].status,"resolue") == 0 ||strcmp(rec[i].status,"rejeter") == 0)
            {
                printf("Cette reclamation est deja traiter tu ne peux pas le modifier .");
                fordelay();
                menu_client(user_menu);
            }
            
            if (difftime(tm.tm_sec,rec[i].date.sec) < 1)
            {
                printf("Cette reclamation deppaser 24h tu ne peut pas modifier .");
                fordelay();
                menu_client(user_menu);
            }
            if (strcmp(rec[i].client,user_menu->username)==0)
            {
                printf("1. modifier le motif.\n2. modifier la description. \n3. modifier la categorie \n4. modifier all choix.\n\tEntrer votre choix : ");
            scanf("%d",&choix);
            switch (choix)
            {
            case 1:
                printf("Entrez le nouveau motif : ");
                scanf(" %s", rec[i].motif);
                break;
            case 2:
                printf("Entrez la nouvelle description : ");
                scanf(" %s", rec[i].description);
                break;
            case 3:
                printf("Entrez la nouvelle categorie : ");
                scanf(" %s", rec[i].categorie);
                break;
            case 4:
                printf("Entrez le nouveau motif : ");
                scanf(" %s", rec[i].motif);
                printf("Entrez la nouvelle description : ");
                scanf(" %s", rec[i].description);
                printf("Entrez la nouvelle categorie : ");
                scanf(" %s", rec[i].categorie);
                break;
            
            default:
                printf("Invalid choix ");
                fordelay();
                menu_client(user_menu);
                break;
            }
            printf("Reclamation modifiee avec succes.\n");
            fordelay();
            menu_client(user_menu);
            }else {
                printf("cette reclamtion est n'est pas votre reclamtion.\n");
                fordelay();
                menu_client(user_menu);
            }
            
        }else if(i != IndexRec) {
            continue;
        }
        else {
            printf("Invalid ID.");
            fordelay();
            menu_client(user_menu);
        }
        
    }
    
}

void aff_rec_client(User *user_menu) {
    int aucun_reclamation = 0;
    printf("%s\n",user_menu->username);

    for (int i = 0; i <= IndexRec; i++)
    {
        if (strcmp(user_menu->username,rec[i].client) == 0)
        {
            printf("ID: %d\n", rec[i].id);
            printf("Motif: %s\n", rec[i].motif);
            printf("Description: %s\n", rec[i].description);
            printf("Categorie: %s\n", rec[i].categorie);
            printf("Status: %s\n", rec[i].status);
            printf("Date: %d/%d/%d\n", rec[i].date.jour,rec[i].date.moi,rec[i].date.anne);
            printf("Client: %s\n\n", rec[i].client);
            aucun_reclamation++;
            
        }
        
    }
    if (aucun_reclamation == 0)
    {
        printf("Tu n'a pas une reclamation \n");
    }
    
    int choix;
    invalid_aff_rec_client:
    printf("Entrer 0 to return to menu : ");
    
    scanf("%d",&choix);
    if (choix == 0)
    {
        menu_client(user_menu);
    }else {
        printf("Invalid choix !!!!!! \n");
        goto invalid_aff_rec_client;
    }    
}

void supprimer_Rec_client(User *user_menu) {
    int choix2,id;
    printf("Entrer l'ID de reclamation : ");
    scanf("%d",&id);
    for (int i = 0; i <= IndexRec; i++) {
        if (rec[i].id == id && strcmp(rec[i].client,user_menu->username) == 0) {
            for (int j = i; j <= IndexRec - 1; j++) {
                rec[j] = rec[j + 1];
            }
            IndexRec--;
            printf("Reclamation supprimee avec succes.\n");
            Invalid_suppression_client_reclamation:
            printf("Entrer 0 pour retourner au menu : ");
            scanf("%d",&choix2);
            if (choix2 == 0)
            {
                menu_client(user_menu);
            }else {
                printf("Invalid choix");
                goto Invalid_suppression_client_reclamation;
            }
            return;
        }
    }
    printf("Reclamation non trouvee, ou vous n'avez le droit de suprimer cette reclamation\n");
    printf("Entrer 0 pour retourner au menu : ");
    scanf("%d",&choix2);
    if (choix2 == 0)
    {
        menu_client(user_menu);
    }else {
        printf("Invalid choix");
        goto Invalid_suppression_client_reclamation;
    }
}


void fordelay() {
    for (int i = 0; i < 3; i++) {
        printf(".");
        Sleep(1000);
    }
}

int main() {

    User user;
    int choice;
    int isRegistered = 0;
    int index_user;
    int *user_cote;
    srand(time(NULL));
    while (true) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                 user_cote = signIn(&user);
                if(user_cote[0] == 1) {
                    menu_admin();
                }else if(user_cote[0] == 2){
                    for (int i = 0; i <= Indexfree; i++)
                    {
                        if (i == user_cote[1])
                        {
                            menu_ajent(&users[i]);
                            break;
                        }
                        
                    }
                }else if (user_cote[0] == 3)
                {
                    for (int i = 0; i <= Indexfree; i++)
                    {
                        if (i == user_cote[1])
                        {
                            menu_client(&users[i]);
                            break;
                        }
                        
                    }
                    
                }else {
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

void server_down() {
    system("cls");
    for (int i = 5; i >= 0; i--)
    {
        displayMenu();
        printf("Server Down for %ds ",i);
        Sleep(1000);
    }
    anti_spam = 0; 
    main();
    
}

