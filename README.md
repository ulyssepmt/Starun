# Starun 👾

(Mon premier) Petit jeu codé en C sur Code:Blocks, utilisant les APIs Windows (GDI32) et la librairie OpenAL pour la gestion des sons. 

L'objectif pour le joueur consiste à rejoindre la Terre lors d'une mission de reconnaissance dans l'espace. Il doit avant cela affronter (exploser ou esquiver) les terribles aliens et asteroids se mettant en travers de son chemin et ramasser des pièces qui lui redonneront des points de vie et augmenteront son score.  
La vitesse des aliens augmente à chaque niveau (en tout 5 avec un boss à la fin, 10 aliens par niveaux).  

Le jeu utilise cinq threads principaux permettant de générer les textures des enemis (les aliens, les extras (pièces et asteroids)), le joueur, le raffraichissment d'écran et la gestion du jeu. 
Un thread secondaire est appelé quand la touche [X] est utilisée, donnant la possiblité au joueur d'abattre les ennemis. 

Ce projet fût un défi personnel (même si cela ne paraît pas énorme) mettant en avant la manipulation des structures de données et la gestion de la mémoire (éviter au maximum les leaks, etc). Les développeurs C avec un niveau intermédiaire/correct pourront penser que certaines parties du code ne suivent pas une logique correcte ou que le code n'est pas parfait (ce qui est fort probable). Si c'est le cas, n'hésitez pas à commenter dans l'onglet "issues" et je mettrais à jour le jeu ! 

# UPDATE 21/04/24: 
- Nerf de la vitesse des aliens (1.5->1.25 incrémenté de 1 / niveaux). 
- Les obstacles percutés ne donnent plus de malus. 

# UPDATE 22/03/24: 
- Ajout des extras.
- Ajout d'un boss au niveau 5.
- Changements du score, le joueur doit ramasser des pièces au lieu d'abattre les aliens qui lui donnaient des points.
- Corrections apportées au niveau des bugs graphiques.
- Correction du code sur certaines parties illogiques.


![cap1](https://github.com/ulyssepmt/Starun/assets/89702597/36d6e675-848f-44c5-b7f1-885dd2a389cd)

![cap2](https://github.com/ulyssepmt/Starun/assets/89702597/c6a3c420-7940-4fa6-94ac-6c0d8fb31245)



# Démo : 
[UPDATED 21/04/2024] [[https://youtu.be/ZcdnBqL0IDY]](https://youtu.be/w2Jc2N13QQg)

https://youtu.be/v-Lb9ldcQ7E


# Crédits : 

Bitmaps utilisés : macrovector (https://www.freepik.com/author/macrovector)

Son de victoire : Lion Cookie Sounds (le reste des autres sons sont frees)

Icon utilisé : goodstuffnononsense (https://goodstuffnononsense.com)



