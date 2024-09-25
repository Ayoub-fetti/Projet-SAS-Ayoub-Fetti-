#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_UTLISATEURS 100
#define MAX_RECLAMATIONS 100



typedef struct                  // structure pour utilisateur
{
    char username[50];
    char password[50];
    char role[20];
    int estVerrouille;
} Utilisateur;

typedef struct                   // structure pour reclamation
{
    int id;
    char motif[50];
    char description[200];
    char categorie[50];
    char status[20];
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
        {                                          // pour verifier si le mot de passe contient le nom d'utilisateur
        return 0;
        }

    for (int i = 0; i < strlen(password); i++)
        {       // pour verifier  les criteres de  mot de passe
            if (isupper(password[i])) hasUpper = 1;
            if (islower(password[i])) hasLower = 1;
            if (isdigit(password[i])) hasDigit = 1;
        }

    return strlen(password) >= 8 && hasUpper && hasLower && hasDigit && contient_special_char(password);
}

void sign_Up()                      //***fonction pour s'inscrire
{
    Utilisateur newuser;
    printf("Entrer un nom d'utilisateur :\n");
    scanf("%s", newuser.username);

    while (1)
        {
        printf("Entrer un mot de passe :\n");
        scanf("%s", newuser.password);

        if (isPasswordOK(newuser.password, newuser.username))
            {
            break;
            } else
            {
            printf("Mot de passe invalide.\n Il doit contient au moins 8 caracteres:\n une majuscule, une minuscule, un chiffre et un caractere special.\n Et ne doit pas contenir le nom d'utilisateur.\n");
            }
    }

    printf("Choisissez un role (administrateur, agent, client) :\n");
    scanf("%s", newuser.role);

    newuser.estVerrouille = 0;
    user[usercount++] = newuser;                         // ajouter l'utilisateur sur le tableau
    printf("Vous etes inscrie avec succes!\n\n");
}


   Utilisateur sign_In()                                //*** fonction pour se connecter
   {
    char username[50], password[50];
    int essai = 0;
    Utilisateur utilisateurVide = {"", "", "", 0};  // un utilisateur vide a retourner en cas d'echec

    while (essai < 3) {
        printf("Nom d'utilisateur :\n");
        scanf("%s", username);
        printf("Mot de passe :\n");
        scanf("%s", password);

        for (int i = 0; i < usercount; i++) {
            if (strcmp(user[i].username, username) == 0 && strcmp(user[i].password, password) == 0) {
                printf("Vous etes connecter avec succes!\n");
                return user[i];                    // retourne l'utilisateur connecte
            }
        }

        essai++;
        printf("Identifiant ou mot de passe incorrect. %d tentatives restantes.\n", 3 - essai);
    }


    return utilisateurVide;                        // Si echec apres plusieurs tentatives, retourner un utilisateur vide
}


int isadministrateur(Utilisateur user)
{
    return strcmp(user.role, "administrateur") == 0;
}

int isagent(Utilisateur user)
{
    return strcmp(user.role, "agent") == 0;
}

int isclient(Utilisateur user)
{
    return strcmp(user.role, "client") == 0;
}



   void add_reclamation(char *client_nom)                    // ***fonction pour ajouter une reclamation
   {
    Reclamation newrec;
    newrec.id = rand() % 10000;

    printf("Entrer le motif de la reclamation :\n");
    scanf("%s", newrec.motif);
    printf("Entrer une description :\n");
    scanf(" %[^\n]s", newrec.description);
    printf("Entrer la categorie de la reclamation  :\n");
    scanf("%s", newrec.categorie);

                                                         // recuperation automatique de la date actuelle
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    strftime(newrec.date, sizeof(newrec.date), "%d/%m/%Y", local);

    newrec.priorite = attribuer_priorite(newrec.description);
    strcpy(newrec.status, "en cours");
    strcpy(newrec.client_nom, client_nom);

    newrec.date_ajout = now;   // enregistre l'heure actuelle

    rec[reccount++] = newrec;
    printf("Reclamation ajoutee avec succes ! Status : en cours . ID : %d \n", newrec.id);





}






void modifier_reclamation(Utilisateur currentuser)               //*** fonction pour modifier la reclamation
{
    int id, found = 0;
    printf("Entrer ID de la reclamation a modifier :\n");
    scanf("%d", &id);


    for (int i = 0; i < reccount; i++)
        {
        if (rec[i].id == id)
        {
            found = 1;

                                                                // verifier si l'utilisateur a les droits pour modifier la reclamation
            if (isadministrateur(currentuser) || isagent(currentuser) ||
                (isclient(currentuser) && strcmp(rec[i].client_nom, currentuser.username) == 0 && strcmp(rec[i].status, "en cours") == 0))
            {
                if (isclient(currentuser))
                    {
                                                     // Verifier si la reclamation a ete enregistree il y a moins de 24 heures (seulement pour les clients)
                    time_t now = time(NULL);
                    double diff_in_seconds = difftime(now, rec[i].date_ajout);

                    if (diff_in_seconds > 5) // 86400 secondes = 24 heures
                        {
                        printf("La reclamation ne peut pas etre modifiee, car vous aveez passer 24 heures apres la date de soumission.\n");
                        return;
                        }
                    }

                printf("Modification de la reclamation (ID : %d).\n", id);
                printf("Entrer le nouveau motif :\n");
                scanf("%s", rec[i].motif);
                printf("Entrer la nouvelle description :\n");
                scanf(" %[^\n]s", rec[i].description);
                printf("Entrer la nouvelle categorie :\n");
                scanf("%s", rec[i].categorie);

                printf("Reclamation modifiee avec succes!\n");
            }
            else
            {
                printf("Vous n'avez pas les droits pour modifier cette reclamation.\n");
            }
            return;
        }
    }

    if (!found)
    {
        printf("Reclamation avec ID %d non trouvee.\n", id);
    }
}




 void supprimer_reclamation(Utilisateur currentuser)         //*** fonction pour supprimer une reclamation
 {
    int id, found = 0;
    printf("Entrer ID de la reclamation a supprimer :\n");
    scanf("%d", &id);

    for (int i = 0; i < reccount; i++) {
        if (rec[i].id == id) {
            found = 1;

                                                          // verifier si utilisateur est un administrateur ou un agent
            if (isadministrateur(currentuser) || isagent(currentuser))
                {
                                                                        // administrateur ou agent peut supprimer sans condition
                for (int j = i; j < reccount - 1; j++) {
                    rec[j] = rec[j + 1];
                }
                reccount--;
                printf("Reclamation supprimee avec succes !\n");
                return;
            }
                                                                                // Si utilisateur est un client
            else if
            (isclient(currentuser) && strcmp(rec[i].client_nom, currentuser.username) == 0)
            {
                // Veriifier si la reclamation a moins de 24 heures
                time_t now = time(NULL);
                double diff_in_seconds = difftime(now, rec[i].date_ajout);

                if (diff_in_seconds <= 20)
                    {
                                                               // supprimer  la reclamation
                    for (int j = i; j < reccount - 1; j++)
                    {
                        rec[j] = rec[j + 1];
                    }
                    reccount--;

                    printf("Reclamation supprimee avec succes !\n");
                }
                else
                    {
                        printf("La reclamation ne peut pas etre supprimer, car vous aveez passer 24 heures apres la date de soumission.\n");
                    }
                return;
            }
            else
                {
                printf("Vous n'avez pas les droits pour supprimer cette reclamation.\n");
                return;
                }
        }
    }

    if (!found)
        {
        printf("Reclamation avec ID %d non trouvee.\n", id);
        }
}

 void liste_reclamation(Utilisateur currentuser)                  //*** fonction pour afficher la liste des reclamations
 {
                                                                // Verifier si utilsateur est administrateur ou an agent
    if (!isadministrateur(currentuser) && !isagent(currentuser))
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




void traiter_reclamation(Utilisateur currentuser)                //***fonction pour traiter une reclamation
{
    int id, found = 0;
    char newstatus[20];

    printf("Entrer ID de la reclamation a traiter :\n");
    scanf("%d", &id);

    for (int i = 0; i < reccount; i++)
        {
        if (rec[i].id == id)
        {
            found = 1;

            if (isadministrateur(currentuser) || isagent(currentuser))
                {
                printf("Statut actuel : %s\n", rec[i].status);
                printf("Entrer le nouveau statut (en cours, resolu, rejecte) :\n");
                scanf("%s", newstatus);
                strcpy(rec[i].status, newstatus);

                                                        // Si la reclamation est resolue, enregistrer la date de resolution
                if (strcmp(newstatus, "resolu") == 0)
                    {
                    time_t now = time(NULL);
                    struct tm *local = localtime(&now);
                    strftime(rec[i].resolutionDate, sizeof(rec[i].resolutionDate), "%d/%m/%Y", local);
                    }

                printf("Reclamation traitee avec succes.\n");
            }
            else
                {
                printf("Vous n'avez pas le droit de traiter cette reclamation.\n");
                }
            break;
        }
    }

    if (!found) {
        printf("Reclamation avec ID %d non trouvee.\n", id);
    }
}

void rechercher_reclamation (Utilisateur currentuser)                       // ***fonction pour rechercher une reclamation
{
    int choix, id, found = 0;
    char client_nom[50], status[50], categorie[50],date[50];

    if (!isadministrateur(currentuser) && !isagent(currentuser))
        {
        printf("Vous n'avez pas le droit de rechercher une reclamation !\n");
        return;
        }

    printf("---------------Rechercher la reclamation par :---------------\n");
    printf("1- ID de la reclamation :\n");
    printf("2- Le nom du client :\n");
    printf("3- La categorie de la reclamation :\n");
    printf("4- La date de la reclamation :\n");
    printf("Entrez votre choix :\n");
    scanf("%d", &choix);

    switch (choix)
    {
    case 1:
        printf("Entrer ID de la reclamation :\n");
        scanf("%d", &id);
        for (int i = 0; i < reccount; i++)
            {
            if (rec[i].id == id)
                {
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
        for (int i = 0; i < reccount; i++)
            {
            if (strcmp(rec[i].client_nom, client_nom) == 0)
                {
                found = 1;
                printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",
                       rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                }
            }
        break;
    case 3:
        printf("Entrer la categorie de la reclamation :\n");
        scanf("%s", categorie);
        for (int i = 0; i < reccount; i++)
            {
            if (strcmp(rec[i].categorie, categorie) == 0)
                {
                found = 1;
                printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",
                       rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                }
            }
        break;
    case 4:
        printf("Entrer la date la reclamation :\n");
        scanf("%s", date);
        for (int i=0; i < reccount; i++ )
        {
            if (strcmp(rec[i].date,date)==0)
            {
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

int attribuer_priorite(const char *description)               //*** fonction pour attribuer la priorite pour les reclamations
{
    const char *mots_haute_priorite[] = {"urgent", "crucial", "vital", "primordial", "essentiel"};
    const char *mots_moyenne_priorite[] = {"important", "significatif", "substantiel", "consequent", "necessaire"};

    for (int i = 0; i < sizeof(mots_haute_priorite) / sizeof(mots_haute_priorite[0]); i++)
        {
        if (strstr(description, mots_haute_priorite[i]) != NULL)
        {
            return 3; // haute
        }
    }

    for (int i = 0; i < sizeof(mots_moyenne_priorite) / sizeof(mots_moyenne_priorite[0]); i++)
        {
        if (strstr(description, mots_moyenne_priorite[i]) != NULL)
        {
            return 2; // moyenne
        }
    }

    return 1; // baisse
}
void trier_reclamation_priorite()               //***fonction pour trier les reclamations
{
    for (int i = 0; i < reccount - 1; i++)
        {
        for (int j = 0; j < reccount - i - 1; j++)
        {
            if (rec[j].priorite < rec[j + 1].priorite)
            {
                                               // switcher les reclamations
                Reclamation temp = rec[j];
                rec[j] = rec[j + 1];
                rec[j + 1] = temp;
            }
        }
    }
}



void afficher_reclamation_priorite(Utilisateur currentuser)   //*** fonction pour afficher les reclamations triee
{
    if (!isadministrateur(currentuser))
        {
        printf("Acces refuse. Vous n'avez pas les droits pour acceder aux reclamations triees par priorite.\n");
        return;
        }


    trier_reclamation_priorite();

    printf("Reclamations triees par priorite :\n");
    printf("%-5s %-15s %-20s %-10s %-10s %-15s %-10s\n", "ID", "Motif", "Description", "Client", "Statut", "Date", "Priorite");

    for (int i = 0; i < reccount; i++)
        {
        printf("%-5d %-15s %-20.20s %-10s %-10s %-15s %-10d\n",
               rec[i].id, rec[i].motif, rec[i].description, rec[i].client_nom, rec[i].status, rec[i].date, rec[i].priorite);
        }
}
void gerer_roles_utilisateur(Utilisateur currentuser)          //*** fonction pour l'administrateur peut changer le role de l'utilisateur
{
    if (!isadministrateur(currentuser))
        {
        printf("Acces refuse. Seuls les administrateurs peuvent gerer les reles.\n");
        return;
        }

    char username[50];
    char newrole[20];
    int found = 0;

    printf("Entrer le nom d'utilisateur dont vous voulez changer le role :\n");
    scanf("%s", username);

    for (int i = 0; i < usercount; i++)
        {
        if (strcmp(user[i].username, username) == 0)
        {
            found = 1;
            printf("Role actuel de %s : %s\n", username, user[i].role);
            printf("Entrer le nouveau role (administrateur, agent, client) :\n");
            scanf("%s", newrole);

            if (strcmp(newrole, "administrateur") == 0 || strcmp(newrole, "agent") == 0 || strcmp(newrole, "client") == 0)
                {
                strcpy(user[i].role, newrole);
                printf("Le role de %s a ete change en %s.\n", username, newrole);
                }
                else
                    {
                    printf("Role invalide. Les roles valides sont : administrateur, agent, client.\n");
                    }
            break;
        }
    }

    if (!found)
        {
        printf("Utilisateur non trouve.\n");
        }
}

void rapport_journalier(Utilisateur currentuser)     //***fonction pour rapport journalier
{

    FILE *report = fopen("rapport_journalier.txt", "w");
    if (report == NULL)
    {
        printf("Erreur lors de la création du rapport.\n");
        return;
    }

                                                      // Obtenir la date actuelle sous forme de chaine
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    char todayStr[11];
    strftime(todayStr, sizeof(todayStr), "%d/%m/%Y", today);  // format de la date actuelle en char

    fprintf(report, "Rapport journalier - %s\n\n", todayStr);

    int newrec = 0, resolvedrec = 0;
    fprintf(report, "Nombre total de reclamations : %d\n\n", reccount);
    fprintf(report, "Nouvelles reclamations soumises aujourd'hui :\n");

    for (int i = 0; i < reccount; i++) {
                                                                 // Comparer directement les char reprrsentant les dates
        if (strcmp(rec[i].date, todayStr) == 0)
            {
            fprintf(report, "ID : %d, Motif : %s, Client : %s, Statut : %s\n",
                    rec[i].id, rec[i].motif, rec[i].client_nom, rec[i].status);
            newrec++;
            }

                                                                // Verifier si la reclamation a ete resolue aujourd'hui
        if (strcmp(rec[i].status, "resolu") == 0 && strcmp(rec[i].resolutionDate, todayStr) == 0)
            {
            fprintf(report, "Reclamation resolue ID : %d, Motif : %s, Client : %s\n",
                    rec[i].id, rec[i].motif, rec[i].client_nom);
            resolvedrec++;
            }
    }

    if (newrec == 0)
        {
        fprintf(report, "Aucune nouvelle reclamation soumise aujourd'hui.\n");
        }

    fprintf(report, "\nReclamations resolues aujourd'hui :\n");

    if (resolvedrec == 0)
        {
        fprintf(report, "Aucune reclamation resolue aujourd'hui.\n");
        }

                                                             //les statistiques de resolution
    fprintf(report, "\nStatistiques :\n");
    fprintf(report, "Nombre de nouvelles reclamations aujourd'hui : %d\n", newrec);
    fprintf(report, "Nombre de reclamations résolues aujourd'hui : %d\n", resolvedrec);

                                                           // calculer le taux de resolution
    if (newrec > 0)
        {
        float tauxResolution = ((float)resolvedrec / newrec) * 100;
        fprintf(report, "Taux de resolution : %.2f%%\n", tauxResolution);
        }
    else
        {
        fprintf(report, "Taux de resolution : Non applicable (aucune reclamation soumise aujourd'hui).\n");
        }

    fclose(report);
    printf("Rapport journalier genere avec succes dans 'rapport_journalier.txt'.\n");
}




void menu_principal(Utilisateur currentuser)
{
    int choix;

    while(1)
        {
        printf("\n------------- Menu Principal -------------\n");

                                                                          // Verifier du role de l'utilisateur connecte
        if (strcmp(currentuser.role, "administrateur") == 0)
            {
            printf("1- Afficher la liste des reclamations\n");
            printf("2- Modifier une reclamation\n");
            printf("3- Supprimer une reclamation\n");
            printf("4- Traiter une reclamation\n");
            printf("5- Rechercher une reclamation\n");
            printf("6- Afficher les reclamations triees par priorite\n");
            printf("7- Gerer les roles des utilisateur\n");
            printf("8- Afficher le rapport journalier\n");
            printf("9- Deconnexion\n");
            }
            else if (strcmp(currentuser.role, "agent") == 0)
            {

            printf("1- Afficher la liste des reclamations\n");
            printf("2- Modifier une reclamation\n");
            printf("3- Supprimer une reclamation\n");
            printf("4- Traiter une reclamation\n");
            printf("5- Rechercher une reclamation\n");
            printf("6- Deconnexion\n");
            }
            else if (strcmp(currentuser.role, "client") == 0)
            {
            printf("1- Ajouter une reclamation\n");
            printf("2- Modifier une reclamation\n");
            printf("3- Supprimer une reclamation\n");
            printf("4- Deconnexion\n");
            }

        printf("Entrez votre choix :\n");
        scanf("%d", &choix);

                                                       // Gerer le choix de l'utilisateur
        if (strcmp(currentuser.role, "administrateur") == 0)
            {
            switch(choix)
            {
                case 1:
                    liste_reclamation(currentuser);
                    break;
                case 2:
                    modifier_reclamation( currentuser);
                    break;
                case 3:
                    supprimer_reclamation(currentuser);
                    break;
                case 4:
                    traiter_reclamation(currentuser);
                    break;
                case 5:
                    rechercher_reclamation(currentuser);
                    break;
                case 6:
                    afficher_reclamation_priorite(currentuser);
                    break;
                case 7:
                    gerer_roles_utilisateur(currentuser);
                    break;
                case 8:
                     rapport_journalier(currentuser);
                     break;
                case 9:
                    printf("Deconnexion...\n");
                    system("cls");
                    return;
                default:
                    printf("Choix invalide. Veuillez reessayer.\n");
            }
        }
        else if (strcmp(currentuser.role, "agent") == 0)
            {
            switch(choix) {

                case 1:
                    liste_reclamation(currentuser);
                    break;
                case 2:
                    modifier_reclamation(currentuser);
                    break;
                case 3:
                    supprimer_reclamation(currentuser);
                    break;
                case 4:
                    traiter_reclamation(currentuser);
                    break;
                case 5:
                    rechercher_reclamation(currentuser);
                    break;
                case 6:
                    printf("Deconnexion...\n");
                    system("cls");
                    return;
                default:
                    printf("Choix invalide. Veuillez reessayer.\n");
            }
        }
        else if (strcmp(currentuser.role, "client") == 0)
            {
            switch(choix)
            {
                case 1:
                    add_reclamation(currentuser.username);
                    break;
                case 2:
                    modifier_reclamation(currentuser);
                    break;
                case 3:
                    supprimer_reclamation(currentuser);
                    break;
                case 4:
                    printf("Deconnexion...\n");
                    system("cls");
                    return;
                default:
                    printf("Choix invalide. Veuiller reessayer.\n");
            }
        }
    }
}

int main()
{
    Utilisateur currentuser;
    int connecte = 0;

    while (1) {
        printf("\n--- Bienvenue dans l'Application de Gestion des Reclamations ---\n\n");
        printf("1- S'inscrire\n");
        printf("2- Se connecter\n");
        printf("3- Quitter le programme\n");
        printf("Entrez votre choix :\n");

        int choix;
        scanf("%d", &choix);

        switch (choix)
        {
            case 1:
                sign_Up();
                break;
            case 2:
                currentuser = sign_In();
                system("cls");
                if (strcmp(currentuser.username, "") != 0) {  // Si l'utilisateur n'est pas vide la est connexion reussie
                    connecte = 1;
                    menu_principal(currentuser);
                } else {
                    printf("Echec de la connexion.\n");
                }
                break;
            case 3:
                printf("Au revoir !\n");
                return 0;

            default:
                printf("Choix invalide. Veuiller reessayer.\n");
        }

        if (connecte)
            {
            connecte = 0;                                  // reinitialisation
            }
    }

    return 0;
}
