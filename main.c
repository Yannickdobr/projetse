#include <stdio.h>
#include <stdlib.h>
#include "entreprise_allocation.h"

// Fonction pour afficher le menu principal
void afficher_menu_principal() {
    printf("\n===============================================\n");
    printf("    SYSTEME D'ALLOCATION DES RESSOURCES\n");
    printf("         ENTREPRISE DE VENTE ELECTRONIQUE\n");
    printf("===============================================\n");
    printf("1. Afficher le stock des produits\n");
    printf("2. Afficher la liste des employes\n");
    printf("3. Simuler des demandes clients\n");
    printf("4. Afficher les demandes en cours\n");
    printf("5. Strategie d'allocation STATIQUE\n");
    printf("6. Strategie d'allocation DYNAMIQUE\n");
    printf("7. Strategie d'allocation EQUITABLE\n");
    printf("8. Strategie d'allocation PRIORITAIRE\n");
    printf("9. Comparaison des strategies\n");
    printf("10. Reinitialiser le systeme\n");
    printf("0. Quitter le programme\n");
    printf("===============================================\n");
    printf("Votre choix : ");
}

// Fonction pour attendre l'entrée utilisateur
void attendre_entree() {
    printf("\nAppuyez sur Entree pour continuer...");
    getchar(); // Pour capturer le '\n' restant
    getchar(); // Pour attendre l'appui sur Entrée
}

// Fonction pour effacer l'écran
void clear_screen() {
    system("clear || cls"); // Fonctionne sur Linux/Mac et Windows
}

// Fonction principale
int main() {
    SystemeEntreprise entreprise;
    int choix;
    int demandes_simulees = 0;
    
    // Initialisation du système
    initialiser_entreprise(&entreprise);
    
    printf("===============================================\n");
    printf("    SYSTEME D'ALLOCATION DES RESSOURCES\n");
    printf("         ENTREPRISE DE VENTE ELECTRONIQUE\n");
    printf("===============================================\n");
    
    printf("\nInitialisation du systeme...\n");
    printf("- %d produits electroniques crees\n", entreprise.nb_produits);
    printf("- %d employes initialises\n", entreprise.nb_employes);
    printf("- Systeme pret a fonctionner\n");
    
    do {
        afficher_menu_principal();
        scanf("%d", &choix);
        
        switch (choix) {
            case 1:
                clear_screen();
                afficher_stock(&entreprise);
                break;
                
            case 2:
                clear_screen();
                afficher_employes(&entreprise);
                break;
                
            case 3:
                clear_screen();
                simuler_scenarios(&entreprise);
                demandes_simulees = 1;
                printf("\nSimulation terminee. Utilisez les options 5-8 pour appliquer les strategies.\n");
                break;
                
            case 4:
                clear_screen();
                afficher_demandes(&entreprise);
                break;
                
            case 5:
                clear_screen();
                if (!demandes_simulees) {
                    printf("Veuillez d'abord simuler des demandes (option 3)!\n");
                } else {
                    printf("=== APPLICATION STRATEGIE STATIQUE ===\n");
                    for (int i = 0; i < entreprise.nb_demandes; i++) {
                        traiter_demande(&entreprise, i, 1);
                    }
                    printf("\n=== ETAT FINAL DU STOCK ===\n");
                    afficher_stock(&entreprise);
                }
                break;
                
            case 6:
                clear_screen();
                if (!demandes_simulees) {
                    printf("Veuillez d'abord simuler des demandes (option 3)!\n");
                } else {
                    printf("=== APPLICATION STRATEGIE DYNAMIQUE ===\n");
                    for (int i = 0; i < entreprise.nb_demandes; i++) {
                        traiter_demande(&entreprise, i, 2);
                    }
                    printf("\n=== ETAT FINAL DU STOCK ===\n");
                    afficher_stock(&entreprise);
                }
                break;
                
            case 7:
                clear_screen();
                if (!demandes_simulees) {
                    printf("Veuillez d'abord simuler des demandes (option 3)!\n");
                } else {
                    printf("=== APPLICATION STRATEGIE EQUITABLE ===\n");
                    for (int i = 0; i < entreprise.nb_demandes; i++) {
                        traiter_demande(&entreprise, i, 3);
                    }
                    printf("\n=== ETAT FINAL DU STOCK ===\n");
                    afficher_stock(&entreprise);
                }
                break;
                
            case 8:
                clear_screen();
                if (!demandes_simulees) {
                    printf("Veuillez d'abord simuler des demandes (option 3)!\n");
                } else {
                    printf("=== APPLICATION STRATEGIE PRIORITAIRE ===\n");
                    for (int i = 0; i < entreprise.nb_demandes; i++) {
                        traiter_demande(&entreprise, i, 4);
                    }
                    printf("\n=== ETAT FINAL DU STOCK ===\n");
                    afficher_stock(&entreprise);
                }
                break;
                
            case 9:
                clear_screen();
                printf("\n=== COMPARAISON DES STRATEGIES ===\n\n");
                printf("STATIQUE: Simple mais rigide\n");
                printf("DYNAMIQUE: Adaptatif et efficace\n");
                printf("EQUITABLE: Juste mais peu performant\n");
                printf("PRIORITAIRE: Performant mais inegalitaire\n");
                printf("\nRecommandation: La strategie DYNAMIQUE offre\nle meilleur compromis performance/equite.\n");
                break;
                
            case 10:
                clear_screen();
                initialiser_entreprise(&entreprise);
                demandes_simulees = 0;
                printf("Systeme reinitialise avec succes!\n");
                break;
                
            case 0:
                printf("\nMerci d'avoir utilise le systeme d'allocation!\n");
                printf("Au revoir!\n");
                break;
                
            default:
                printf("Choix invalide! Veuillez choisir entre 0 et 10.\n");
                break;
        }
        
        if (choix != 0) {
            attendre_entree();
            clear_screen();
        }
        
    } while (choix != 0);
    
    return 0;
}