#include <dirent.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <errno.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

int reader(char *filename, char *str, struct dirent *dir, char *path) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	int trigger = 0;
	char temp[512];
	char *fullname;
	char *match;

	asprintf(&fullname,"%s/%s",path,filename);
	
	if (strstr(filename,str))
	{
		if (!(dir->d_type & DT_DIR)){
			printf( RED "%s" RESET "\n--filename matches\n" , fullname);
		}
		else 
		{
			printf( RED "%s" RESET " (dir)\n--dir name matches\n" , fullname);
		}
	}
	
	if((fp = fopen(fullname, "r")) == NULL) {
		return(-1);
		printf("%s\n",fullname);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			temp[strlen(temp) - 1];
			//printf("A match found on line: %d\n", line_num);
			if (trigger == 0) {
				printf(RED "%s\n" RESET,fullname);
			}
			if (strlen(temp) > 50) {
				//~ match = strstr(temp,str);
				printf("--%d\t...\n" , line_num);
			}
			else {
				printf("--%d\t%s\n" , line_num,temp);
			}
			find_result++;
			trigger = 1;
		}
		line_num++;
	}

	if(find_result == 0) {
		//printf("\nSorry, couldn't find a match.\n");
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}


list_dir (char * dir_name, char *str)
{
    DIR * d;

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (d) {
       // fprintf (stderr, "Cannot open directory '%s': %s\n",
      //          dir_name, strerror (errno));
        //exit (EXIT_FAILURE);
    
		while (1) {
			struct dirent * entry;
			char * d_name;

			/* "Readdir" gets subsequent entries from "d". */
			entry = readdir (d);
			if (! entry) {
				/* There are no more entries in this directory, so break
				   out of the while loop. */
				break;
			}
			d_name = entry->d_name;
			/* Print the name of the file and directory. */
		//printf ("%s/%s\n", dir_name, d_name);


		/* If you don't want to print the directories, use the
		   following line: */
			reader(d_name,str,entry,dir_name);
			if (! (entry->d_type & DT_DIR)) 
			{
				
			}

			if (entry->d_type & DT_DIR) 
			{
				/* Check that the directory is not "d" or d's parent. */
				
				if (strcmp (d_name, "..") != 0 &&
					strcmp (d_name, ".") != 0) {
					int path_length;
					char path[PATH_MAX];
	 
					path_length = snprintf (path, PATH_MAX,
											"%s/%s", dir_name, d_name);
				//	printf ("%s\n", path);
					if (path_length >= PATH_MAX) {
						fprintf (stderr, "Path length has got too long.\n");
						exit (EXIT_FAILURE);
					}
					/* Recursively call "list_dir" with the new path. */
					list_dir (path,str);
				}
			}
		}
		/* After going through all the entries, close the directory. */
		if (closedir (d)) {
			fprintf (stderr, "Could not close '%s': %s\n",
					 dir_name, strerror (errno));
			exit (EXIT_FAILURE);
		}
	}
}


int main(int argc, char *argv[])
{
	DIR				*d1;
	struct dirent 	*dir;
	char 			*filename;
	d1 = opendir(".");
	if(argc != 2)
	{
	perror("Please enter search string\n");	
	exit(0);
	}
	if (d1)
	{
		while ((dir = readdir(d1)) != NULL)
		{
			filename = dir->d_name;
			if (dir->d_type & DT_DIR){
				if (strcmp(filename,"..") & strcmp(filename,"."))
				{
					//~ printf("%s\n",filename);
					list_dir(filename, argv[1]);
				}
			}
			
		
			//reader(filename,argv[1],dir,"/");
		}
		closedir(d1);
	}
	return(0);
}

