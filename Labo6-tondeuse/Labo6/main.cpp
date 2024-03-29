//------------------------------------------------------------------------------
// Fichier     main.cpp
// Auteur      GM Breguet
// Date        21.11.2022
//
// But         Tondeuse automatique
//
// Remarque
//
// Modifs      n/a
//
// Compilateur
//------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include "tondeuse.h"

using namespace std;

//------------------------------------------------------------------------------
int main(){
   
   //---------------------------------------------------------------------------
   // Conditions d'utilisation
   // 1) Le terrain n'est pas nécessairement d'une forme régulière.
   // 2) Le périmètre doit être fermé par un fil de limite ou par un obstacle.
   //    Sans quoi le comportement est imprévisible (la tondeuse se perd).
   // 3) La position initiale de la tondeuse est correcte.
   // 4) La tondeuse se déplace d'un pas à la fois (Haut, Bas, Gauche, Droite)
   //    => pas de déplacement en diagonale.
   // 5) L'herbe haute est coupée au passage de la tondeuse.
   //    Les parties qui ne sont pas atteintes (ou atteignables) restent inchangées.
   //
   // Codage du terrain ci-dessous
   // - L : limite
   // - X : obstacle
   //
   // Représentation à l'affichage
   // - '#' : limite
   // - 'X' : obstacle
   // - '~' : herbe haute
   // - '.' : herbe coupée
   //---------------------------------------------------------------------------
                      //                    1                   2
                      //0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
   Terrain terrain = { {L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,X,X,X,X},  //  0
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,X},  //  1
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,X},  //  2
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  //  3
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  //  4
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,H,H,H,H,L},  //  5
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,H,H,H,H,L},  //  6
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,H,H,H,H,L},  //  7
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,H,H,H,H,L},  //  8
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,X,X,X,H,H,H,H,L},  //  9
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 10
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 11
                       {L,H,H,H,H,H,H,H,H,H,X,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 12
                       {L,H,H,H,H,H,H,H,H,X,H,H,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 13
                       {L,H,H,H,H,H,H,H,H,X,H,H,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 14
                       {L,H,H,H,H,H,H,H,H,H,X,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 15
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 16
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 17
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 18
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 19
                       {L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},  // 20
                       {L,H,H,H,X,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L,L,L,L,L},  // 21
                       {L,H,H,H,X,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},          // 22
                       {H,L,H,H,X,X,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},          // 23
                       {H,H,L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},          // 24
                       {H,H,H,L,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,L},          // 25
                       {H,H,H,H,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L} };        // 26

   // position initiale de la tondeuse
   Tondeuse tondeuse = {1, 3};

   // tondre => n déplacements aléatoires
   tondre(terrain,      // terrain de travail
          tondeuse,     // tondeuse
          10'000,      // nbre de pas
          true);        // afficher à chaque pas


   return EXIT_SUCCESS;
}
