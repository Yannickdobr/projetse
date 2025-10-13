#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entreprise_allocation.h"

// Initialise le système d'entreprise avec des données de test
void initialiser_entreprise(SystemeEntreprise *sys) {
    sys->nb_produits = 0;
    sys->nb_employes = 0;
    sys->nb_demandes = 0;
    
    // Initialiser les produits
    initialiser_produits(sys);
    
    // Initialiser les employés
    initialiser_employes(sys);
}

// Initialise les produits électroniques
void initialiser_produits(SystemeEntreprise *sys) {
    // Création des produits électroniques
    Produit p1 = {0, "iPhone 15", 50, 0, 999.99, 1};
    Produit p2 = {1, "Samsung Galaxy", 30, 0, 899.99, 2};
    Produit p3 = {2, "MacBook Pro", 20, 0, 1999.99, 1};
    Produit p4 = {3, "iPad Air", 40, 0, 599.99, 3};
    Produit p5 = {4, "AirPods Pro", 100, 0, 249.99, 4};
    
    // Ajout des produits au système
    sys->produits[sys->nb_produits++] = p1;
    sys->produits[sys->nb_produits++] = p2;
    sys->produits[sys->nb_produits++] = p3;
    sys->produits[sys->nb_produits++] = p4;
    sys->produits[sys->nb_produits++] = p5;
}

// Initialise les employés
void initialiser_employes(SystemeEntreprise *sys) {
    // Création des employés
    Employe e1 = {0, "Alice Dupont", "manager", 1};
    Employe e2 = {1, "Bob Martin", "vendeur", 2};
    Employe e3 = {2, "Carol Wilson", "vendeur", 2};
    Employe e4 = {3, "David Lee", "technicien", 3};
    Employe e5 = {4, "Eva Garcia", "vendeur", 2};
    
    // Ajout des employés au système
    sys->employes[sys->nb_employes++] = e1;
    sys->employes[sys->nb_employes++] = e2;
    sys->employes[sys->nb_employes++] = e3;
    sys->employes[sys->nb_employes++] = e4;
    sys->employes[sys->nb_employes++] = e5;
}

// Affiche le stock des produits
void afficher_stock(SystemeEntreprise *sys) {
    printf("\n=== STOCK DES PRODUITS ===\n");
    printf("ID | Nom                | Stock | Reserve | Prix    | Priorite\n");
    printf("---+--------------------+-------+---------+---------+---------\n");
    
    for (int i = 0; i < sys->nb_produits; i++) {
        printf("%2d | %-18s | %5d | %7d | $%6.2f | %d\n",
               sys->produits[i].id,
               sys->produits[i].nom,
               sys->produits[i].quantite_stock,
               sys->produits[i].quantite_reservee,
               sys->produits[i].prix,
               sys->produits[i].priorite);
    }
}

// Affiche la liste des employés
void afficher_employes(SystemeEntreprise *sys) {
    printf("\n=== LISTE DES EMPLOYES ===\n");
    printf("ID | Nom           | Role        | Priorite\n");
    printf("---+---------------+-------------+---------\n");
    
    for (int i = 0; i < sys->nb_employes; i++) {
        printf("%2d | %-13s | %-11s | %d\n",
               sys->employes[i].id,
               sys->employes[i].nom,
               sys->employes[i].role,
               sys->employes[i].niveau_priorite);
    }
}

// Affiche les demandes en cours
void afficher_demandes(SystemeEntreprise *sys) {
    printf("\n=== DEMANDES EN COURS ===\n");
    
    if (sys->nb_demandes == 0) {
        printf("Aucune demande en cours.\n");
        return;
    }
    
    for (int i = 0; i < sys->nb_demandes; i++) {
        Demande *d = &sys->demandes[i];
        Produit *p = &sys->produits[d->id_produit];
        
        char statut_str[20];
        switch(d->statut) {
            case 0: strcpy(statut_str, "EN ATTENTE"); break;
            case 1: strcpy(statut_str, "APPROUVEE"); break;
            case 2: strcpy(statut_str, "REFUSEE"); break;
            default: strcpy(statut_str, "INCONNU"); break;
        }
        
        printf("Demande %d: Client %d -> %d x %s\n", 
               d->id_demande, d->id_client, d->quantite_demandee, p->nom);
        printf("         Priorite: %d, Budget: $%.2f, Statut: %s\n",
               d->priorite_client, d->budget_client, statut_str);
        
        if (d->employe_attribue != -1) {
            printf("         Employe: %s\n", 
                   sys->employes[d->employe_attribue].nom);
        }
        printf("\n");
    }
}

// Vérifie si le stock est suffisant pour une demande
int verifier_stock(SystemeEntreprise *sys, int id_produit, int quantite) {
    // Vérifier si le produit existe
    if (id_produit < 0 || id_produit >= sys->nb_produits) {
        return 0; // Produit inexistant
    }
    
    // Pointeur vers le produit demandé
    Produit *p = &sys->produits[id_produit];
    
    // Calculer la quantité disponible
    int disponible = p->quantite_stock - p->quantite_reservee;
    
    // Retourner 1 si le stock est suffisant, 0 sinon
    return (disponible >= quantite);
}

// Stratégie d'allocation STATIQUE
// Chaque employé a des produits fixes à gérer
void allocation_statique(SystemeEntreprise *sys, int id_demande) {
    printf("\n=== STRATEGIE STATIQUE ===\n");
    printf("Chaque employé gère des produits spécifiques\n");
    
    // Vérifier si la demande existe
    if (id_demande < 0 || id_demande >= sys->nb_demandes) {
        printf("Erreur: Demande invalide!\n");
        return;
    }
    
    // Pointeur vers la demande
    Demande *demande = &sys->demandes[id_demande];
    int id_produit = demande->id_produit;
    
    // Attribution statique basée sur l'ID du produit
    // Utilisation du modulo pour répartir entre les employés
    int employe_attribue = id_produit % sys->nb_employes;
    
    // Attribution de l'employé à la demande
    demande->employe_attribue = employe_attribue;
    
    printf("Demande %d attribuée à %s (produit ID %d)\n",
           id_demande,
           sys->employes[employe_attribue].nom,
           id_produit);
}

// Stratégie d'allocation DYNAMIQUE
// Attribution basée sur la charge de travail actuelle
void allocation_dynamique(SystemeEntreprise *sys, int id_demande) {
    printf("\n=== STRATEGIE DYNAMIQUE ===\n");
    printf("Attribution basée sur la charge de travail\n");
    
    // Vérifier si la demande existe
    if (id_demande < 0 || id_demande >= sys->nb_demandes) {
        printf("Erreur: Demande invalide!\n");
        return;
    }
    
    // Tableau pour compter le nombre de demandes par employé
    int charge[MAX_EMPLOYES] = {0};
    
    // Compter les demandes actuelles de chaque employé
    for (int i = 0; i < sys->nb_demandes; i++) {
        if (sys->demandes[i].employe_attribue != -1) {
            charge[sys->demandes[i].employe_attribue]++;
        }
    }
    
    // Trouver l'employé avec la charge la plus faible
    int employe_min_charge = 0;
    for (int i = 1; i < sys->nb_employes; i++) {
        if (charge[i] < charge[employe_min_charge]) {
            employe_min_charge = i;
        }
    }
    
    // Attribuer la demande à l'employé le moins chargé
    sys->demandes[id_demande].employe_attribue = employe_min_charge;
    
    printf("Demande %d attribuée à %s (charge: %d demandes)\n",
           id_demande,
           sys->employes[employe_min_charge].nom,
           charge[employe_min_charge]);
}

// Stratégie d'allocation ÉQUITABLE
// Répartit également les ressources entre tous les clients
void allocation_equitable(SystemeEntreprise *sys) {
    printf("\n=== STRATEGIE EQUITABLE ===\n");
    printf("Repartition equitable des ressources\n");
    
    // Tableau pour stocker la quantité maximale par client pour chaque produit
    int max_par_client[MAX_PRODUITS];
    
    // Calculer la quantité maximale par client pour chaque produit
    for (int i = 0; i < sys->nb_produits; i++) {
        // Répartir équitablement le stock entre tous les clients
        max_par_client[i] = sys->produits[i].quantite_stock / sys->nb_demandes;
        
        // Assurer au moins 1 unité par client si possible
        if (max_par_client[i] < 1) {
            max_par_client[i] = 1;
        }
        
        printf("Produit %s: max %d unites par client\n",
               sys->produits[i].nom, max_par_client[i]);
    }
    
    // Traiter les demandes en respectant les limites équitables
    for (int i = 0; i < sys->nb_demandes; i++) {
        Demande *demande = &sys->demandes[i];
        Produit *produit = &sys->produits[demande->id_produit];
        
        // Réduire la quantité demandée si elle dépasse la limite équitable
        if (demande->quantite_demandee > max_par_client[demande->id_produit]) {
            printf("Client %d: reduction de %d a %d unites de %s\n",
                   demande->id_client,
                   demande->quantite_demandee,
                   max_par_client[demande->id_produit],
                   produit->nom);
            
            // Appliquer la limite
            demande->quantite_demandee = max_par_client[demande->id_produit];
        }
        
        // Attribution équitable des employés (répartition circulaire)
        demande->employe_attribue = i % sys->nb_employes;
    }
}

// Stratégie d'allocation PRIORITAIRE
// Donne la priorité aux clients VIP et produits importants
void allocation_prioritaire(SystemeEntreprise *sys) {
    printf("\n=== STRATEGIE PRIORITAIRE ===\n");
    printf("Priorite aux clients VIP et produits importants\n");
    
    // Trier les demandes par priorité (VIP d'abord)
    // Utilisation d'un tri à bulles simple
    for (int i = 0; i < sys->nb_demandes - 1; i++) {
        for (int j = i + 1; j < sys->nb_demandes; j++) {
            // Échanger si la priorité de i est plus faible (nombre plus élevé)
            if (sys->demandes[i].priorite_client > sys->demandes[j].priorite_client) {
                // Échanger les demandes
                Demande temp = sys->demandes[i];
                sys->demandes[i] = sys->demandes[j];
                sys->demandes[j] = temp;
            }
        }
    }
    
    // Traiter d'abord les demandes prioritaires
    for (int i = 0; i < sys->nb_demandes; i++) {
        Demande *demande = &sys->demandes[i];
        Produit *produit = &sys->produits[demande->id_produit];
        
        // Les clients prioritaires obtiennent les employés prioritaires
        if (demande->priorite_client <= 2) { // VIP (priorité 1 ou 2)
            demande->employe_attribue = 0; // Manager pour les VIP
        } else {
            // Répartir les autres clients entre les vendeurs
            demande->employe_attribue = (i % (sys->nb_employes - 1)) + 1;
        }
        
        printf("Client %d (priorite %d) -> %s\n",
               demande->id_client,
               demande->priorite_client,
               sys->employes[demande->employe_attribue].nom);
    }
}

// Traite une demande avec une stratégie spécifique
void traiter_demande(SystemeEntreprise *sys, int id_demande, int strategie) {
    // Vérifier si la demande existe
    if (id_demande < 0 || id_demande >= sys->nb_demandes) {
        printf("Erreur: Demande invalide!\n");
        return;
    }
    
    // Pointeurs vers la demande et le produit
    Demande *demande = &sys->demandes[id_demande];
    Produit *produit = &sys->produits[demande->id_produit];
    
    printf("\nTraitement de la demande %d:\n", id_demande);
    printf("Client %d demande %d unites de %s\n",
           demande->id_client, demande->quantite_demandee, produit->nom);
    
    // Vérifier le stock disponible
    if (!verifier_stock(sys, demande->id_produit, demande->quantite_demandee)) {
        printf("Stock insuffisant! Disponible: %d\n",
               produit->quantite_stock - produit->quantite_reservee);
        demande->statut = 2; // Refusée
        return;
    }
    
    // Appliquer la stratégie d'allocation choisie
    switch (strategie) {
        case 1: 
            allocation_statique(sys, id_demande); 
            break;
        case 2: 
            allocation_dynamique(sys, id_demande); 
            break;
        case 3: 
            allocation_equitable(sys); 
            break;
        case 4: 
            allocation_prioritaire(sys); 
            break;
        default: 
            printf("Strategie inconnue!\n"); 
            return;
    }
    
    // Réserver le stock
    produit->quantite_reservee += demande->quantite_demandee;
    demande->statut = 1; // Approuvée
    
    printf("Demande approuvee! Stock reserve: %d/%d\n",
           produit->quantite_reservee, produit->quantite_stock);
}

// Simule différents scénarios de demande
void simuler_scenarios(SystemeEntreprise *sys) {
    printf("\n=== SIMULATION DE DEMANDES ===\n");
    
    // Créer des demandes de test variées
    Demande d1 = {0, 101, 0, 2, -1, 0, 1, 2000.0}; // VIP veut iPhone
    Demande d2 = {1, 102, 1, 1, -1, 0, 5, 1000.0}; // Normal veut Samsung
    Demande d3 = {2, 103, 2, 1, -1, 0, 2, 2500.0}; // VIP veut MacBook
    Demande d4 = {3, 104, 0, 5, -1, 0, 3, 5000.0}; // Moyen veut iPhone
    Demande d5 = {4, 105, 3, 3, -1, 0, 4, 2000.0}; // Normal veut iPad
    
    // Ajouter les demandes au système
    sys->demandes[sys->nb_demandes++] = d1;
    sys->demandes[sys->nb_demandes++] = d2;
    sys->demandes[sys->nb_demandes++] = d3;
    sys->demandes[sys->nb_demandes++] = d4;
    sys->demandes[sys->nb_demandes++] = d5;
    
    printf("5 demandes creees pour la simulation:\n");
    printf("- Client VIP 101: 2 iPhone 15\n");
    printf("- Client Normal 102: 1 Samsung Galaxy\n");
    printf("- Client VIP 103: 1 MacBook Pro\n");
    printf("- Client Moyen 104: 5 iPhone 15\n");
    printf("- Client Normal 105: 3 iPad Air\n");
}