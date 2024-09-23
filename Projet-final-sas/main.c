#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <conio.h>

#define MAX_UTLISATEURS 100
#define MAX_RECLAMATIONS 100



typedef struct                  // structure pour utilisateur
{
    char username[50];
    char password[50];
    char role[20];
    int estVerrouille;   // Indique si le compte est verrouillé (0 = non, 1 = oui)
} Utilisateur;

typedef struct                   // structure pour reclamation
{
    int id;
    char motif[50];
    char description[200];
    char categorie[50];
    char status[20];               // en cours, resolu, rejete
    char date[20];
    char client_nom[50];
    int priorite;                     // 1 = basse, 2 = moyenne, 3 = haute
    time_t date_ajout;
    char resolutionDate[20];
} Reclamation;

Utilisateur user[MAX_UTLISATEURS];                      // tableaux pour stocker utilisateur et les reclamations
Reclamation rec[MAX_RECLAMATIONS];

int usercount = 0;                      // varibale pour tracer le nombre d'utilisateurs et les reclamations
int reccount = 0;






  int contient_special_char(char *password)                  // ***fonction pour s'inscrire et verifier les criteres de mot de pass
{
    char specialchar[] = " ! @ # $ % ^ & * µ ( ) { } [ ] ?";

    for (int i = 0; i < strlen(password); i++)
        {
        for (int j = 0; j < strlen(specialchar); j++)
        {
            if (password[i] == specialchar[j])
            {
                return 1;
            }
        }
        }
    return 0;
}

int isPasswordOK(char *password, char *username)
{
    int hasUpper = 0, hasLower = 0, hasDigit = 0;

    if (strstr(password, username) != NULL)
        {                     // pour verifier si le mot de passe contient le nom d'utilisateur
        return 0;
        }

    for (int i = 0; i < strlen(password); i++) {       // pour verifier  les criteres de  mot de passe
        if (isupper(password[i])) hasUpper = 1;
        if (islower(password[i])) hasLower = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }

    return strlen(password) >= 8 && hasUpper && hasLower && hasDigit && contient_special_char(password);
}

void sign_Up()                      //***fonction pour s'inscrire
{
    Utilisateur newuser;
    printf("Entrez un nom d'utilisateur :\n");
    scanf("%s", newuser.username);

    while (1)
        {
        printf("Entrez un mot de passe :\n");
        scanf("%s", newuser.password);

        if (isPasswordOK(newuser.password, newuser.username))
            {
            break;
            } else
            {
            printf("Mot de passe invalide.\n Il doit contient au moins 8 caracteres:\n une majuscule, une minuscule, un chiffre et un caractere special.\n Et ne doit pas contenir le nom d'utilisateur.\n");
            }
    }

    printf("Choisissez un role (Administrateur, Agent, Client) :\n");
    scanf("%s", newuser.role);

    newuser.estVerrouille = 0;
    user[usercount++] = newuser;                         // ajouter l'utilisateur sur le tableau
    printf("Vous etes inscrie avec succes!\n\n");
}


   Utilisateur sign_In() {
    char username[50], password[50];
    int essai = 0;
    Utilisateur utilisateurVide = {"", "", "", 0};  // Un utilisateur vide à retourner en cas d'échec

    while (essai < 3) {
        printf("Nom d'utilisateur :\n");
        scanf("%s", username);
        printf("Mot de passe :\n");
        scanf("%s", password);

        for (int i = 0; i < usercount; i++) {
            if (strcmp(user[i].username, username) == 0 && strcmp(user[i].password, password) == 0) {
                printf("Vous etes connecter avec succes!\n");
                return user[i];  // Retourne l'utilisateur connecte
            }
        }

        essai++;
        printf("Identifiant ou mot de passe incorrect. %d tentatives restantes.\n", 3 - essai);
    }


    // Si echec apres plusieurs tentatives, retourner un utilisateur vide
    return utilisateurVide;
}


int isAdministrateur(Utilisateur user)
{
    return strcmp(user.role, "Administrateur") == 0;
}

int isAgent(Utilisateur user)
{
    return strcmp(user.role, "Agent") == 0;
}

int isClient(Utilisateur user)
{
    return strcmp(user.role, "Client") == 0;
}



   void add_reclamation(char *client_nom)                    // fonction pour ajouter une reclamation
   {
    Reclamation newrec;
    newrec.id = rand() % 10000;

    printf("Entrer le motif de la reclamation :\n");
    scanf("%s", newrec.motif);
    printf("Entrer une description :\n");
    scanf(" %[^\n]s", newrec.description);
    printf("Entrer la categorie de la reclamation :\n");
    scanf("%s", newrec.categorie);

    // Récupération automatique de la date actuelle
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    strftime(newrec.date, sizeof(newrec.date), "%d/%m/%Y", local);

    newrec.priorite = attribuer_priorite(newrec.description);
    strcpy(newrec.status, "en cours");
    strcpy(newrec.client_nom, client_nom);

    newrec.date_ajout = now;

    rec[reccount++] = newrec;
    printf("Reclamation ajoutee avec succes ! Status : en cours . ID : %d \n", newrec.id);
                            // Enregistre l'heure actuelle
    newrec.date_ajout = now;


}






void modifier_reclamation(Utilisateur currentuser)            // ***fonction pour modifier la reclamation
{
    int id, found = 0;
    printf("Entrez l'ID de la reclamation a modifier :\n");
    scanf("%d", &id);

    for (int i = 0; i < reccount; i++) {
        if (rec[i].id == id) {
            found = 1;

            // Verifier si l'utilisateur a les droits pour modifier la reclamation
            if (isAdministrateur(currentuser) || isAgent(currentuser) ||
                (isClient(currentuser) && strcmp(rec[i].client_nom, currentuser.username) == 0 && strcmp(rec[i].status, "en cours") == 0))
            {
                // Verifier si la relamation a ete enregistrer il y a moins de 24 heures
                time_t now = time(NULL);
                double diff_in_seconds = difftime(now, rec[i].date_ajout);

                if (diff_in_seconds <= 86400) {  // 86400 secondes = 24 heures
                    printf("Modification de la reclamation (ID : %d).\n", id);
                    printf("Entrez le nouveau motif :\n");
                    scanf("%s", rec[i].motif);
                    printf("Entrez la nouvelle description :\n");
                    scanf(" %[^\n]s", rec[i].description);
                    printf("Entrez la nouvelle categorie :\n");
                    scanf("%s", rec[i].categorie);

                    printf("Reclamation modifiee avec succes!\n");
                }
                else
                    {
                    printf("La reclamation ne peut pas etre modifiee, car que 24 heures sont passees.\n");
                    }
            }
            else
                {
                printf("Vous n'avez pas les droits pour modifier cette reclamation.\n");
                }
            break;
        }
    }

    if (!found)
    {
        printf("Reclamation avec ID %d non trouvee.\n", id);
    }
}




 void supprimer_reclamation(Utilisateur currentuser)                //*** fonction pour supprimer une reclamation
{
    int id, found = 0;
    printf("Entrer ID de la reclamation a supprimer : ");
    scanf("%d", &id);

    for (int i = 0; i < reccount; i++) {
        if (rec[i].id == id) {
            found = 1;

            // Verifier si l'utilisateur a les droits pour supprimer la réclamation
            if (isAdministrateur(currentuser) || isAgent(currentuser) ||
                (isClient(currentuser) && strcmp(rec[i].client_nom, currentuser.username) == 0 && strcmp(rec[i].status, "en cours") == 0))
            {
                // Verifier si la réclamation a ete soumise il y a moins de 24 heures
                time_t now = time(NULL);
                double diff_in_seconds = difftime(now, rec[i].date_ajout);

                if (diff_in_seconds <= 86400) {  // 86400 secondes = 24 heures
                    // Supprimer la réclamation (decaler le tableau)
                    for (int j = i; j < reccount - 1; j++) {
                        rec[j] = rec[j + 1];
                    }
                    reccount--;
                    printf("Reclamation supprimee avec succes !\n");
                }
                 else
                    {
                    printf("La reclamation ne peut pas etre supprimee, car plus de 24 heures se sont ecoulees.\n");
                    }
            }
            else
                {
                printf("Vous n'avez pas les droits pour supprimer cette reclamation.\n");
                }
            break;
        }
    }

    if (!found)
        {
        printf("Reclamation avec ID %d non trouvee.\n", id);
        }
}

 void liste_reclamation(Utilisateur currentuser)                  //*** fonction pour afficher la liste des reclamations
 {
                                                                // Vérifier si l'utilsateur est administrateur ou an agent
    if (!isAdministrateur(currentuser) && !isAgent(currentuser))
    {
        printf("Acces refuse. Vous n'avez pas les droits pour afficher les reclamations.\n");
        return;
    }

                                                                 // Verifier s'il y a des reclamations a afficher
    if (reccount == 0)

        {
        printf("Aucune reclamation a afficher.\n");
        return;
        }

    printf("Liste des reclamations:\n");
    printf("---------------------------------------------------------------------------\n");
    printf("%-5s %-15s %-20s %-10s %-10s %-15s\n", "ID", "Motif", "Description", "Client", "Statut", "Date");     // (%-5) sont des specificateur de formatage pour bien aligner les colones

                                                                          // Parcourir et afficher toutes les reclamations
    for (int i = 0; i < reccount; i++)

        {
        printf("%-5d %-15s %-20s %-10s %-10s %-15s\n",rec[i].id, rec[i].motif, rec[i].description, rec[i].client_nom, rec[i].status, rec[i].date);
        }
    printf("---------------------------------------------------------------------------\n");
}




void traiter_reclamation (Utilisateur currentuser)                  // ***fonction pour traiter une reclamation
{
    int id, found = 0;
    char newstatus[20], status[20];

    printf("Entrer ID de la reclamation a traiter:\n");
    scanf("%d", &id);

    for ( int i = 0; i < reccount; i++)
    {
        if ( rec[i].id == id)
        {
            found = 1;

            if ( isAdministrateur(currentuser) || isAgent(currentuser))
            {
                printf("Le status actuelle : %s \n", rec[i], status);
                printf("Entrer le nouveau status ( en cours, resolu, rejecte ):\n");
                scanf("%s", newstatus);
                strcpy(rec[i].status, newstatus);


                printf("Reclamation mise a jour avec succes.\n");
            }
            else
            {
                printf("Vous n'avez pas le droit pour supprimer la reclamation.\n");
            }
            break;
        }
    }
    if (!found)
        {
            printf("Id : %d Non trouvee.\n",id);
        }

}

void rechercher_reclamation (Utilisateur currentuser)                       // ***fonction pour rechercher une reclamation
{
    int choix, id, found = 0;
    char client_nom[50], status[50], categorie[50];

    if (!isAdministrateur(currentuser) && !isAgent(currentuser))
        {
        printf("Vous n'avez pas le droit de rechercher une reclamation !\n");
        return;
        }

    printf("---------------Rechercher la reclamation par :---------------\n");
    printf("1- ID de la reclamation :\n");
    printf("2- Le nom du client :\n");
    printf("3- La categorie de la reclamation :\n\n");
    printf("Entrez votre choix :\n");
    scanf("%d", &choix);

    switch (choix)
    {
    case 1:
        printf("Entrer ID de la reclamation :\n");
        scanf("%d", &id);
        for (int i = 0; i < reccount; i++) {
            if (rec[i].id == id) {
                found = 1;
                printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",
                       rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                break;
            }
        }
        break;
    case 2:
        printf("Entrer le nom du client :\n");
        scanf(" %[^\n]s", client_nom);
        for (int i = 0; i < reccount; i++) {
            if (strcmp(rec[i].client_nom, client_nom) == 0) {
                found = 1;
                printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",
                       rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
            }
        }
        break;
    case 3:
        printf("Entrer la categorie de la reclamation :\n");
        scanf("%s", categorie);
        for (int i = 0; i < reccount; i++) {
            if (strcmp(rec[i].categorie, categorie) == 0) {
                found = 1;
                printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",
                       rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
            }
        }
        break;
    default:
        printf("Choix invalide.\n");
        return;
    }

    if (!found) {
        printf("Reclamation non trouvee.\n");
    }
}

void attribuer_priorite( char * description)                          // ***fonction pour detecter les mots de la priorite
{
 if ( strstr(description, "urgent") || strstr(description, "crucial") || strstr(description, "vital")
     || strstr(description, "primordial") || strstr(description, "essentiel"))
 {
     return 3;                      // pour haute priorite
 }
 else if ( strstr(description, "important") || strstr(description, "Significatif") || strstr(description, "Substantiel")
          || strstr(description, "Conséquent") || strstr(description, "Nécessaire"))
 {
     return 2;                      // pour moyenne priorite
 }
 else
 {
     return 1;                       // pour baisse priorite
 }
}

void trier_reclamation_priorite()                      // ***fonction pour trier les reclamations par priorite
{
    Reclamation valeur;
    for (int i = 0; i < reccount - 1; i++)
        {
        for (int j = i + 1; j < reccount; j++)
            {
            if (rec[i].priorite < rec[j].priorite)
                {
                    valeur = rec[i];
                    rec[i] = rec[j];
                    rec[j] = valeur;
                }

            }
        }
}

void afficher_reclamation_priorite(Utilisateur currentuser)                   //*** fonction pour afficher la liste des rec triee par ordre priorite
{
    if (!isAdministrateur(currentuser))
    {
        printf("Acces refuse. Vous n'avez pas les droits d'acceder au reclamations triees par priorite");
        return;
    }

    trier_reclamation_priorite();

    printf("Reclamations triees par priorite:\n");
    printf("%-5s %-15s %-20s %-10s %-10s %-15s\n", "ID", "Motif", "Description", "Client", "Statut", "Date");

    for (int i = 0; i < reccount; i++)
    {
        printf("%-5d %-15s %-20s %-10s %-10s %-15s\n",
               rec[i].id, rec[i].motif, rec[i].description, rec[i].client_nom, rec[i].status, rec[i].date);
    }
}


