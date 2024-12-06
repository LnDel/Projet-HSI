#Permet d'accéder aux fichiers src : source .c et entêtes : head .h du projet
srcdir = ./src/
#Permet d'accéder aux fichiers objets et executables du projet
bindir = ./bin/
#Contient la documentation technique du projet
docdir = ./doc/

CC = gcc -Wall
CP = cp
MV = mv
RM = rm -f
#Contient tous les fichiers .c du dossier src
SRC = $(wildcard $(srcdir)*.c)
#Contient tous les fichiers .h du dossier src
HEAD = $(wildcard $(srcdir)*.h)
#Contient tous les fichiers .o
OBJ = $(subst $(srcdir), $(bindir),$(SRC:.c=.o))

#Contient le nom de l'exécutable
PROG = main

#Le programme s'exécute tant que le programme n'est pas créé
all : $(PROG)

#Création du programme final à partir des .o et on supprime les fichiers inutiles (dernière étape)
$(PROG) : $(OBJ)
	$(CC) $^ -o $@ -lm
	$(RM) -r $(bindir)*
	$(MV) $(PROG) $(bindir)

#Création des .o à partir des .c
$(bindir)%.o : $(srcdir)%.c
	$(CC) -c $^ -o $(@) -lm

#Evite les conflits de notation
.PHONY : clean doc run restore folders


#Copie les .c et .h de save dans src 
restore :
	$(RM) -r $(srcdir)*
	$(CP) $(savedir)* $(srcdir)

#On crée la doc
doc :
	$(RM) -r $(docdir)*
	doxygen -g
	sed -i 's/^INPUT\s*=\s*$$/INPUT = src/' Doxyfile
	doxygen Doxyfile
	$(MV) html $(docdir)
	$(MV) latex $(docdir)
	$(MV) Doxyfile $(docdir)

#On crée les dossiers
folders :
	mkdir src
	mkdir bin
	mkdir doc

#Execute l'executable
run :
	$(bindir)$(PROG)