dans ce dossier:
    - freertos fonctionne
    - driver d'uart et protocole de com dispo et adapté à freertos 
    - !!! ATENTION !!! il faut remplacer le ficheir heap2.c par heap4.c si besoin alloc dynamique!!!
    - driver ADC fonctionnel

a faire:
    -adapter les drivers pwm et coder en quadra à freertos
    -ecrire les dirver i2c/spi/ADC
    -prevoir un systeme de debug aisé pour choper le temps de chaque tache
    peut-etre utiliser l'analyseur logique ou un oscillo en fonction du nombre de tache

NE FONCTIONNE PAS:
    - OPTIMISED TASK SELECTION
    - le debug sur la carte j'ai remplacer le reset et go 
    final par un reset no halt (rnh) pour eviter les longs 
    message d'erreurs

/**********************************************************
        NOTE A PROPOS DE FREERTOS
**********************************************************/

    - priorité d'une tache 0 à configMAX_PRIORITY-1
    plus la valeur est faible moins la tache à d'importance
    le niveau 0 est réservé pour idle

    - on peut utiliser la tache idle pour faire tourner en
    fond un/des processes peu important
