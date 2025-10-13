#ifndef ENTREPRISE_ALLOCATION_H
#define ENTREPRISE_ALLOCATION_H

#define MAX_PRODUITS 10
#define MAX_EMPLOYES 5
#define MAX_CLIENTS 8

// Structure pour un produit électronique
typedef struct {
    int id;
    char nom[50];
    int quantite_stock;       // Quantité totale disponible
    int quantite_reservee;    // Quantité actuellement réservée
    float prix;
    int priorite;            // Priorité du produit (1-5)
} Produit;

// Structure pour un employé
typedef struct {
    int id;
    char nom[50];
    char role[30];           // "vendeur", "manager", "technicien"
    int niveau_priorite;     // 1-3 (1=élevé, 3=bas)
} Employe;

// Structure pour une demande d'allocation
typedef struct {
    int id_demande;
    int id_client;
    int id_produit;
    int quantite_demandee;
    int employe_attribue;    // ID de l'employé qui gère
    int statut;              // 0=en attente, 1=approuvée, 2=refusée
    int priorite_client;     // 1-5 (1=VIP, 5=normal)
    float budget_client;
} Demande;

// Structure du système d'allocation
typedef struct {
    Produit produits[MAX_PRODUITS];
    Employe employes[MAX_EMPLOYES];
    Demande demandes[MAX_CLIENTS];
    int nb_produits;
    int nb_employes;
    int nb_demandes;
} SystemeEntreprise;

// Fonctions d'initialisation
void initialiser_entreprise(SystemeEntreprise *sys);
void initialiser_produits(SystemeEntreprise *sys);
void initialiser_employes(SystemeEntreprise *sys);

// Fonctions d'affichage
void afficher_stock(SystemeEntreprise *sys);
void afficher_employes(SystemeEntreprise *sys);
void afficher_demandes(SystemeEntreprise *sys);

// Stratégies d'allocation
void allocation_statique(SystemeEntreprise *sys, int id_demande);
void allocation_dynamique(SystemeEntreprise *sys, int id_demande);
void allocation_equitable(SystemeEntreprise *sys);
void allocation_prioritaire(SystemeEntreprise *sys);

// Fonctions utilitaires
int verifier_stock(SystemeEntreprise *sys, int id_produit, int quantite);
void traiter_demande(SystemeEntreprise *sys, int id_demande, int strategie);
void simuler_scenarios(SystemeEntreprise *sys);

#endif