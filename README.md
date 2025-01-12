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

### Question 2
Pour générer automatiquement la librairie, nous nous sommes orientés vers un script python qui va venir créer un fichier un .c et le header associé.

### Question 3
Le main récupère la trame puis à l'aide de boucle il vient l'afficher


### Question 4



### Question 5


### Question 6
Pour créer les fonctions de décodage, nous avons créer une 2 fonctions : une pour la ligne série et une pour la ligne UDP. Ces fonctions viennent lire les valeurs de chaque bit/octet des trames reçues et vient mettre à jour en conséquence les valeurs associées.

### Question 7
Chaque trame a sa fonction d'encodage. La fonction lit les valeurs du BCGV et créer la trame en conséquence.  
De plus, la fonction de validation vient prendre 2 trames et valide qu'elles soient identiques

### Question 8


