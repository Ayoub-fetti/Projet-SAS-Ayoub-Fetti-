#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_USERS  100
#define MAX_RECLAMATIONS 100


typedef struct                  // une structure pour Utilisateurs
{
char  username[50];
char  password[50];
char   role[10];
int estVerrouille;       // bach n3ref wach compte wach verrouiller ou non ( 1 = oui et 0 = non )

}Utilisateur;



typedef struct             // une structure pour reclamations
{
    int id;
    int priorite; // 1 basse, 2 moyenne, 3 haute
    char motif[50];
    char description[500];
    char categorie[50];
    char status[50];     // en cours, resolu, rejecte
    char date[50];       // dd/mm/yy
    char client_nom[50];

} Reclamation;


Utilisateur user[MAX_USERS];
Reclamation rec[MAX_RECLAMATIONS];             // deux tableaux pour stocker utilisateurs et reclamations

int usercount = 0;
int reccount = 0;                              // deux variable pour tracer le nombre d'utilisateur et reclamations





int contient_special_char( char * password)    // ***fonction pour inscrire et verifier les critreres de password***
{
   char specialchar[] = "! @ # $ % ^ & * ( ) - _ = + { } [ ] | \ : ; ' , . < > / ? ~";
   for ( int i=0 ; i < strlen(password); i++)
   {
      for ( int j=0 ; j < strlen(specialchar); j++)
      {
          if (password[i] = specialchar[j])
            return 1;
   }
}
return 0;
}



int passwordOK(char * password, char * username)
{
    int hasUpper = 0, hasLower = 0, hasDigit = 0;

if (strstr(password, username) != NULL)              // verifier si le password contient le nom de l'utilisateur
{
    return 0;
}

for ( int i = 0; i < strlen(password); i++)
{
    if (isupper(password[i])) hasUpper = 1;
    if (islower(password[i])) hasLower = 1;
    if (isdigit(password[i])) hasDigit = 1;
}

return  strlen(password) >= 8 && hasUpper && hasLower && hasDigit && contientspecialchar(password);
}




void sign_Up()                       // ***fonction pour creer un compte***
{

    Utilisateur newuser;
    printf("Entrer un nom d'utilisateur :\n");
    scanf(" %[^\n]s", newuser.username);

    while(1)
    {
        printf("Entrer un mot de passe :\n");
        scanf("%s", newuser.password);

        if (ispasswordOK(newuser.password,newuser.username))
        {
            break;
        }
        else
        {
           printf("Mot de passe invalide , le mot de passe doit contient au minimum 8 caractere:\n Une Majuscule,\n Une miniscule,\n Un chiffre,\n Un caractere special,\n Ne doit pas contient le nom d'utilisateur.\n");
        }
    }
    printf("Choisir un role (Administrateur, Agent, Client) :\n");
    scanf("%s", newuser.role);

    newuser.estVerrouille = 0;
    user[usercount++] = newuser;                          // ajouter l'utilisateur dans le tableau user[]

    printf("Votre inscription a ete completee avec succes");
}




int sign_In()                            // ***fonction pour connecter un compte***
{
    char username[50];
    char password[50];
    int essai = 0;


    while (essai < 3)
    {
        printf("Entrer le nom d'utilisateur:\n");
        scanf(" %[^\n]s", username);
        printf("Entrer le mot de passe:\n");
        scanf(" %[^\n]s", password);

        for ( int i=0; i< usercount; i++ )
        {
            if (strcmp(user[i].username,username) == 0 && strcasecmp(password[i].password,password) == 0)
            {
                if (user[i].estVerrouille == 1)
                {
                    printf("Le compte est verouille. Reesseayer plus tard !\n");
                    return 0;
                }

                printf("Vous etes connecter avec succees\n");

               return 1;
            }
        }
        essai++;
        printf("nom d'utilisateur ou mot de passe incorrect. Il vous reste %d tentatives.\n", 3 - essai);
    }
    for ( int i=0; i< usercount; i++ )
        {
            if (strcmp(user[i].username,username) == 0 && strcasecmp(password[i].password,password) == 0)
            {
                user[i].estVerrouille == 1;
                printf("Le compte est verouiller apres 3 tentatives echouees. Reessayer dans 15 minites.");
                break;
            }
        }
return 0;
}

 int isAdministrateur(Utilisateur user)                       // pour verifier le role de l'utilisateur
 {
     return strcmp(user.role, "admin") == 0;
 }
 int isAgent(Utilisateur user)
 {
     return strcmp(user.role, "agent") == 0;
 }
 int isClient(Utilisateur user)
 {
     return strcmp(user.role, "client") === 0;
 }




 void add_reclamation(char * client_nom)                      // ***fonction pour ajouter une reclamation***
 {
     Reclamation newrec;
     newrec.id = rand() % 10000;
     printf("Entrer le Motif de la reclamation:\n");
     scanf(" %[^\n]s", newrec.motif);

     printf("Entrer la description de la reclamation:\n");
     scanf(" %[^\n]s", newrec.description);

     printf("Entrer la categorie de la reclamation:\n");
     scanf(" %[^\n]", newrec.categorie);

    newrec.priorite = attribuer_priorite(newrec.description);
    strcpy(newrec.status, "en cours");              // les status est "en cours" par default
    strcpy(newrec.client_nom, client_name);

    time_t t = time(NULL);
    struct tm tm = *location(&t);
    sprintf(newrec.date,"%02d/%02d/%04d", tm.tm_mday, tm.tm_mon +1, tm.tm_year + 1900);

    rec[reccount++] = newrec;
    printf("Reclamation ajoutee avec succes. Priorite : %d\n", newrec.priorite);

 }


int client_modifier_supprimer(Reclamation rec, user currentUser)             // ***fonction pour le client peut modifier ou supprimer la reclamation
    {
    if (strcmp(rec.client_nom, currentuser.username) != 0)              // Verifier si le client est le proprietaire de la reclamation

        printf("Vous n'avez pas le droit de modifier ou supprimer cette reclamation.\n");
        return 0;
    }

                                                            // Verifier si la reclamation est encore "en cours"
    if (strcmp(complaint.status, "en cours") != 0)
        {
        printf("Vous ne pouvez pas modifier ou supprimer une reclamation qui a deja ete traitee.\n");
        return 0;
        }

                                            // Calculer la difference en heures entre la date de soumission et l'heure actuelle
    struct tm tmSubmit = {0};
    sscanf(rec.date, "%d/%d/%d", &tmSubmit.tm_mday, &tmSubmit.tm_mon, &tmSubmit.tm_year);
    tmSubmit.tm_mon -= 1;             // Ajuster le mois
    tmSubmit.tm_year -= 1900;        // Ajuster l'année

    time_t submitTime = mktime(&tmSubmit);
    time_t currentTime = time(NULL);
    double differenceInHours = difftime(currentTime, submitTime) / 3600;                   // Convertir la difference en heures

    if (differenceInHours > 24) {
        printf("Vous ne pouvez pas modifier ou supprimer une reclamation soumise il y a plus de 24 heures.\n");
        return 0;
    }

    return 1;                                   // La reclamation peut etre modifiee ou supprimee
}

void modifier_supprimer_reclamation(user currentuser)                //**** fonction pour modifier ou supprimer les reclamation
{
    int id, choix;

    printf("Entrez ID de la reclamation que vous souhaitez modifier ou supprimer : ");
    scanf("%d", &id);

                                              // Recherche de la reclamation par ID
    int index = -1;
    for (int i = 0; i < reccount; i++)
        {
        if (rec[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        {
        printf("Reclamation non trouvee.\n");
        return;
        }

                                          // Si l'utilisateur est un client, verifier  24h
    if (isClient(currentuser))
        {
        if (!client_modifier_supprimer(rec[index], currentuser))
        {
            return;                                 // Si les conditions ne sont pas respectees, arreter  la fonction
        }
        }

                                                     // Demander modifier ou supprimer
    printf("Que souhaitez-vous faire ?\n1- Modifier la reclamation\n2- Supprimer la reclamation\n");
    scanf("%d", &choix);

    if (choix == 1)
        {
                                                             // modifier de la reclamation
        printf("Entrez le nouveau motif de la reclamation : ");
        scanf(" %[^\n]s", rec[index].motif);
        printf("Entrez la nouvelle description de la reclamation : ");
        scanf(" %[^\n]s", rec[index].description);
        printf("Entrez la nouvelle categorie de la reclamation : ");
        scanf(" %[^\n]s", rec[index].categorie);


        printf("Reclamation modifiee avec succes.\n");

        }
        else if (choice == 2)
            {
                                                       // Supprimer  la reclamation
        for (int i = index; i < reccount - 1; i++)
        {
            crec[i] = rec[i + 1];                       // decaler les elements dans le tableau
        }
        reccount--;                                     // reduire le nombre de reclamations

        printf("Reclamation supprimee avec succes.\n");

        }
        else
            {
        printf("Choix invalide.\n");
            }
}


 void liste_reclamation(Utilisateur currentUser)                  //*** fonction pour afficher la liste des reclamations
 {
                                                                // Vérifier si l'utilsateur est administrateur ou an agent
    if (!isAdministrateur(currentuser) && !isAgent(currentsser))
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
    printf("------------------------------------------------------------\n");
    printf("%-5s %-15s %-20s %-10s %-10s %-15s\n", "ID", "Motif", "Description", "Client", "Statut", "Date");     // (%-5) sont des specificateur de formatage pour bien aligner les colones

                                                                          // Parcourir et afficher toutes les reclamations
    for (int i = 0; i < reccount; i++)

        {
        printf("%-5d %-15s %-20s %-10s %-10s %-15s\n",rec[i].id, rec[i].motif, rec[i].description, rec[i].client_nom, rec[i].status, rec[i].date);
        }
    printf("------------------------------------------------------------\n");
}




void traiter_reclamation (Utilisateur currentuser)                  // ***fonction pour traiter une reclamation
{
    int id, found = 0;
    char newstatus[20];

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

void rechercher_reclamation (Utilisateur currentuser)                         // ***fonction pour rechercher une reclamation
{
    int choix, id, found = 0;
    char client_nom[50], status[50], categorie[50];

    if ( (!isAdministrateur(currentuser) || !isAgent(currentuser)) )
    {
        printf("Vous n'avez pas le droit de rechercher une reclamation !\n");
    }

    printf("---------------Rechercher la reclamation par :---------------\n");
    printf("1- Identifiant de la reclamation :\n");
    printf("2- Le nom du client :\n");
    printf("3- Le status de la reclamation :\n");
    printf("4- La categorie de la reclamation :\n\n");
    printf("Entrer votre choix:\n");

    switch (choix)
    {
    case 1:
        printf("Entrer ID de la reclamation :\n");
        scanf("%d", &id);
        for (int i = 0; i < reccount; i++)
            {
                if (rec[i].id == id) {
                    found = 1;

                    printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                    break;
                }
            }
            break;
    case 2:
        printf("Entrer le nom du client :\n");
        scanf(" %[^\n]s", client_nom);

            for (int i = 0; i < reccount; i++)
                {
                if (strcmp(reccount[i].client_nom, client_nom) == 0)
                    {
                    found = 1;
                    printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                    }
                }
                break;
    case 3:
        printf("Entrer le status de la reclamation:\n");
        scanf(" %[^\n]s", status);

        for (int i = 0; i < reccount; i++)
            {
                if (strcmp(rec[i].status, status) == 0)
                {
                    found = 1;

                    printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                }
            }
            break;
    case 4:

        printf("Entrer la categorie de la reclamation:\n");
        scanf("%s", categorie);

        for (int i = 0; i < reccount; i++)
            {
                if (strcmp(rec[i].categorie, categorie) == 0)
                {
                    found = 1;

                    printf("Reclamation trouvee : Motif : %s, Statut : %s, Client : %s, Date : %s\n",rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);
                }
            }
            break;
    default :
        printf("Choix invalide.\n");
        return;
    }
    if (!found)
        {
            printf("Reclamation non trouvee.\n",id);
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
    Reclamation temp;
    for (int i = 0; i < reccount - 1; i++)
        {
        for (int j = i + 1; j < reccount; j++)
            {
            if (rec[i].priorite < rec[j].priorite)
                {
                    temp = rec[i];
                    rec[i] = rec[j];
                    rec[j] = temp;
                }

            }
        }
}

void afficher_reclamation_priorite (User currentuser)     //  ***fonction pour afficher les reclamations triees par priorite
{
    if (!isAdministrateur(currentuser))
        {
        printf("Acces refusee , Vous n'avez pas les droit d'acceder au reclamations triees par priorite");
        return;
        }

     trier_reclamation_priorite();
     printf("Reclamation triees par priorite:\n");

     for (int i = 0; i < reccount - 1; i++)
     {
        printf("ID : %d, Motif : %s, priorite : %d, Statut : %s, Client : %s, Date : %s\n",rec[i].id,rec[i].priorite,rec[i].motif, rec[i].status, rec[i].client_nom, rec[i].date);

     }
}













