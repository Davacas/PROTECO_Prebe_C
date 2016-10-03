#ifndef GITFUNCS_H_INCLUDED
#define GITFUNCS_H_INCLUDED
	char* itoa(int i, char b[]);
	void initCmd(char *a, DIR* b);
	void addCmd(char *a, char *b, int c);
	void commitCmd(char *a, char *b, char **comentario);
	void statusCmd(char *a);
	void logCmd(char *a);
	void otroCmd (char *a);
	void noArgs ();
#endif