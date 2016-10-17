
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE * fp1=fopen(argv[1], "r");
	FILE * fp2=fopen(argv[2], "r");
	FILE * out_fp=fopen(argv[3], "w+");
	if(out_fp==NULL || fp1==NULL || fp2==NULL)
	{
		//printf("Cannot open 1 of the files\n");
		return 1;
	}
	//printf("files opened. ");
	//init file vars
	size_t line_size = 0;
	ssize_t read;
	char * line = NULL;
	read = getline(&line, &line_size, fp2 );
	//get number of vertices
	fprintf(out_fp, "%s", line);
	//discard this value
	read = getline(&line, &line_size, fp1 );
	//loop
	//printf("adding small file... ");
	while( read = getline(&line, &line_size, fp1 )!= -1)
	{
		if(atoi(line)==-1)
		{
			break;
		}
		else
		{
			fprintf(out_fp,"%s", line);
		}
	}
	//printf("adding big file... ");
	while( read = getline(&line, &line_size, fp2 )!= -1)
	{
		if(atoi(line)==-1)
		{
			break;
		}
		else
		{
			fprintf(out_fp,"%s", line);
		}
	}
	//printf("done\n");
	fclose(fp1);
	fclose(fp2);
	fclose(out_fp);
	return 0;
}
