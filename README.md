# Projet Robot

### Guide de répertoires
- `libraries` contient les bibliotheques nécessaires pour interagir avec le hardware attaché au robot. La bibliothèque `NewPing` nous permet d'interagir avec le capteur acoustique, `Servo` nous permet d'interagir avec le moteur pour changer la direction du capteur acoustique, et finalement `ejovo` est une bibliothèque écrit par nous pour stocker la pièce cartographé dans une facon ultra efficace en utilisant une matrice (abstraction de tableau de deux dimension) des `uint8_t` pour répresenter un carré dans la vraie pièce avec un bit (1 pour un mur, 0 pour le sol). Finalement, le programme C `read_matrix.c` nous permet de transformer la matrice des `uint8_t` enregistré par le robot cartographeur à une visualisation dans le terminale.
- `Servo_testing` contient un programme arduino pour tester la fonctionnement du moteur qui dirige le capteur acoustique.

## ./