#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>

typedef struct {
    char username[50];
    char password[50];
} User;

User users[100];
int Indexfree = 2;

 
void tri_by_name();
void fordelay();
void displayMenu();
void signUp();
void menu_client();
void menu_ajent();
void menu_admin();
int signIn(User *user);
bool isCorrect_MDP(char *mdp);




// Fonction pour afficher le menu
void displayMenu() {
    system("cls");
    printf("\t\t\tBienvenue !\n\n\n");
    printf("\t1. Sign In\n");
    printf("\t2. Sign Up\n\n");
    printf("\t3. Quitter\n");
    printf("\t\t\t\tChoisissez une option: ");
}

void signUp(User *user) {
    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin@2024");
    strcpy(users[1].username,"ajent");
    strcpy(users[1].password,"ajent@2024");
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

    printf("Inscription reussie !\n");
    Indexfree++; 
    tri_by_name();

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
    

    while (1) {
        printf("Connexion:\n");
        printf("Entrez votre nom d'utilisateur: ");
        scanf("%s", username);
        printf("Entrez votre mot de passe: ");
        scanf("%s", password);

        

        if (strcmp(users[0].username, username) == 0 && strcmp(users[0].password, password) == 0) {
            cote = 1;
            break;
        } else if (strcmp(users[1].username, username) == 0 && strcmp(users[1].password, password) == 0) {
            cote = 2;
            break;
        } else if (strcmp(user-> username, username) == 0 && strcmp(user-> password, password) == 0) {
            cote = 3;
            printf("Connexion reussie !\n");
            break;
        } else {
            printf("Nom d'utilisateur ou mot de passe incorrect.\n");
        }
    }
    return cote;
}

void menu_admin() {
    int choix;
    printf("C'est le cote admin\n");
    fordelay();
    system("cls");
    printf("\t\t\tWELCOME ADMIN !!!!\n");
    printf("Ajouter\n");
    scanf("%d", &choix);
}

void menu_ajent() {
    printf("C'est le cote agent\n");
    fordelay();
}

void menu_client() {
    printf("C'est le cote client\n");
    fordelay();
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
                printf("Au revoir !\n");
                for (int i = 0; i < Indexfree; i++)
                {
                    printf("name : %s\nmdp : %s\n",users[i].username,users[i].password);
                }
                
                return 0;
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
    printf("Contacts tries par nom.\n");
}
