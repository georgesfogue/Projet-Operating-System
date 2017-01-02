#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "cmdlib.h"

#define COL_RED "\x1b[31m"
#define COL_OFF "\x1b[0m"

int main(int argc, char *argv[]){
	int allFlag = 0;
	int longFlag = 0;
	int recursiveFlag = 0;
	char *cvalue = NULL;

	DIR *dir;
	struct dirent *ent;


	char path[1024];
	int argindex;
	char **fileNames;
	int fileNames_len;
	char **dirNames;
	int dirNames_len;
	int index = 0;  /*	Pour parcourir des tables en boucles locales; utilisée pour p.ex.
			pour stocker les noms des répertoire  */

	int t;
	int opterr=0;
	struct option longopts[] = {
		{"all", no_argument, NULL, 'a'},
		{"recursive", no_argument, NULL, 'R'},
		{0, 0, 0, 0}
	};

	while((t = getopt_long(argc, argv, "alR", longopts, NULL)) != -1){
		switch(t){
			case 'a':
				allFlag = 1;
			break;
			case 'l':
				longFlag = 1;
			break;
			case 'R':
				recursiveFlag = 1;
			break;
			case '?':
			default:
			abort();
		}
	}

	fileNames_len=argc-optind;
	if(fileNames_len == 0) {
		fileNames_len=1;
		/*Allocation de la mémoire avec initialisation pour le tableau ou l'on stockera les noms
		 des répértoires*/
		fileNames = calloc(fileNames_len, sizeof(char*));
		fileNames[0] = strdup(".");
	}
	else	fileNames = calloc(fileNames_len, sizeof(char*));
		for(argindex = optind; argindex < argc; argindex++){
			fileNames[argindex-optind] = strdup(argv[argindex]);
		}

	strcpy(path, " ");
	for(argindex = 0; argindex < fileNames_len; argindex++){
		if ((dir = opendir(fileNames[argindex])) != NULL) {
			/*Affichage de tous les fichiers et repertoires dans dir*/
			printf(COL_RED "%s:\n" COL_OFF,fileNames[argindex]);
			while((ent = readdir(dir)) != NULL){
				//Si l'option est -a: Inclure les fichiers commencant par '.' 
				if(allFlag)	printf("%s\n", ent->d_name);
				else if(!startsWith(".", ent->d_name))	printf("%s\n", ent->d_name);
				//Si l'option est -R: Envoyer une nouvelle commande avec au Main avec les sous-repertoires
				if((ent->d_type == 4) && !startsWith(".", ent->d_name) && (recursiveFlag)) {
					strcat(path, fileNames[argindex]);
					strcat(path, "/");
					strcat(path, ent->d_name);
					strcat(path, " ");
				}

			}
			printf("Sous-répertoires trouvés: %s\n", path);
			if(strlen(path) > 3) //Recursive call to Main to be implemented.
					continue; //return main(argc, path);
			closedir(dir);
		}

		else{
			/*Impossible d'ouvrir le repertoire dir*/
			perror("");
			return EXIT_FAILURE;
		}
	}


/*	printf("option index %d\n",optind);
	printf("argc %d\n",argc);
	printf("optopt %c\n",optopt);
*/
	free(fileNames);
return EXIT_SUCCESS;
}

