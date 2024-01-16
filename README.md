# Projet Kart ESE GEII UGA

## Description

Ce projet universitaire vise à concevoir et développer le logiciel embarqué pour le contrôle d'un kart électrique à l'aide d'un microcontrôleur PIC18F. Le code source est écrit en langage C et est destiné à être flashé sur le microcontrôleur embarqué dans le kart.

## Objectif

L'objectif principal de ce code est de permettre le contrôle sûr et efficace du kart électrique, en prenant en charge la gestion du moteur électrique, le système de freinage, la communication avec les capteurs. Il faut savoir que le moteur est piloté en couple grâce a ces capteurs.

## Matériel

- **Microcontrôleur**: Le code est conçu pour être exécuté sur un microcontrôleur PIC18F.

- **Capteurs**: Le kart est équipé de capteurs tels qu'un capteur de courant et des potentiomètres nécessaires à la collecte de données en temps réel.

## Fonctionnalités

1. **Contrôle du moteur**: Le code gère la vitesse du moteur électrique en fonction des commandes utilisateur, en veillant à un démarrage en douceur et à une accélération contrôlée et bien entendu un pilotage en couple du moteur.

2. **Système de freinage**: La gestion du freinage est implémentée pour assurer un arrêt sûr du kart en réponse aux commandes de freinage de l'utilisateur.

3. **Communication série**: Dans le futur, le microcontrôleur communiquerait avec un système central via une liaison série pour la surveillance à distance et la collecte de données.

4. **Gestion des capteurs**: Le code collecte et traite les données provenant des capteurs pour fournir des informations en temps réel sur la vitesse, la position, et d'autres paramètres importants du kart et gèrer la vitesse du kart.

## Installation

1. **Environnement de développement**: Assurez-vous d'avoir installé l'environnement de développement MPLABX IDE, spécifié dans la documentation technique du microcontrôleur.

2. **Compilation**: Utilisez le compilateur XC8 pour compiler le code source.

3. **Flashage**: Flasher le microcontrôleur avec le fichier binaire généré après la compilation.

## Documentation supplémentaire

Consultez la documentation technique fournie avec le matériel pour des informations détaillées sur le microcontrôleur, les broches, les spécifications matérielles, et d'autres détails importants.

## Auteurs

- [Bouche Gauvain]
## Licence

Ce code est distribué sous la licence [Iut-1_GEII_ESE].
