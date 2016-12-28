
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

struct option longopts [] = {
  {"help", no_argument, 0, 'h'},
  {"changes", no_argument, 0, 'c'},
	{"verbose",no_argument,0,'v'},
  {0,0,0,0}
};
int main (int argc, char *argv[]) {
  int v = 0;
  int c = 0;
  int ch = 0;
	char *mode = argv[optind];
  char *buf = argv[optind+1];
  mode_t mode1;
  mode_t mode2;
  mode_t mode3;
  
  while ((ch = getopt_long(argc, argv, "hvc", longopts, NULL)) != -1){
    switch (ch){
      case 'h':
        printf("chmod[OPTION]... MODE[MODE]...FILE ...\n"\
               "chmod : Modifier le mode de chaque fichier\n" \
               "chmod permission : r(read) = 4, w(write) = 2, x(execute) = 1\n"\
               "0:No access, 1:execute, 2:write, 3:write & execute, 4:read\n"\
               "5:read & execute, 6:read & write, 7:read, write & execute\n"\
               " -v, --verbose: afficher un diagnostic pour chaque fichier traité.\n"\
               " -c, --changes: comme --verbose, mais seulement en cas de modification\n");
        return EXIT_SUCCESS;
      case 'c':
        c = 1;
        break;
		  case 'v':
				v = 1;
				break;
			case 0: 
        break;// getopt_long() set a variable, just keep going
      case ':':
        fprintf(stderr, "%s: option '-%c' requires an argument.\n", argv[0], optopt);
        break;
      case '?':
      default:
        fprintf(stderr, "%s: option '-%c' is invalid.\n", argv[0], optopt);
    }
  }
  if (argc < 3){
    fprintf(stderr, "chmod error : few argument\n");
    exit(EXIT_FAILURE);
  }
  if ((strlen(mode) != 4) | ((mode[0] - '0') != 0) | ((mode[1] - '0') > 7)
           | ((mode[2] - '0') > 7) | ((mode[3]  - '0' )> 7)) {
    printf("%c\n", mode[2]);
    printf("input error \n");
    exit(EXIT_FAILURE);
  }
  if(buf == NULL){
    printf("no path entered \n");
  }
  
  switch (mode[0]){
    case '1':
      mode0 = S_ISVTX;
      break;
    case '2':
      mode0 = S_ISGID;
      break;
    case '3':
      mode0 = S_ISVTX | S_ISVTX;
      break;
    case '4':
      mode0 = S_ISUID;
      break;
    case '5':
      mode0 = S_ISUID | S_ISVTX;
      break;
    case '6':
      mode0 = S_ISUID | S_ISGID;
      break;
    case '7':
      mode0 = S_ISUID | S_ISGID | S_ISVTX;
      break;
    case '0':
    default:
      mode0 = 0;
      break;
  }
  
  switch (mode[1]){
    case '1':
      mode1 = S_IXUSR;
      break;
    case '2':
      mode1 = S_IWUSR;
      break;
    case '3':
      mode1 = S_IXUSR | S_IWUSR;
      break;
    case '4':
      mode1 = S_IRUSR;
      break;
    case '5':
      mode1 = S_IXUSR | S_IRUSR;
      break;
    case '6':
      mode1 = S_IRUSR | S_IWUSR;
      break;
    case '7':
      mode1 = S_IXUSR;
      break;
    case '0':
    default:
      mode1 = 0;
      break;
  }
  
  switch (mode[2]){
    case '1':
      mode2 = S_IXGRP;
      break;
    case '2':
      mode2 = S_IWGRP;
      break;
    case '3':
      mode2 = S_IXGRP | S_IWGRP;
      break;
    case '4':
      mode2 = S_IRGRP;
      break;
    case '5':
      mode2 = S_IXGRP | S_IRGRP;
      break;
    case '6':
      mode2 = S_IRGRP | S_IWGRP;
      break;
    case '7':
      mode2 = S_IXGRP;
      break;
    case '0':
    default:
      mode2 = 0;
      break;
  }
  
  switch (mode[3]){
    case '1':
      mode3 = S_IXOTH;
      break;
    case '2':
      mode3 = S_IWOTH;
      break;
    case '3':
      mode3 = S_IXOTH | S_IWOTH;
      break;
    case '4':
      mode3 = S_IROTH;
      break;
    case '5':
      mode3 = S_IXOTH | S_IROTH;
      break;
    case '6':
      mode3 = S_IROTH | S_IWOTH;
      break;
    case '7':
      mode3 = S_IXOTH
      break;
    case '0':
    default:
      mode3 = 0;
      break;
  }
  
  if (v == 1){
    printf("Le fichier %s va recevoir %s comme droits d'accès \n", argv[3], argv[2]);
  }
  
  if (chmod(argv[3], mode0 | mode1 | mode2 | mode3) < 0){
    printf ("INPUT error \n");
    fprintf (stderr, "chmod error : faux droits d'accès \n");
    exit(EXIT_FAILURE);
  }
  
  if (c == 1){
    printf("le fichier %s a obtenu des %s autolisations \n", argv[2]);
  }
  return EXIT_SUCCESS;
}
