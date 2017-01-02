#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>

#include "cmdlib.h"

#define COL_RED "\x1b[31m"
#define COL_OFF "\x1b[0m"

int main(int argc, char *argv[]){
	int verboseFlag = 0;
	int backupFlag = 0;

	FILE *pfile;

	char newSource[100];
	char newDest[100];
	int argindex;
	char **fileNames;
	int fileNames_len;
	int index = 0;  /*	Pour parcourir des tables en boucles locales; utilisée pour p.ex.
			pour stocker les noms des répertoire  */

	int t;
	int opterr=0;
	struct option longopts[] = {
		{"verbose", no_argument, NULL, 'v'},
		{"backup", no_argument, NULL, 'b'},
		{0, 0, 0, 0}
	};

	while((t = getopt_long(argc, argv, "vb", longopts, NULL)) != -1){
		switch(t){
			case 'v':
				verboseFlag = 1;
			break;
			case 'b':
				backupFlag = 1;
			break;
			case '?':
			default:
			abort();
		}
	}

	fileNames_len=argc-optind;
	if(fileNames_len != 2) {
		printf("Nombre de paramètres incorrecte.\n");
		exit(EXIT_FAILURE);
	}
	else {
		fileNames = calloc(fileNames_len, sizeof(char*));
		for(argindex = optind; argindex < argc; argindex++){
			fileNames[argindex-optind] = strdup(argv[argindex]);
		}
		//Test error on source file
		//0-NULL , 17-EEXIST , 21-EISDIR
		strcpy(newSource, fileNames[fileNames_len-2]);
//		pfile = fopen(newSource, "rwb");
		//if source doesnt exist
		if(access(newSource, F_OK) == -1) {
			printf("Le fichier source ["COL_RED"%s"COL_OFF"] n'existe pas.\n", newSource);
			free(fileNames);
			return EXIT_FAILURE;
		}
		else {
			strcpy(newDest, fileNames[fileNames_len-1]);
			pfile = fopen(newDest, "wb");
			//if destination file is a directory
			if(errno == 21) {
				char *baseName;
				int indexlast = strlen(fileNames[fileNames_len-1])-1;
				baseName = basename(fileNames[fileNames_len-2]);
				//if there is already a '/' at the end of destination file
				if(fileNames[fileNames_len-1][indexlast] == '/')  fileNames[fileNames_len-1][indexlast] = '\0';
				strcpy(newDest, paths_concat(fileNames[fileNames_len-1], baseName));
				errno=0;
			}
//			printf("Source: %s\nDestination: %s\n", newSource, newDest);
			if(errno == 0) {
				//if destination file exists
				if(access(newDest, F_OK) != -1) {
					if(backupFlag) {
						strcat(newDest, "~");
						if(verboseFlag)
							printf("Fichier copié de: "COL_RED"%s"COL_OFF"\n vers: "COL_RED"%s"COL_OFF"\n", newSource, newDest);
						copy_file(newSource, newDest);
					}
					else {
						printf("Le fichier "COL_RED"%s"COL_OFF" existe déjà.\n", newDest);
						free(fileNames);
						return EXIT_FAILURE;
					}
				}
				else {
					if(verboseFlag)
						printf("Fichier copié de: "COL_RED"%s"COL_OFF"\n vers: "COL_RED"%s"COL_OFF"\n", newSource, newDest);
					copy_file(newSource, newDest);
				}
			}
			else {
				perror("ERREUR");
				free(fileNames);
				return EXIT_FAILURE;
			}
		}
	}
//	printf("source: %s\n",fileNames[fileNames_len-2]);
//	printf("destination: %s\n",fileNames[fileNames_len-1]);

	free(fileNames);
return EXIT_SUCCESS;
}
