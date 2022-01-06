# Projet Robot

## Guide de répertoires
- `libraries` contient les bibliotheques nécessaires pour interagir avec le hardware attaché au robot. La bibliothèque `NewPing` nous permet d'interagir avec le capteur acoustique, `Servo` nous permet d'interagir avec le moteur pour changer la direction du capteur acoustique, et finalement `ejovo` est une bibliothèque écrit par nous pour stocker la pièce cartographé dans une facon ultra efficace en utilisant une matrice (abstraction de tableau de deux dimension) des `uint8_t` pour répresenter un carré dans la vraie pièce avec un bit (1 pour un mur, 0 pour le sol). Finalement, le programme C `read_matrix.c` nous permet de transformer la matrice des `uint8_t` enregistré par le robot cartographeur à une visualisation dans le terminale.
- `Servo_testing` contient un programme arduino pour tester la fonctionnement du moteur qui dirige le capteur acoustique.
- `media` contient des photos de la pièce cartographée.

### ./
- `main.ino` contient le code qui régit notre petit robot cartographeur.
- `read` est un binaire (dont le code source est disponible dans `read_matrix.c`) compilé avec `gcc` sur un x86_64 système linux. Ce programme accepte comme donnée d'entrée un fichier `.txt` d'une matrice de dimension 2 des `uint8_t` qui repésente une pièce cartographé par notre robot. `read` affiche au terminal une visualization de la pièce.
- `serial_out.txt` est un exemple des données de sorties de notre robot cartographeur, pris du terminal serial de l'arduino. Pour visualiser l'exemple, appeler `./read < serial_out.txt`. Attention, l'exemple de `serial_out.txt` n'est pas une vraie donnée de sortie de notre robot, il a été créé pour tester la fonctionnement du binaire `read`.

