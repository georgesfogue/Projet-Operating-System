

int startsWith(const char *pre, const char *str)
{
	size_t lenpre = strlen(pre),
		lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

int is_a_file(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

char* paths_concat(const char *a, const char *b)
{
    char *res;
    if ((res = malloc(strlen(a)+strlen(b)+2 * sizeof(char))) == NULL)
        return res;
    res[0] = '\0';

    strcat(res, a);
    strcat(res, "/");
    strcat(res, b);

    return res;
}

int copy_file(char *old_filename, char  *new_filename) {
		FILE  *ptr_old, *ptr_new;
		int err = 0, err1 = 0;
		int  a;

		ptr_old = fopen(old_filename, "rb");
		ptr_new = fopen(new_filename, "wb");

		if(err != 0)
			return  EXIT_FAILURE;

		if(err1 != 0)
		{
			fclose(ptr_old);
			return  EXIT_FAILURE;
		}

		while(1)
		{
			a  =  fgetc(ptr_old);

			if(!feof(ptr_old))
				fputc(a, ptr_new);
			else
				break;
		}

		fclose(ptr_new);
		fclose(ptr_old);
		return  0;
}


