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
char   role[50];
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


int contientspecialchar( char * password)    // ***fonction pour inscrire et verifier les critreres de password
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

void signUp()                       // fonction pour creer un compte
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
    printf("Choisir un role (1- Administrateur, 2- Agent, 3- Client) :\n");
    scanf("%s", newuser.role);

    newuser.estVerrouille = 0;
    user[usercount++] = newuser;                          // ajouter l'utilisateur dans le tableau user[]

    printf("Votre inscription a ete completee avec succes");
}

int signIn()                            // fonction pour connecter un compte
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
               printf("Vous etes connecter avec succees");
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



