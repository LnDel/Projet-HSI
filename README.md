# Projet-HSI

## Compilation et exécution du projet

### Compiler le projet :
```bash
make
```

### Exécuter le driver
Dans un premier terminal :
```bash
make driver
```

### Lancer docker
Dans un deuxième terminal :
```bash
cd docker
sudo docker-compose up
```

### Exécuter le main
Dans un troisième terminal :
```bash
make run
```

### Compiler puis exécuter l'application en une seule commande
```bash
make all_do
```

## Explication des questions

### Question 1
Afin de stocker tous les types et toutes les variables utiles à notre projet nous avons choisi un document .json  
Le document contient un premier objet avec la définition de tous les types et un deuxième avec les variables concernant le BCGV
Ce document fut modifier plusieurs fois selon nos besoins.

### Question 2
Pour générer automatiquement la librairie, nous nous sommes orientés vers un script python qui va venir créer un fichier un .c et le header associé.

### Question 3
Le main récupère les trame dans une boucle puis vient les afficher dans un premier temps. Ensuite nous l'avons amélioré au fur et à mesure de notre avancement sur les autres questions. 

### Question 4
Pour travailler sur les states machines, nous avons choisi de nous séparer en deux pour différentes state machine. Tout d'abord, nous n'avions pas compris que selon les diagrammes, on devait potentiellement faire plusieurs state machines. Nous en avions donc que 3 au début. On ne pouvait pas tester si elles fonctionnaient car on avait besoin des fonction décode pour voir le résultat.
On a compris plus tard qu'il fallait plusieurs state machine suivant le diagramme ce qui nous a pas pris beaucoup de temps à faire car il fallait souvent suivre le même principe.
Nous avons quand même conscience que le timer est mal géré dans notre programme mais nous n'avons pu trouver la solution à temps.

### Question 5
La question a été mal comprise dans un premier lieu, nous avions fait un seul makefile au total. Après s'être rendu compte de l'erreur nous avons crée les deux autres makefile afin de respecter la consigne (il suffisait juste de séparer ce qui avait déjà été fait en plusieurs fichiers).
A la fin, nous avons rajouté des commandes afin de faciliter le lancement de l'application (une commande pour lancer l'application, une pour lancer le driver une pour compiler puis lancer l'application).

### Question 6
Pour créer les fonctions de décodage, nous avons créé 2 fonctions : une pour la ligne série et une pour la ligne UDP. Ces fonctions viennent lire les valeurs de chaque bit/octet des trames reçues et vient mettre à jour en conséquence les valeurs associées.

### Question 7
Chaque trame a sa fonction d'encodage. La fonction lit les valeurs du BCGV et crée la trame en conséquence.  
De plus, la fonction de validation vient prendre 2 trames et valide qu'elles soient identiques.

### Question 8
Pour cette question nous avions pris de l'avance. En effet après avoir lu cette question, nous avons pris la liberté de mettre des commentaires dans notre main afin de structurer le fichier app.c dès le début. On ajoutait donc chaque fonction après que les tests sur une autre branche furent concluants.
C'est de cette manière que nous avons pu remarquer que nous avons oublier de gérer la partie Log Erreur et nous nous en excusons.